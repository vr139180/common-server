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

#include <gameLib/protobuf/Proto_all.h>
#include <worldsLib/cmsworld_const.h>
#include <worldsLib/GameRegionMeta.h>

#include "region/GamePlayer.h"

void RegionMapBoxImpl::user_enter_region(GamePlayer* puser)
{
	RegionCellNode* pcell = puser->get_region_owner();
	RegionCellNode* ncell = this->get_cellnode_byloc(puser->get_location());
	//容错处理
	if (ncell == 0)
		return;

	//处理用户保存问题
	if (pcell == ncell)
		return;

	RegionCellNode* fromsource = 0;
	if (pcell != 0)
		pcell->player_out_node(puser, ncell, &fromsource);

	ncell->player_join_node(puser, fromsource);
}

void RegionMapBoxImpl::user_logout_region(GamePlayer* puser)
{
	RegionCellNode* pcell = puser->get_region_owner();
	if (pcell == 0)
		return;

	pcell->player_logout_node(puser);
}

void RegionMapBoxImpl::user_move_region(GamePlayer* puser)
{
	RegionCellNode* pcell = puser->get_region_owner();
	RegionCellNode* ncell = this->get_cellnode_byloc(puser->get_location());
	if (ncell == 0 || pcell == 0 || ncell == pcell)
		return;

	RegionCellNode* fromsource = 0;
	pcell->player_out_node(puser, ncell, &fromsource);
	ncell->player_join_node(puser, fromsource);
}

//TODO:用RTree优化 数据变化查询
void RegionMapBoxImpl::user_inout_tick()
{
	for (int xc = 0; xc < max_column_cells_; ++xc)
	{
		for (int zc = 0; zc < max_row_cells_; ++zc)
		{
			RegionCellNode* pnode = &(cell_nodes_[xz_to_array(xc, zc)]);
			if (pnode->is_empty_cell())
				continue;

			notify_cellnode_inout(pnode);
		}
	}

	for (int xc = 0; xc < max_column_cells_; ++xc)
	{
		for (int zc = 0; zc < max_row_cells_; ++zc)
		{
			cell_nodes_[xz_to_array(xc, zc)].reset_frameinfo();
		}
	}
}

void RegionMapBoxImpl::notify_cellnode_inout(RegionCellNode* pnode)
{
	PRO::Game_UsersVisiable_ntf* ntf = new PRO::Game_UsersVisiable_ntf();
	ntf->set_full(false);
	std::unique_ptr<PRO::Game_UsersVisiable_ntf> xptr(ntf);

	//获取九宫格变动数据
	S_INT_32 start_c = pnode->column() - 1;
	if (start_c < 0)
		start_c = 0;
	S_INT_32 end_c = pnode->column() + 1;
	if (end_c >= max_column_cells_)
		end_c = max_column_cells_ - 1;
	S_INT_32 start_r = pnode->row() - 1;
	if (start_r < 0)
		start_r = 0;
	S_INT_32 end_r = pnode->row() + 1;
	if (end_r >= max_row_cells_)
		end_r = max_row_cells_ - 1;

	CMSBox check_region;
	{
		check_region.set_xz(start_c * REGION_CELL_SIZE * 1.0, (end_r + 1) * REGION_CELL_SIZE * 1.0);
		check_region.set_wh((end_c - start_c + 1) * REGION_CELL_SIZE * 1.0, (end_r - start_r + 1) * REGION_CELL_SIZE * 1.0);
	}

	bool bnonezero = true;
	for (int ii = start_c; ii <= end_c; ++ii)
	{
		for (int jj = start_r; jj <= end_r; ++jj)
		{
			RegionCellNode* pdata = &(cell_nodes_[xz_to_array(ii, jj)]);
			//in
			const RegionCellNode::INOUT_MAP& ins = pdata->frame_int_players();
			for (RegionCellNode::INOUT_MAP::const_iterator citer = ins.cbegin(); citer != ins.cend(); ++citer)
			{
				GamePlayer* p = citer->first;
				RegionCellNode* pn = citer->second;
				//pn == 0 表示不是从其他格移动过来的，不需要进行过滤
				if (pn != 0)
				{
					//9格内的in out过滤
					if (check_region.is_point_in_box(pn->center_point()))
						continue;
				}

				PRO::GameUserInfo* pui = ntf->add_online_users();
				p->copy_user_info(pui);
				bnonezero = false;
			}

			//out
			const RegionCellNode::INOUT_MAP& outs = pdata->frame_out_players();
			for (RegionCellNode::INOUT_MAP::const_iterator citer = outs.cbegin(); citer != outs.cend(); ++citer)
			{
				GamePlayer* p = citer->first;
				RegionCellNode* pn = citer->second;
				//9格内的in out过滤
				if (check_region.is_point_in_box(pn->center_point()))
					continue;

				ntf->add_offline_users(p->get_useriid());
				bnonezero = false;
			}

			//logout
			const std::set<S_INT_64>& louts = pdata->frame_logout_players();
			for (std::set<S_INT_64>::const_iterator citer = louts.cbegin(); citer != louts.end(); ++citer)
			{
				ntf->add_offline_users((*citer));
				bnonezero = false;
			}
		}
	}

	if (bnonezero)
		return;

	//广播给所有用户
	pnode->broadcast( ntf);
}
