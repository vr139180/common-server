// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "network/GameRegionResolver.h"

#include <cmsLib/util/Random.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/service_type.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <worldsLib/utils/WorldUtil.h>
#include <worldsLib/GameWorldHelper.h>

#include "FightRouterApp.h"

USE_PROTOCOL_NAMESPACE

GameRegionResolver::GameRegionResolver():base()
{
}

GameRegionResolver::~GameRegionResolver()
{
	gsvr_region_map_.clear();
}

GameRegionResolver::GAMESERVICE_V& GameRegionResolver::get_service_of_region_nomutx(S_INT_32 regionid)
{
	GSVR_MAP_REGION::iterator fiter = gsvr_region_map_.find(regionid);
	if (fiter == gsvr_region_map_.end())
	{
		gsvr_region_map_[regionid] = GAMESERVICE_V();
		fiter = gsvr_region_map_.find(regionid);
	}

	return fiter->second;
}

void GameRegionResolver::regist_to_region_nomutx(S_INT_32 regionid, S_INT_64 gid)
{
	GAMESERVICE_V& gids = get_service_of_region_nomutx(regionid);
	gids.push_back(gid);
}

void GameRegionResolver::unregist_from_region_nomutx(S_INT_32 regionid, S_INT_64 gid)
{
	GAMESERVICE_V& gids = get_service_of_region_nomutx(regionid);
	GAMESERVICE_V::iterator fiter = std::find(gids.begin(), gids.end(), gid);
	if( fiter != gids.end())
		gids.erase(fiter);
}

S_INT_64 GameRegionResolver::random_gservice_of_region_nomutx(S_INT_32 regionid)
{
	GAMESERVICE_V& gids = get_service_of_region_nomutx(regionid);

	if (gids.size() == 0)
		return 0;

	int cnt = CMS_RandMax((int)gids.size());
	GAMESERVICE_V::iterator iter = gids.begin();
	for (; iter != gids.end() && cnt > 0; ++iter, --cnt);

	return (*iter);
}

void GameRegionResolver::return_freelink(GameServiceLinkFrom* link)
{
	S_INT_64 gid = link->get_iid();
	S_INT_32 rid = link->get_bind_regionid();
	if( rid > 0 && gid > 0)
	{
		ThreadLockWrapper guard(lock_);

		unregist_from_region_nomutx(rid, gid);
	}

	base::return_freelink(link);
}

void GameRegionResolver::bind_to_region(GameServiceLinkFrom* plink, S_INT_32 regionid)
{
	plink->bind_region(regionid);
	S_INT_64 gid = plink->get_iid();

	{
		ThreadLockWrapper guard(lock_);

		regist_to_region_nomutx(regionid, gid);
	}

	logDebug(out_runtime, "----fightrouter gameservice:%lld bind to region:%d", gid, regionid);
}

GameServiceLinkFrom* GameRegionResolver::ask_free_gameservice_random(S_INT_32 regionid)
{
	S_INT_64 gid = 0;
	{
		ThreadLockWrapper guard(lock_);

		gid = random_gservice_of_region_nomutx(regionid);
	}

	return get_servicelink_byiid(gid);
}

void GameRegionResolver::dispatch_to_game_from_othsvr(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> xptr(pro);

	S_INT_64 gsvrid = WorldUtil::get_svrid_from_gameid(pro->get_gameid());
	GameServiceLinkFrom* plink = get_servicelink_byiid(gsvrid);
	if (plink == 0)
		return;
	
	plink->send_protocol(xptr.release());
}

