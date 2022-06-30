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

	bool load_taskmetas();

private:
	TASKGROUP_MAP	main_tasks_;
	TASKGROUP_MAP	branch_tasks_;
};

#endif //__TASKMETAHOME_H__
