#include <taskLib/TaskMetaHome.h>

#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/LogExt.h>

TaskMetaHome& TaskMetaHome::instance()
{
	static TaskMetaHome s_instance;
	return s_instance;
}

TaskMetaHome::~TaskMetaHome()
{
	release();
}

void TaskMetaHome::release()
{
	boost_groups_.clear();
	main_tasks_.clear();
	branch_tasks_.clear();

	for (TASKGROUP_MAP::iterator iter = all_groups_.begin(); iter != all_groups_.end(); ++iter)
	{
		delete iter->second;
	}
	all_groups_.clear();
}

bool TaskMetaHome::load_taskmetas()
{
	{
		std::list<std::string> mail_tasks;
		ConfigTool::get_instance().get_filelist("/tasks", ".main", mail_tasks);

		for (std::list<std::string>::iterator iter = mail_tasks.begin(); iter != mail_tasks.end(); ++iter)
		{
			std::string f = "/tasks/" + (*iter);
			TaskGroupMeta* pmeta = TaskGroupMeta::load_taskgroup(f.c_str());
			if (pmeta == 0)
			{
				logError(out_sys, "task system- load main task group:%s failed", f.c_str());
				continue;
			}

			main_tasks_[pmeta->get_iid()] = pmeta;
			all_groups_[pmeta->get_iid()] = pmeta;

			if (pmeta->is_boost())
				boost_groups_.push_back(pmeta);
		}
	}

	{
		std::list<std::string> branch_tasks;
		ConfigTool::get_instance().get_filelist("/tasks", ".branch", branch_tasks);

		for (std::list<std::string>::iterator iter = branch_tasks.begin(); iter != branch_tasks.end(); ++iter)
		{
			std::string f = "/tasks/" + (*iter);
			TaskGroupMeta* pmeta = TaskGroupMeta::load_taskgroup(f.c_str());
			if (pmeta == 0)
			{
				logError(out_sys, "task system- load branch task group:%s failed", f.c_str());
				continue;
			}

			branch_tasks_[pmeta->get_iid()] = pmeta;
			all_groups_[pmeta->get_iid()] = pmeta;
		}
	}

	return true;
}

TaskGroupMeta* TaskMetaHome::get_taskgroup_by_iid(int gid)
{
	TASKGROUP_MAP::iterator fiter = all_groups_.find(gid);
	if (fiter == all_groups_.end())
		return 0;

	return fiter->second;
}
