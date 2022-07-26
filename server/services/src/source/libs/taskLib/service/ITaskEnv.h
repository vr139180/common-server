#ifndef __ITASKENV_H__
#define __ITASKENV_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/lua/ScriptContext.h>
#include <taskLib/task_const.h>
#include <taskLib/meta/ConditionsMeta.h>

class IUserDataEnv;

/************************************************************************/
/* 定义了外部环境的资源接口                                                                     */
/************************************************************************/
//全局数据环境
//定义的接口函数
class IGlobalDataEnv
{
public:
	//获取lua运行环境
	virtual ScriptContext* get_lua_context() = 0;

	//获取taskgroup task的唯一iid
	virtual S_INT_64 new_taskgroup_iid() = 0;
	virtual S_INT_64 new_task_iid() = 0;

	//任务系统lua环境准备
	virtual void task_luacontext_begin(IGlobalDataEnv* genv, IUserDataEnv* uenv) = 0;
	virtual void task_luacontext_end() = 0;
};

//用户数据环境
//需要用户模块实现该结构，如果支持lua环境，需要绑定对应的函数到lua
class IUserDataEnv
{
public:
	//task xml objective实现的计算入口
	//返回值为int64 bool: false == 0
	virtual bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret) = 0;

	//eTriggerType::TT_ROLEINFO_CHANGE
	virtual int get_role_level() = 0;

	//eTriggerType::TT_BAG_CHANGE
	virtual int get_bag_itemnum(S_INT_64 itemiid) = 0;

	//任务线是否结束
	virtual bool is_end_of_taskgroup(S_INT_32 gid) = 0;

public:
	//lua环境中的名字 u_roleiid
	virtual std::string get_luaojb_name() = 0;
};

class TaskGroupCellRT;
class TaskCellRT;

//任务数据变更回调
class ITaskDataUpdateCB
{
public:
	//触发新的任务线
	virtual void notify_new_taskgroup( TaskGroupCellRT* gcrt) = 0;
	//任务线进入下个节点
	virtual void notify_forward_nextcell_taskgroup(TaskGroupCellRT* gcrt) = 0;
	//任务线结束
	virtual void notify_taskgroup_end(TaskGroupCellRT* gcrt) = 0;

	//用户获取一个新任务
	virtual void notify_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc) = 0;
	//完成一个任务
	virtual void notify_end_task(TaskGroupCellRT* gcrt, TaskCellRT* tc) = 0;
};

//整合了 IGlobalDataEnv,IUserDataEnv
class ITaskContext
{
public:
	virtual IGlobalDataEnv* get_globalevn() = 0;
	virtual IUserDataEnv* get_userenv() = 0;

	bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret) {
		return get_userenv()->run_xmlobjective(fn, params, ret);
	}

	ScriptContext* get_lua_context() {
		return get_globalevn()->get_lua_context();
	}
};

#endif //__ITASKENV_H__
