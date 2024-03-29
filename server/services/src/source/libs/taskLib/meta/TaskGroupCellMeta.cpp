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

#include <taskLib/meta/TaskGroupCellMeta.h>

#include <gameLib/LogExt.h>
#include <taskLib/meta/TaskGroupMeta.h>

TaskGroupCellMeta::TaskGroupCellMeta():next_default_celliid_(-1)
{
}

TaskGroupCellMeta::~TaskGroupCellMeta()
{
	release();
}

int TaskGroupCellMeta::get_taskgroup_iid()
{
	return parent_->get_iid();
}

void TaskGroupCellMeta::release()
{
	trigger_taskgroups_.clear();

	for (TASKS_MAP::iterator iter = tasks_.begin(); iter != tasks_.end(); ++iter)
	{
		delete iter->second;
	}
	tasks_.clear();

	for (boost::unordered_map<IConditionsMeta*, int>::iterator iter = next_conditions_.begin(); 
		iter != next_conditions_.end(); ++iter)
	{
		delete iter->first;
	}
	next_conditions_.clear();
}

TaskGroupCellMeta* TaskGroupCellMeta::load_groupcell(tinyxml2::XMLElement* e, TaskGroupMeta* p)
{
	TaskGroupCellMeta* pCell = new TaskGroupCellMeta();
	pCell->parent_ = p;

	std::unique_ptr< TaskGroupCellMeta> ptr(pCell);

	pCell->cell_iid_ = XmlUtil::GetXmlAttrInt(e, "iid", -1);

	tinyxml2::XMLElement* tasks = e->FirstChildElement("tasks");
	if (tasks != 0)
	{
		for (tinyxml2::XMLElement* t = tasks->FirstChildElement("task"); t != 0; t = t->NextSiblingElement("task"))
		{
			int tiid = XmlUtil::GetXmlAttrInt(t, "iid", -1);
			if (tiid == -1)
				return 0;

			std::string path = XmlUtil::build_path("/tasks/%d/%d.task", p->get_iid(), tiid);
			TaskMetaBase* ptm = TaskMetaBase::load_taskmeta(path.c_str(), pCell);
			if (ptm == 0)
				return 0;

			pCell->tasks_[ptm->get_iid()] = ptm;
		}
	}

	tinyxml2::XMLElement* taskgroup = e->FirstChildElement("groups");
	if (taskgroup != 0)
	{
		for (tinyxml2::XMLElement* t = taskgroup->FirstChildElement("group"); t != 0; t = t->NextSiblingElement("group"))
		{
			int gid = XmlUtil::GetXmlAttrInt(t, "iid", -1);
			if (gid == -1)
				return 0;

			pCell->trigger_taskgroups_.push_back(gid);
		}
	}

	tinyxml2::XMLElement* next = e->FirstChildElement("next");
	if (next != 0)
	{
		pCell->next_default_celliid_ = XmlUtil::GetXmlAttrInt(next, "default", -1);
		if (pCell->next_default_celliid_ == -1)
		{
			logError(out_runtime, "task system - load taskgroup:%d cell:%d not set [next default celliid]", p->get_iid(), pCell->get_iid());
			return 0;
		}

		for (tinyxml2::XMLElement* nc = next->FirstChildElement("conditions"); nc != 0; nc = nc->NextSiblingElement("conditions"))
		{
			int cid = XmlUtil::GetXmlAttrInt(nc, "next", -1);
			if (cid == -1)
			{
				logError(out_runtime, "task system - load taskgroup:%d cell:%d set condition next celliid error", p->get_iid(), pCell->get_iid());
				return 0;
			}

			IConditionsMeta* pmeta = IConditionsMeta::build_from_xml(nc, "xml");
			if (pmeta == 0)
				return 0;

			pCell->next_conditions_[pmeta] = cid;
		}
	}

	return ptr.release();
}

TaskMetaBase* TaskGroupCellMeta::get_task_byiid(int iid)
{
	TASKS_MAP::iterator fiter = tasks_.find(iid);
	if (fiter == tasks_.end())
		return 0;
	return fiter->second;
}