void GameRegionResolver::dispath_to_game(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> xptr(pro);
	S_UINT_16 msgid = pro->get_msg();
	if (msgid == GMS_PROTYPE::GMS_ENTERGAME_REQ)
	{
		Game_EnterGame_req* req = dynamic_cast<Game_EnterGame_req*>(pro->msg_);

		S_INT_64 gid = 0;
		S_INT_32 cid = 0, rid = 0;

		S_INT_64 gameid = req->game_iid();
		if (gameid > 0)
		{
			gid = WorldUtil::get_svrid_from_gameid(gameid);
			cid = WorldUtil::get_channel_from_gameid(gameid);
			rid = WorldUtil::get_regionid_from_gameid(gameid);
		}

		GameServiceLinkFrom* plink = 0;

		GLoc3D loc;
		ProtoUtil::get_location_from_msg(req, loc);
		
		logDebug(out_runtime, "fightrouter recv user:%lld enter game request, loc:%s", pro->get_useriid(), loc.to_string().c_str());

		if (loc.is_zero_point())
		{
			//获取进入世界的第一个点
			S_INT_32 srid = 0;
			GameWorldHelper::instance().get_hello_world(loc, srid);
			//处理relogin的情况
			//如果位置和记录的gameid服务信息不一致，就重选gameservice
			//原来的通过用户激活超时来维护
			if (srid != rid)
			{
				rid = srid;
				cid = 0;
				gid = 0;

				//重选gameid
				req->set_game_iid(0);
			}

			//更新位置
			ProtoUtil::set_location_to_msg(req, loc);
		}
		else
		{
			S_INT_32 srid = 0;
			GameWorldHelper::instance().get_regionid_from_loc(loc, srid);
			//处理数据不一致的情况
			if (srid != rid)
			{
				rid = srid;
				cid = 0;
				gid = 0;

				//重选gameid
				req->set_game_iid(0);
			}
		}
		
		if (gid != 0)
		{
			//relogin的情况
			plink = get_servicelink_byiid(gid);
			if (plink == 0)
			{
				//重选gameid
				req->set_game_iid(0);

				plink = ask_free_gameservice_random(rid);
			}
		}
		else
		{
			plink = ask_free_gameservice_random(rid);
		}

		//无可用服务
		if (plink == 0)
		{
			//无可用服务
			Game_EnterGame_ack* ack = new Game_EnterGame_ack();
			ack->set_result(3);

			NetProtocol* pro2 = new NetProtocol(pro->head_, ack);

			SProtocolHead& head = pro2->write_head();
			head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER;
			head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

			svrApp.router_to_gate(pro2);
			return;
		}
		else
		{
			plink->send_protocol(xptr.release());
		}
	}
	else if (msgid == GMS_PROTYPE::GMS_USERALIVE_NTF)
	{
		//useralive只通过gameid进行激活，特殊处理
		S_INT_64 gsvrid = WorldUtil::get_svrid_from_gameid(pro->get_gameid());
		GameServiceLinkFrom* plink = get_servicelink_byiid(gsvrid);
		if (plink != 0)
			plink->send_protocol(xptr.release());
	}
	else
	{
		S_INT_64 gsvrid = WorldUtil::get_svrid_from_gameid(pro->get_gameid());
		GameServiceLinkFrom* plink = get_servicelink_byiid(gsvrid);
		if (plink == 0)
		{
			//服务失效，重选gameservice
			{
				Game_ServiceDisable_ntf* ntf = new Game_ServiceDisable_ntf();
				NetProtocol* pro2 = new NetProtocol(pro->head_, ntf);

				SProtocolHead& head = pro2->write_head();
				head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER;
				head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

				svrApp.router_to_gate(pro2);
			}

			//重选game service
			GLoc3D loc;
			ProtoUtil::get_location_from_msg(pro->msg_, loc);
			S_INT_32 rid = 0;
			if (!GameWorldHelper::instance().get_regionid_from_loc(loc, rid))
			{
				//位置错误
				Game_EnterGame_ack* ack = new Game_EnterGame_ack();
				ack->set_result(2);

				NetProtocol* pro2 = new NetProtocol(pro->head_, ack);

				SProtocolHead& head = pro2->write_head();
				head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER;
				head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

				svrApp.router_to_gate(pro2);
				return;
			}
			else
			{
				plink = ask_free_gameservice_random(rid);
				if (plink == 0)
				{
					//无可用服务
					Game_EnterGame_ack* ack = new Game_EnterGame_ack();
					ack->set_result(3);

					NetProtocol* pro2 = new NetProtocol(pro->head_, ack);

					SProtocolHead& head = pro2->write_head();
					head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER;
					head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

					svrApp.router_to_gate(pro2);
					return;
				}
				else
				{
					Game_EnterGame_req* req = new Game_EnterGame_req();
					ProtoUtil::set_location_to_msg(req, loc);
					//新进入gameservice
					req->set_game_iid(0);

					plink->send_netprotocol(req);
					return;
				}
			}
		}
		else
		{
			plink->send_protocol(xptr.release());
		}
	}
}
