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

#ifndef __TASKXMLCONDITION_H__
#define __TASKXMLCONDITION_H__

#include <taskLib/meta/TaskXmlObjective.h>

typedef enum tag_CondRelationType {
	CondRelationType_And =1,
	CondRelationType_Or,
	CondRelationType_Not,
}CondRelationType;

class TaskXmlCondition
{
private:
	TaskXmlCondition();

	void release();

public:
	~TaskXmlCondition();
	static TaskXmlCondition* build_taskcondition(tinyxml2::XMLElement* e, TaskXmlCondition* p);

	bool is_leaf() { return is_leaf_; }
	bool have_childen_conditions() { return childen_conds_.size() > 0; }
	bool have_objectives() { return objectives_.size() > 0; }

	const std::vector<TaskXmlCondition*>& get_childen_conditions() { return childen_conds_; }
	const std::vector<XmlObjectvie*>& get_objectives() { return objectives_; }

	//检查返回值
	bool check_results(std::vector<bool>& rets);

private:
	//是否是叶子节点
	bool							is_leaf_;
	CondRelationType				node_relation_;
	std::vector<TaskXmlCondition*>	childen_conds_;
	//终端计算条件
	std::vector<XmlObjectvie*>		objectives_;

	TaskXmlCondition*				parent_;
};

#endif //__TASKXMLCONDITION_H__
