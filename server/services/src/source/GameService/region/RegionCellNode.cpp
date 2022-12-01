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

#include "region/RegionCellNode.h"
#include <worldsLib/cmsworld_const.h>

#include "region/GamePlayer.h"

RegionCellNode::RegionCellNode():b_join_region_(false), owner_regionid_(0),
column_(0), row_(0)
{
}

void RegionCellNode::init_cell(S_INT_32 c, S_INT_32 r, bool joinregion, S_INT_32 regionid)
{
	this->column_ = c;
	this->row_ = r;
	this->b_join_region_ = joinregion;
	this->owner_regionid_ = regionid;
}

CMSPointXZ RegionCellNode::center_point()
{
	return CMSPointXZ(column_ * REGION_CELL_SIZE + REGION_CELL_SIZE / 2.0, row_ * REGION_CELL_SIZE + REGION_CELL_SIZE / 2.0);
}

void RegionCellNode::broadcast(BasicProtocol* msg)
{
	for (std::set<GamePlayer*>::iterator iter = node_players_.begin(); iter != node_players_.end(); ++iter)
	{
		GamePlayer* p = (*iter);

		BasicProtocol* x = msg->New();
		x->CopyFrom(*msg);
		p->send_to_gate(x);
	}
}

void RegionCellNode::freeze()
{
	node_players_.clear();
}

//fromsource: 返回的是，当前同步帧 如果有in的时候的from ,如果没有就是返回自身作为from起源
bool RegionCellNode::player_out_node(GamePlayer* puser, RegionCellNode* to, RegionCellNode** fromsource)
{
	std::set<GamePlayer*>::iterator xfiter = std::find(node_players_.begin(), node_players_.end(), puser);
	if (xfiter == node_players_.end())
		return false;

	node_players_.erase(xfiter);
	puser->unbind_region_owner();

	//处理inout计算
	//out时检测是否之前有in,有in 那么，清除in不记录out
	INOUT_MAP::iterator fiter = frame_in_players_.find(puser);
	if (fiter != frame_in_players_.end())
	{
		*fromsource = fiter->second;

		frame_in_players_.erase(fiter);
		
		return true;
	}

	*fromsource = this;
	frame_out_players_[puser] = to;
	
	return true;
}

void RegionCellNode::player_sourcefrom_update_logout(GamePlayer* puser)
{
	INOUT_MAP::iterator fiter = frame_out_players_.find(puser);
	if (fiter == frame_out_players_.end())
		return;

	frame_out_players_.erase(fiter);

	//加入out列表
	frame_logout_players_.insert(puser->get_useriid());
}

void RegionCellNode::player_logout_node(GamePlayer* puser)
{
	std::set<GamePlayer*>::iterator xfiter = std::find(node_players_.begin(), node_players_.end(), puser);
	if (xfiter == node_players_.end())
		return;

	node_players_.erase(xfiter);
	puser->unbind_region_owner();

	//检查是否有in
	INOUT_MAP::iterator fiter = frame_in_players_.find(puser);
	if (fiter != frame_in_players_.end())
	{
		RegionCellNode* fromsource = fiter->second;
		frame_in_players_.erase(fiter);

		if (fromsource != 0)
			fromsource->player_sourcefrom_update_logout(puser);

		return;
	}

	//处理out计算
	frame_logout_players_.insert(puser->get_useriid());
}

void RegionCellNode::player_sourcefrom_update_tonode(GamePlayer* puser, RegionCellNode* to)
{
	INOUT_MAP::iterator fiter = frame_out_players_.find(puser);
	if (fiter == frame_out_players_.end())
		return;

	frame_out_players_[puser] = to;
}

//join, from:需要查看 from node中的out,修改它的 frame_out关联的to
void RegionCellNode::player_join_node(GamePlayer* puser, RegionCellNode* from)
{
	//过滤已经进入的
	std::set<GamePlayer*>::iterator xfiter = std::find(node_players_.begin(), node_players_.end(), puser);
	if (xfiter != node_players_.end())
		return;

	node_players_.insert(puser);
	puser->bind_region_owner(this);

	//处理inout计算
	//in时检测之前是否有out, 有out 那么清除out,不记录in
	INOUT_MAP::iterator fiter = frame_out_players_.find(puser);
	if (fiter != frame_out_players_.end())
	{
		frame_out_players_.erase(fiter);
		return;
	}
	std::set<S_INT_64>::iterator fiter2 = frame_logout_players_.find(puser->get_useriid());
	if (fiter2 != frame_logout_players_.end())
	{
		frame_logout_players_.erase(fiter2);
		return;
	}

	frame_in_players_[puser] = from;

	if (from != 0)
		from->player_sourcefrom_update_tonode(puser, this);
}

void RegionCellNode::reset_frameinfo()
{
	frame_in_players_.clear();
	frame_out_players_.clear();
	frame_logout_players_.clear();
}

bool RegionCellNode::has_cellnode_changed()
{
	return (frame_in_players_.size() + frame_out_players_.size() + frame_logout_players_.size()) > 0;
}
