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

#include "worldsLib/GameWorldHelper.h"
#include <gameLib/config/ConfigTool.h>
#include <cmsLib/util/XmlUtil.h>

//--------------------------------------GameRegionSimpleMeta-----------------------
GameRegionSimpleMeta::GameRegionSimpleMeta():regionid_(0)
{
}

GameRegionSimpleMeta* GameRegionSimpleMeta::build_from(tinyxml2::XMLElement* e)
{
	GameRegionSimpleMeta* ptr = new GameRegionSimpleMeta();
	std::unique_ptr<GameRegionSimpleMeta> xptr(ptr);

	ptr->regionid_ = XmlUtil::GetXmlAttrInt(e, "id", 0);
	if (ptr->regionid_ == 0)
		return 0;

	std::string regstr = XmlUtil::GetXmlAttrStr(e, "start");
	CMSPointXY startpos;
	if (!CMSPointXY::build_from_str(regstr, startpos))
		return 0;
	float w = XmlUtil::GetXmlAttrFloat(e, "width");
	float h = XmlUtil::GetXmlAttrFloat(e, "height");
	if (w == 0 || h == 0)
		return 0;

	ptr->region_.init_box(startpos, w, h);

	return xptr.release();
}

bool GameRegionSimpleMeta::is_point_in_region(const GLoc3D& loc)
{
	CMSPointXY xy(loc.x(), loc.y());
	return region_.is_point_in_box(xy);
}

//--------------------------------------GameWorldHelper----------------------------
GameWorldHelper& GameWorldHelper::instance()
{
	static GameWorldHelper s_instance;
	return s_instance;
}

GameWorldHelper::GameWorldHelper():
	total_width_(0.0),
	total_height_(0.0)
{
}

GameWorldHelper::~GameWorldHelper()
{
	release();
}

void GameWorldHelper::release()
{
	for (REGIONS_MAP::iterator iter = all_regions_.begin(); iter != all_regions_.end(); ++iter)
		delete iter->second;
	all_regions_.clear();
}

bool GameWorldHelper::init_gameworld()
{
	std::string fstr = "/cmsworld/worldregion.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return false;

	tinyxml2::XMLElement* root = doc.RootElement();
	total_width_ = XmlUtil::GetXmlAttrFloat(root, "width");
	total_height_ = XmlUtil::GetXmlAttrFloat(root, "height");

	tinyxml2::XMLElement* starts = root->FirstChildElement("startpoints");
	if (starts == 0)
		return false;

	for (tinyxml2::XMLElement* re = starts->FirstChildElement("startpoint"); re != 0; re = re->NextSiblingElement("startpoint"))
	{
		GLoc3D loc;
		if (!GLoc3D::build_from_str(XmlUtil::GetXmlAttrStr(re, "pos", ""), loc))
			return false;

		start_points_.push_back(loc);
	}

	for (tinyxml2::XMLElement* re = root->FirstChildElement("region"); re != 0; re = re->NextSiblingElement("region"))
	{
		GameRegionSimpleMeta* pmeta = GameRegionSimpleMeta::build_from(re);
		if (pmeta == 0)
			return false;

		all_regions_[pmeta->region()] = pmeta;
	}

	return true;
}

const GLoc3D& GameWorldHelper::get_startpoint()
{
	return *(start_points_.begin());
}

GameRegionSimpleMeta* GameWorldHelper::find_region_byid(S_INT_32 regionid)
{
	REGIONS_MAP::iterator fiter = all_regions_.find(regionid);
	if (fiter == all_regions_.end())
		return 0;
	return fiter->second;
}

void GameWorldHelper::get_hello_world(GLoc3D& loc, S_INT_32 regionid)
{
	loc = get_startpoint();
	get_regionid_from_loc(loc, regionid);
}

bool GameWorldHelper::get_regionid_from_loc(const GLoc3D& loc, S_INT_32& regionid)
{
	if (loc.x() < 0 || loc.x() >= total_width_)
		return false;
	if (loc.y() < 0 || loc.y() >= total_height_)
		return false;

	//从点获取区域
	//TODO: 未来用RTree数优化查找
	for (REGIONS_MAP::iterator iter = all_regions_.begin(); iter != all_regions_.end(); ++iter)
	{
		GameRegionSimpleMeta* pr = iter->second;
		if (pr->is_point_in_region(loc))
		{
			regionid = pr->region();
			return true;
		}
	}

	return false;
}