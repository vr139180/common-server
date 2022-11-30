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

private:
	//box类型的节点
	RegionCellNode			cell_nodes_;

	RegionChannelService*	owner_;
};

#endif //__REGIONMAPBOXIMPL_H__
