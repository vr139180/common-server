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

#ifndef __GAMEWORLDHELPER_H__
#define __GAMEWORLDHELPER_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/tinyxml2/tinyxml2.h>
#include <worldsLib/geometry/common_types.h>

class GameRegionSimpleMeta
{
public:
	GameRegionSimpleMeta();
	static GameRegionSimpleMeta* build_from(tinyxml2::XMLElement* e);

	S_INT_32 region() { return regionid_; }

	const CMSBox& box() { return region_; }
private:
	//region id
	S_INT_32	regionid_;
	CMSBox		region_;
};

class GameWorldHelper
{
	typedef boost::unordered_map<S_INT_32, GameRegionSimpleMeta*> REGIONS_MAP;
private:
	GameWorldHelper();

public:
	static GameWorldHelper& instance();
	virtual ~GameWorldHelper();

	bool init_gameworld();

	GameRegionSimpleMeta* find_region_byid(S_INT_32 regionid);

protected:
	void release();

private:
	float total_width_;
	float total_height_;

	REGIONS_MAP	all_regions_;
};

#endif //__GAMEWORLDHELPER_H__