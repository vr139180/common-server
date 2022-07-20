#ifndef __TASKGROUPCELLMETA_H__
#define __TASKGROUPCELLMETA_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <cmsLib/util/XmlUtil.h>
#include <taskLib/meta/TaskMeta.h>
#include <taskLib/meta/ConditionsMeta.h>

class TaskGroupMeta;

class TaskGroupCellMeta
{
	typedef boost::unordered_map<int, TaskMetaBase*>	TASKS_MAP;
private:
	TaskGroupCellMeta();
	void release();

public:
	static TaskGroupCellMeta* load_groupcell(tinyxml2::XMLElement* e, TaskGroupMeta* p);
	~TaskGroupCellMeta();

public:
	int get_iid() { return cell_iid_; }
	int get_taskgroup_iid();
	bool have_tasks() { return tasks_.size() > 0; }
	bool have_taskgroups() { return trigger_taskgroups_.size() > 0; }

	std::vector<int> get_trigger_taskgroups() { return trigger_taskgroups_; }
	const TASKS_MAP& get_tasks() { return tasks_; }

	TaskMetaBase* get_task_byiid(int iid);

	const boost::unordered_map<IConditionsMeta*, int>& get_next_conditions() { return next_conditions_; }
	int get_default_next() { return next_default_celliid_; }

private:
	int			cell_iid_;

	//�����Ĳ�������
	TASKS_MAP			tasks_;
	//������������
	std::vector<int>	trigger_taskgroups_;

	//��һ��ȱʡ�ڵ�
	int			next_default_celliid_;
	boost::unordered_map<IConditionsMeta*, int>	next_conditions_;

	TaskGroupMeta*	parent_;
};

#endif //__TASKGROUPCELLMETA_H__
