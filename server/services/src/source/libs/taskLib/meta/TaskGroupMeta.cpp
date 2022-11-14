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

#include <taskLib/meta/TaskGroupMeta.h>

#include <gameLib/config/ConfigTool.h>
#include <gameLib/LogExt.h>

TaskGroupMeta::TaskGroupMeta():is_boost_(false)
, start_cell_(0), end_cell_(0)
{
}

TaskGroupMeta::~TaskGroupMeta()
{
	release();
}

void TaskGroupMeta::release()
{
	for (CELLS_MAP::iterator iter = cells_.begin(); iter != cells_.end(); ++iter)
	{
		delete iter->second;
	}
	cells_.clear();
}

TaskGroupMeta* TaskGroupMeta::load_taskgroup(const char* filename)
{
	TaskGroupMeta *pmeta = new TaskGroupMeta();
	std::unique_ptr<TaskGroupMeta> ptr( pmeta);

	std::string str = ConfigTool::get_instance().get_txtfilecontent( filename, true);
	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	if (!pmeta->load_from_xml(doc.RootElement()))
		return 0;

	return ptr.release();
}

bool TaskGroupMeta::load_from_xml( tinyxml2::XMLElement* e)
{
	this->group_iid_ = XmlUtil::GetXmlAttrInt(e, "iid", -1);
	start_celliid_ = XmlUtil::GetXmlAttrInt(e, "sc", -1);
	end_celliid_ = XmlUtil::GetXmlAttrInt(e, "ec", -1);
	last_cell_iid_ = XmlUtil::GetXmlAttrInt(e, "last_celliid", -1);
	is_boost_ = XmlUtil::GetXmlAttrBool(e, "boost", false);
	version_ = XmlUtil::GetXmlAttrStr(e, "version");

	tinyxml2::XMLElement* ce = e->FirstChildElement("cell");
	if (ce == 0)
		return false;

	for (; ce != 0; ce = ce->NextSiblingElement("cell"))
	{
		TaskGroupCellMeta* cell = TaskGroupCellMeta::load_groupcell(ce, this);
		if (cell == 0)
			return false;

		cells_[cell->get_iid()] = cell;
		if (cell->get_iid() == this->start_celliid_)
			this->start_cell_ = cell;
		else if (cell->get_iid() == this->end_celliid_)
			this->end_cell_ = cell;
	}

	if (start_cell_ == 0 || end_cell_ == 0)
	{
		logError(out_runtime, "task system- load task meta:%d failed, must set startcell and endcell", group_iid_);
		return false;
	}

	return true;
}

TaskGroupCellMeta* TaskGroupMeta::get_groupcell_byiid(int cid)
{
	CELLS_MAP::iterator fiter = cells_.find(cid);
	if (fiter == cells_.end())
		return 0;
	return fiter->second;
}
