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

#include "worldsLib/GameRegionMeta.h"

#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/util/ShareUtil.h>
#include <gameLib/LogExt.h>
#include <gameLib/config/ConfigTool.h>

#include "worldsLib/GameWorldHelper.h"

//--------------------------------------------RegionJoinMeta---------------------------------------------
RegionJoinMeta::RegionJoinMeta() :b_joined_(false),
to_regionid_(0),
combine_cell_num_(0)
{
}

bool RegionJoinMeta::load_from_xml( tinyxml2::XMLElement* e)
{
	this->b_joined_ = false;
	this->to_regionid_ = XmlUtil::GetXmlAttrInt(e, "toregion", 0);
	if (to_regionid_ <= 0)
		return true;

	this->b_joined_ = true;

	this->combine_cell_num_ = XmlUtil::GetXmlAttrInt(e, "combinecells", 0);
	if (combine_cell_num_ == 0)
		return false;

	return true;
}

//--------------------------------------------GameRegionMeta---------------------------------------------
GameRegionMeta& GameRegionMeta::instance()
{
	static GameRegionMeta s_instance;
	return s_instance;
}

GameRegionMeta::GameRegionMeta()
{
}

GameRegionMeta::~GameRegionMeta()
{
}

bool GameRegionMeta::load_region_config(S_INT_32 regionid)
{
	if (!GameWorldHelper::instance().init_gameworld())
	{
		logFatal(out_runtime, "load cmsworld/worldregion.xml failed");
		return false;
	}

	GameRegionSimpleMeta* psimple = GameWorldHelper::instance().find_region_byid(regionid);
	if (psimple == 0)
		return false;

	std::string fstr = ShareUtil::str_format<128>("/cmsworld/regions/region_%d.xml", regionid);
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return false;

	tinyxml2::XMLElement* root = doc.RootElement();
	this->regionid_ = XmlUtil::GetXmlAttrInt(root, "id", 0);
	if (regionid_ != regionid)
		return false;

	this->region_box_ = psimple->box();

	tinyxml2::XMLElement* joins = root->FirstChildElement("joins");
	if (joins)
	{
		for (tinyxml2::XMLElement* je = joins->FirstChildElement("join"); je != 0; je = je->NextSiblingElement("join"))
		{
			RegionJoinType jt = (RegionJoinType)XmlUtil::GetXmlAttrInt(je, "type", (int)RegionJoinType::RegionJT_NUM);
			if (jt == RegionJoinType::RegionJT_NUM)
				return false;
			region_join_[jt].load_from_xml(je);
		}
	}

	return true;
}

const RegionJoinMeta& GameRegionMeta::get_region_join(RegionJoinType jt)
{
	return region_join_[jt];
}
