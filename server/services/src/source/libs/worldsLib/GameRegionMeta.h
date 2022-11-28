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

#ifndef __GAMEREGIONMETA_H__
#define __GAMEREGIONMETA_H__

#include <cmsLib/core_type.h>
#include <cmsLib/tinyxml2/tinyxml2.h>
#include <worldsLib/cmsworld_const.h>
#include <worldsLib/geometry/common_types.h>

typedef enum tagRegionJoinType {
	RegionJT_Left = 0,
	RegionJT_LeftTop,
	RegionJT_Top,
	RegionJT_TopRight,
	RegionJT_Right,
	RegionJT_RightButtom,
	RegionJT_Buttom,
	RegionJT_ButtomLeft,
	RegionJT_NUM,
}RegionJoinType;

//连接处配置
class RegionJoinMeta
{
public:
	RegionJoinMeta();

	bool load_from_xml( tinyxml2::XMLElement* e);
	bool can_join() { return b_joined_; }
	S_INT_32 combine_cells() { return combine_cell_num_; }

private:
	//是否连通
	bool b_joined_;

	//b_joined == true, 一下配置有效
	S_INT_32	to_regionid_;
	//连接处合并的cell数量
	S_INT_32	combine_cell_num_;
};

class GameRegionMeta
{
private:
	GameRegionMeta();

public:
	static GameRegionMeta& instance();

	virtual ~GameRegionMeta();

	bool load_region_config( S_INT_32 regionid);

private:
	S_INT_32	regionid_;

	CMSBox		region_box_;

	//连接点配置
	RegionJoinMeta	region_join_[RegionJT_NUM];
};

#endif //__GAMEREGIONMETA_H__