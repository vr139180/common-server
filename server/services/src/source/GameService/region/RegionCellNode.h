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

#ifndef __REGIONCELLNODE_H__
#define __REGIONCELLNODE_H__

#include <set>
#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <worldsLib/geometry/common_types.h>

class GamePlayer;

class RegionCellNode
{
public:
	typedef boost::unordered_map<GamePlayer*, RegionCellNode*> INOUT_MAP;
public:
	RegionCellNode();
	void init_cell(S_INT_32 c, S_INT_32 r, bool joinregion, S_INT_32 regionid);

	void freeze();

	void reset_frameinfo();
	bool has_cellnode_changed();
	bool is_empty_cell() { return node_players_.size() == 0; }

	//msg调用方不负责释放
	void broadcast(BasicProtocol* msg);

public:
	bool player_out_node(GamePlayer* puser, RegionCellNode* to, RegionCellNode** fromsource);
	void player_join_node(GamePlayer* puser, RegionCellNode* from);
	void player_sourcefrom_update_tonode(GamePlayer* puser, RegionCellNode* to);
	void player_sourcefrom_update_logout(GamePlayer* puser);

	void player_logout_node(GamePlayer* puser);

public:
	const std::set<GamePlayer*>& all_node_players() const { return node_players_; }
	const INOUT_MAP& frame_int_players() const { return frame_in_players_; }
	const INOUT_MAP& frame_out_players() const { return frame_out_players_; }
	const std::set<S_INT_64>& frame_logout_players() const { return frame_logout_players_; }

private:
	//节点包含的用户信息
	std::set<GamePlayer*>	node_players_;

	//临时的，inout结构
	INOUT_MAP	frame_in_players_;
	INOUT_MAP	frame_out_players_;
	//logout的用户
	std::set<S_INT_64>	frame_logout_players_;

public:
	S_INT_32 column() const { return column_; }
	S_INT_32 row() const { return row_; }
	CMSPointXZ center_point();

private:
	//是否地图交汇处
	bool		b_join_region_;
	//所属 region
	S_INT_32	owner_regionid_;
	//node (x,y)
	S_INT_32	column_;
	S_INT_32	row_;
};

#endif //__REGIONCELLNODE_H__