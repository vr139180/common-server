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

#include "region/RegionMapBoxImpl.h"

#include <worldsLib/cmsworld_const.h>
#include <worldsLib/GameRegionMeta.h>

#include "region/GamePlayer.h"

RegionMapBoxImpl::RegionMapBoxImpl(RegionChannelService* p):base(),
max_column_cells_(0),max_row_cells_(0),
lt_column_(0),lt_row_(0),rb_column_(0), rb_row_(0),
region_id_(0),
owner_( p)
{
}

RegionMapBoxImpl::~RegionMapBoxImpl()
{
}

void RegionMapBoxImpl::init_region()
{
	//build cells
	GameRegionMeta& rmeta = GameRegionMeta::instance();

	region_id_ = rmeta.get_regionid();

	my_region_ = rmeta.get_region();
	lt_column_ = 0;
	lt_row_ = (S_INT_32)(my_region_.height() / REGION_CELL_SIZE) - 1;
	rb_column_ = (S_INT_32)(my_region_.width() / REGION_CELL_SIZE) - 1;
	rb_row_ = 0;

	max_region_ = my_region_;

	//top extends
	{
		const RegionJoinMeta& rj = rmeta.get_region_join(RegionJoinType::RegionJT_Top);
		if (rj.can_join())
		{
			max_region_.extend_top(rj.combine_cells());
		}
	}
	//buttom
	{
		const RegionJoinMeta& rj = rmeta.get_region_join(RegionJoinType::RegionJT_Buttom);
		if (rj.can_join())
		{
			max_region_.extend_buttom(rj.combine_cells());

			lt_row_ += rj.combine_cells();
			rb_row_ += rj.combine_cells();
		}
	}
	//left
	{
		const RegionJoinMeta& rj = rmeta.get_region_join(RegionJoinType::RegionJT_Left);
		if (rj.can_join())
		{
			max_region_.extend_left(rj.combine_cells());

			lt_column_ += rj.combine_cells();
			rb_column_ += rj.combine_cells();
		}
	}
	//right
	{
		const RegionJoinMeta& rj = rmeta.get_region_join(RegionJoinType::RegionJT_Right);
		if (rj.can_join())
		{
			max_region_.extend_right(rj.combine_cells());
		}
	}
	
	max_column_cells_ = (S_INT_32)(max_region_.width() / REGION_CELL_SIZE);
	max_row_cells_ = (S_INT_32)(max_region_.height() / REGION_CELL_SIZE);
	cell_nodes_.reset(new RegionCellNode[max_column_cells_*max_row_cells_]);
	for (int ii = 0; ii < max_column_cells_; ++ii)
	{
		for (int jj = 0; jj < max_row_cells_; ++jj)
		{
			cell_nodes_[xz_to_array(ii, jj)].init_cell(ii, jj, false, region_id_);
		}
	}
}

RegionCellNode* RegionMapBoxImpl::get_cellnode_byloc(const GLoc3D& loc)
{
	if (!max_region_.is_point_in_box(loc))
		return 0;

	//获取相对坐标点
	CMSPointXZ xz = max_region_.normlize(loc);
	S_INT_32 xc = (S_INT_32)(xz.x() / REGION_CELL_SIZE);
	S_INT_32 zc = (S_INT_32)(xz.z() / REGION_CELL_SIZE);
	return &cell_nodes_[xz_to_array(xc, zc)];
}

void RegionMapBoxImpl::get_ninecell_bycr(S_INT_32 c, S_INT_32 r, std::vector<S_INT_32>& nincecells)
{
	//获取九宫格变动数据
	S_INT_32 start_c = c - 1;
	if (start_c < 0)
		start_c = 0;
	S_INT_32 end_c = c + 1;
	if (end_c >= max_column_cells_)
		end_c = max_column_cells_ - 1;
	S_INT_32 start_r = r - 1;
	if (start_r < 0)
		start_r = 0;
	S_INT_32 end_r = r + 1;
	if (end_r >= max_row_cells_)
		end_r = max_row_cells_ - 1;

	nincecells.push_back(start_c);
	nincecells.push_back(end_c);
	nincecells.push_back(start_r);
	nincecells.push_back(end_r);
}

void RegionMapBoxImpl::user_view_broadcast(GamePlayer* puser, BasicProtocol* msg)
{
	RegionCellNode* pnode = puser->get_region_owner();
	if (pnode == 0)
		return;

	//获取九宫格变动数据
	std::vector<S_INT_32> nine;
	get_ninecell_bycr(pnode->column(), pnode->row(), nine);

	for (int ii = nine[0]; ii <= nine[1]; ++ii)
	{
		for (int jj = nine[2]; jj <= nine[3]; ++jj)
		{
			RegionCellNode* pdata = &(cell_nodes_[xz_to_array(ii, jj)]);
			pdata->broadcast(msg);
		}
	}
}

void RegionMapBoxImpl::user_view_first_sync(GamePlayer* puser)
{
	RegionCellNode* pnode = puser->get_region_owner();
	if (pnode == 0)
		return;

	std::vector<S_INT_32> nine;
	get_ninecell_bycr(pnode->column(), pnode->row(), nine);

	PRO::Game_UsersVisiable_ntf *ntf = new PRO::Game_UsersVisiable_ntf();
	ntf->set_full(true);

	for (int ii = nine[0]; ii <= nine[1]; ++ii)
	{
		for (int jj = nine[2]; jj <= nine[3]; ++jj)
		{
			RegionCellNode* pdata = &(cell_nodes_[xz_to_array(ii, jj)]);
			const std::set<GamePlayer*>& pls = pdata->all_node_players();

			for (std::set<GamePlayer*>::const_iterator citer = pls.cbegin(); citer != pls.cend(); ++citer)
			{
				GamePlayer* p = (*citer);
				PRO::GameUserInfo *pui = ntf->add_online_users();
				p->copy_user_info(pui);
			}
		}
	}

	puser->send_to_gate(ntf);
}

void RegionMapBoxImpl::region_node_user_changed( RegionCellNode* pnode)
{
	if (pnode == 0)
		return;

	if (pnode->is_empty_cell())
	{
		std::set<RegionCellNode*>::iterator fiter = std::find(regionnodes_have_users_.begin(), regionnodes_have_users_.end(), pnode);
		if (fiter != regionnodes_have_users_.end())
			regionnodes_have_users_.erase(fiter);
	}
	else
	{
		regionnodes_have_users_.insert(pnode);
	}
}

void RegionMapBoxImpl::inout_change_tmp_record(RegionCellNode* pnode)
{
	if (pnode == 0)
		return;

	inout_change_tmp_.insert(pnode);
}
