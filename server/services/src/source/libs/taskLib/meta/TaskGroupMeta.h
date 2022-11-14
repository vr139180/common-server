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

#ifndef __TASKGROUPMETA_H__
#define __TASKGROUPMETA_H__

#include <boost/unordered_map.hpp>

#include <cmsLib/util/XmlUtil.h>

#include <taskLib/meta/TaskGroupCellMeta.h>

class TaskGroupMeta
{
	typedef boost::unordered_map<int, TaskGroupCellMeta*> CELLS_MAP;
private:
	TaskGroupMeta();

	void release();

public:
	static TaskGroupMeta* load_taskgroup(const char* filename);
	~TaskGroupMeta();

public:
	int get_iid() { return group_iid_; }
	bool is_boost() { return is_boost_; }

	TaskGroupCellMeta* get_groupcell_byiid(int cid);
	TaskGroupCellMeta* get_start_cell() { return start_cell_; }
	TaskGroupCellMeta* get_end_cell() { return end_cell_; }

	bool is_end_cell(int celliid) { return celliid == end_celliid_; }

protected:
	bool load_from_xml(tinyxml2::XMLElement* e);

private:
	CELLS_MAP	cells_;

	TaskGroupCellMeta* start_cell_;
	TaskGroupCellMeta* end_cell_;

	//group info
	int	group_iid_;
	int start_celliid_;
	int end_celliid_;
	bool is_boost_;
	std::string version_;

	//cell iid seed
	int last_cell_iid_;
};

#endif //__TASKGROUPMETA_H__
