#ifndef __TASKGROUPCELLMETA_H__
#define __TASKGROUPCELLMETA_H__

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

private:
	int			cell_iid_;

	TASKS_MAP	tasks_;
	//下一个缺省节点
	int					next_default_celliid_;
	boost::unordered_map<IConditionsMeta*, int>	next_conditions_;

	TaskGroupMeta*	parent_;
};

#endif //__TASKGROUPCELLMETA_H__
