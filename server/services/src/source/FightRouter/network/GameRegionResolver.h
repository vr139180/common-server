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

#ifndef __GAMEREGIONRESOLVER_H__
#define __GAMEREGIONRESOLVER_H__

#include <set>
#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/commons/LinkFromHolder.h>
#include "network/GameServiceLinkFrom.h"

//TODO:���� gameservice��������⣬Ϊ��̬�������ο�
class GameRegionResolver : public LinkFromHolder<GameServiceLinkFrom>
{
	typedef LinkFromHolder<GameServiceLinkFrom> base;
	typedef std::vector<S_INT_64> GAMESERVICE_V;
	typedef boost::unordered_map<S_INT_32, GAMESERVICE_V> GSVR_MAP_REGION;
public:
	GameRegionResolver();
	virtual ~GameRegionResolver();

	virtual void return_freelink(GameServiceLinkFrom* link);

	void dispath_to_game(NetProtocol* pro);
	//ת��������gate֮��ķ����Э��
	void dispatch_to_game_from_othsvr(NetProtocol* pro);

	void bind_to_region(GameServiceLinkFrom* plink, S_INT_32 regionid);

protected:
	//����һ�����õķ���
	GameServiceLinkFrom* ask_free_gameservice_random( S_INT_32 regionid);

	GAMESERVICE_V& get_service_of_region_nomutx(S_INT_32 regionid);
	void regist_to_region_nomutx(S_INT_32 regionid, S_INT_64 gid);
	void unregist_from_region_nomutx(S_INT_32 regionid, S_INT_64 gid);

	S_INT_64 random_gservice_of_region_nomutx(S_INT_32 regionid);

private:
	//region�ͷ����ӳ���ϵ
	GSVR_MAP_REGION		gsvr_region_map_;
};

#endif //__GAMEREGIONRESOLVER_H__
