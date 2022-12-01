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

#ifndef __REGIONMAPBOXIMPL_H__
#define __REGIONMAPBOXIMPL_H__

#include <boost/scoped_array.hpp>

#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/commons/GLoc3D.h>
#include <worldsLib/geometry/common_types.h>
#include "region/IRegionMap.h"
#include "region/RegionCellNode.h"

class RegionChannelService;

class RegionMapBoxImpl : public IRegionMap
{
	typedef IRegionMap base;
public:
	RegionMapBoxImpl(RegionChannelService* p);
	virtual ~RegionMapBoxImpl();

public:
	//--------------------------IRegionMap implementation----------------------
	virtual void init_region();
	virtual S_INT_32 get_region_id() { return region_id_; }

	virtual void user_enter_region(GamePlayer* puser);
	virtual void user_logout_region(GamePlayer* puser);
	virtual void user_move_region(GamePlayer* puser);
	virtual void user_view_broadcast(GamePlayer* puser, BasicProtocol* msg);
	virtual void user_inout_tick();

protected:
	RegionCellNode* get_cellnode_byloc(const GLoc3D& loc);

	void notify_cellnode_inout(RegionCellNode* pnode);

protected:
	S_INT_32 xz_to_array(S_INT_32 x, S_INT_32 z) { return (x + z * max_column_cells_); }

private:
	//包括融合的地块
	CMSBox		max_region_;
	S_INT_32	max_column_cells_;
	S_INT_32	max_row_cells_;
	//我的地块
	CMSBox	my_region_;
	//左上，右下的column,row
	S_INT_32	lt_column_;
	S_INT_32	lt_row_;
	S_INT_32	rb_column_;
	S_INT_32	rb_row_;

	//box类型的节点
	boost::scoped_array<RegionCellNode>	cell_nodes_;

	S_INT_32				region_id_;
	RegionChannelService*	owner_;
};

#endif //__REGIONMAPBOXIMPL_H__
