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

#include <taskLib/meta/TaskMeta.h>

#include <cmsLib/util/ShareUtil.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/LogExt.h>

#include <taskLib/meta/TaskGroupCellMeta.h>

TaskMetaBase::TaskMetaBase()
{

}


TaskMetaBase::~TaskMetaBase()
{
	release();
}

void TaskMetaBase::release()
{

}

TaskMetaBase* TaskMetaBase::load_taskmeta(const char* file, TaskGroupCellMeta* p)
{
	std::string str = ConfigTool::get_instance().get_txtfilecontent( file, true);
	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* e = doc.RootElement();
	std::string impl = XmlUtil::GetXmlAttrStr(e, "impl", "");
	TaskMetaBase *ptask = 0;
	if (impl == "cpp")
		ptask = new TaskMetaCpp();
	else if (impl == "lua")
		ptask = new TaskMetaLua();
	else if (impl == "xml")
		ptask = new TaskMetaXml();

	if (ptask == 0)
		return 0;

	ptask->parent_ = p;
	std::unique_ptr<TaskMetaBase> ptr(ptask);

	if (!ptask->load_from_xml(e))
		return 0;

	return ptr.release();
}

bool TaskMetaBase::load_from_xml(tinyxml2::XMLElement* e)
{
	task_iid_ = XmlUtil::GetXmlAttrInt(e, "iid", -1);
	is_cycle_task_ = XmlUtil::GetXmlAttrBool(e, "iscycle", false);
	cycle_num_ = XmlUtil::GetXmlAttrInt(e, "cyclenum", 0);
	allow_giveup_ = XmlUtil::GetXmlAttrBool(e, "allowgiveup", true);

	tinyxml2::XMLElement* rewards = e->FirstChildElement("rewards");
	rewards_.reset( TaskRewardMeta::build_from_xml( rewards));

	tinyxml2::XMLElement* trg = e->FirstChildElement("trigger");
	if (trg == 0)
	{
		logError(out_runtime, "task system - taskgroup:%d task:%d not set trigger node", parent_->get_taskgroup_iid(), task_iid_);
		return false;
	}

	int evt = XmlUtil::GetXmlAttrInt(trg, "events", 0);
	for (int b = eTriggerType::TT_BEGIN + 1; b < eTriggerType::TT_MAX; ++b)
	{
		if (evt & b)
			trigger_events_.push_back((eTriggerType)b);
	}

	return true;
}

//--------------------------------------TaskMetaCpp---------------------------------
TaskMetaCpp::TaskMetaCpp():TaskMetaBase()
{
}

bool TaskMetaCpp::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!TaskMetaBase::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* cpp = e->FirstChildElement("cpp");
	if (cpp == 0)
		return false;

	impl_name_ = XmlUtil::GetXmlAttrStr(cpp, "name");
	ShareUtil::to_lower(impl_name_);

	tinyxml2::XMLElement* get = cpp->FirstChildElement("get");
	if (get != 0)
	{
		tinyxml2::XMLElement* params = get->FirstChildElement("params");
		get_params_.reset(CPPObjectiveParams::build_cppparams(params));
	}
	else {
		get_params_.reset( CPPObjectiveParams::build_empty());
	}

	tinyxml2::XMLElement* submit = cpp->FirstChildElement("submit");
	if (submit != 0)
	{
		tinyxml2::XMLElement* params = submit->FirstChildElement("params");
		submit_params_.reset(CPPObjectiveParams::build_cppparams(params));
	}
	else {
		submit_params_.reset(CPPObjectiveParams::build_empty());
	}

	return true;
}

//--------------------------------------TaskMetaLua---------------------------------
TaskMetaLua::TaskMetaLua(): TaskMetaBase()
{
}

bool TaskMetaLua::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!TaskMetaBase::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* lua = e->FirstChildElement("lua");
	if (lua == 0)
		return false;

	tinyxml2::XMLElement* g = lua->FirstChildElement("get");
	if (g != 0)
	{
		IConditionsMeta* pmeta = IConditionsMeta::build_from_xml(g, "lua");
		if (pmeta != 0)
			get_cond_.reset(pmeta);
	}

	tinyxml2::XMLElement* s = lua->FirstChildElement("submit");
	if (s != 0)
	{
		IConditionsMeta* pmeta = IConditionsMeta::build_from_xml(s, "lua");
		if (pmeta != 0)
			submit_cond_.reset(pmeta);
	}

	return true;
}

//--------------------------------------TaskMetaXml---------------------------------
TaskMetaXml::TaskMetaXml(): TaskMetaBase()
{

}

bool TaskMetaXml::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!TaskMetaBase::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* xml = e->FirstChildElement("xml");
	if (xml == 0)
		return false;

	tinyxml2::XMLElement* g = xml->FirstChildElement("get");
	if (g != 0)
	{
		IConditionsMeta* pmeta = IConditionsMeta::build_from_xml(g, "xml");
		if (pmeta != 0)
			get_cond_.reset(pmeta);
	}

	tinyxml2::XMLElement* s = xml->FirstChildElement("submit");
	if (s != 0)
	{
		IConditionsMeta* pmeta = IConditionsMeta::build_from_xml(s, "xml");
		if (pmeta != 0)
			submit_cond_.reset(pmeta);
	}

	return true;
}