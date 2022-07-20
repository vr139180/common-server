#ifndef __TASKMETAHOME_H__
#define __TASKMETAHOME_H__

#include <boost/unordered_map.hpp>
#include <taskLib/meta/TaskGroupMeta.h>

class TaskMetaHome
{
	typedef boost::unordered_map<int, TaskGroupMeta*>	TASKGROUP_MAP;
private:
	TaskMetaHome() {}

	void release();

public:
	static TaskMetaHome& instance();
	~TaskMetaHome();

	//加载任务配置信息
	bool load_taskmetas();

	TaskGroupMeta* get_taskgroup_by_iid(int gid);
	const std::vector<TaskGroupMeta*>& get_boost_groups() { return boost_groups_; }

private:
	std::vector<TaskGroupMeta*>	boost_groups_;
	TASKGROUP_MAP	main_tasks_;
	TASKGROUP_MAP	branch_tasks_;
	TASKGROUP_MAP	all_groups_;
};

#endif //__TASKMETAHOME_H__
