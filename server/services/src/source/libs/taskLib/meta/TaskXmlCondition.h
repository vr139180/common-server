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

	//��鷵��ֵ
	bool check_results(std::vector<bool>& rets);

private:
	//�Ƿ���Ҷ�ӽڵ�
	bool							is_leaf_;
	CondRelationType				node_relation_;
	std::vector<TaskXmlCondition*>	childen_conds_;
	//�ն˼�������
	std::vector<XmlObjectvie*>		objectives_;

	TaskXmlCondition*				parent_;
};

#endif //__TASKXMLCONDITION_H__
