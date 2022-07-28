#ifndef __TASKMETA_H__
#define __TASKMETA_H__

#include <vector>
#include <boost/unordered_map.hpp>

#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include <taskLib/task_const.h>
#include <taskLib/meta/ConditionsMeta.h>
#include <taskLib/meta/TaskCppObjective.h>
#include <taskLib/meta/TaskRewardMeta.h>

class TaskGroupCellMeta;

#define TASKIMPL_LUA "lua"
#define TASKIMPL_XML "xml"
#define TASKIMPL_CPP "cpp"

class TaskMetaBase
{
protected:
	TaskMetaBase();
	virtual void release();

public:
	static TaskMetaBase* load_taskmeta(const char* file, TaskGroupCellMeta* p);
	~TaskMetaBase();

	virtual std::string get_impl() = 0;
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

public:
	int get_iid() { return task_iid_; }

	bool is_cycle_task() { return is_cycle_task_; }
	int cycle_num() { return cycle_num_; }
	bool is_allow_giveup() { return allow_giveup_; }

	TaskRewardMeta* get_rewards() { return rewards_.get(); }

private:
	int task_iid_;
	//是否循环任务
	bool is_cycle_task_;
	//允许的循环次数
	int  cycle_num_;
	//是否允许放弃任务
	bool allow_giveup_;
	//触发器类型
	std::vector<eTriggerType> trigger_events_;

	//奖励
	std::shared_ptr<TaskRewardMeta>	rewards_;

	TaskGroupCellMeta*	parent_;
};

class TaskMetaCpp : public TaskMetaBase
{
	friend class TaskMetaBase;
private:
	TaskMetaCpp();

public:

	virtual std::string get_impl() { return TASKIMPL_CPP; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	const char* get_implcpp() { return impl_name_.c_str(); }
	CPPObjectiveParams* get_getparams() { return get_params_.get(); }
	CPPObjectiveParams* get_submitparams() { return submit_params_.get(); }

private:
	std::string impl_name_;

	std::shared_ptr<CPPObjectiveParams>	get_params_;
	std::shared_ptr<CPPObjectiveParams> submit_params_;
};

class TaskMetaLua : public TaskMetaBase
{
	friend class TaskMetaBase;
private:
	TaskMetaLua();

public:

	virtual std::string get_impl() { return TASKIMPL_LUA; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	ConditionMetaLua* get_getcondition() {
		if (get_cond_->is_luacondition())
			return dynamic_cast<ConditionMetaLua*>(get_cond_.get());
		return 0;
	}
	ConditionMetaLua* get_submitcondition() {
		if (get_cond_->is_luacondition())
			return dynamic_cast<ConditionMetaLua*>(submit_cond_.get());
		return 0;
	}

private:
	std::shared_ptr<IConditionsMeta>	get_cond_;
	std::shared_ptr<IConditionsMeta>	submit_cond_;
};

class TaskMetaXml : public TaskMetaBase
{
	friend class TaskMetaBase;
private:
	TaskMetaXml();

public:
	virtual std::string get_impl() { return TASKIMPL_XML; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	ConditionsMetaXml* get_getcondition() {
		if (get_cond_->is_xmlcondition())
			return dynamic_cast<ConditionsMetaXml*>(get_cond_.get());
		return 0;
	}

	ConditionsMetaXml* get_submitcondition() {
		if (get_cond_->is_xmlcondition())
			return dynamic_cast<ConditionsMetaXml*>(submit_cond_.get());
		return 0;
	}

private:
	std::shared_ptr<IConditionsMeta>	get_cond_;
	std::shared_ptr<IConditionsMeta>	submit_cond_;
};

#endif //__TASKMETA_H__
