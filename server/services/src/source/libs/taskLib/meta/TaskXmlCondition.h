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
