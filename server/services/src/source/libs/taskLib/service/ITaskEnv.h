#ifndef __ITASKENV_H__
#define __ITASKENV_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/lua/ScriptContext.h>
#include <taskLib/task_const.h>
#include <taskLib/meta/ConditionsMeta.h>

class IUserDataEnv;

/************************************************************************/
/* �������ⲿ��������Դ�ӿ�                                                                     */
/************************************************************************/
//ȫ�����ݻ���
//����Ľӿں���
class IGlobalDataEnv
{
public:
	//��ȡlua���л���
	virtual ScriptContext* get_lua_context() = 0;

	//��ȡtaskgroup task��Ψһiid
	virtual S_INT_64 new_taskgroup_iid() = 0;
	virtual S_INT_64 new_task_iid() = 0;

	//����ϵͳlua����׼��
	virtual void task_luacontext_begin(IGlobalDataEnv* genv, IUserDataEnv* uenv) = 0;
	virtual void task_luacontext_end() = 0;
};

//�û����ݻ���
//��Ҫ�û�ģ��ʵ�ָýṹ�����֧��lua��������Ҫ�󶨶�Ӧ�ĺ�����lua
class IUserDataEnv
{
public:
	//task xml objectiveʵ�ֵļ������
	//����ֵΪint64 bool: false == 0
	virtual bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret) = 0;

	//eTriggerType::TT_ROLEINFO_CHANGE
	virtual int get_role_level() = 0;

	//eTriggerType::TT_BAG_CHANGE
	virtual int get_bag_itemnum(S_INT_64 itemiid) = 0;

	//�������Ƿ����
	virtual bool is_end_of_taskgroup(S_INT_32 gid) = 0;

public:
	//lua�����е����� u_roleiid
	virtual std::string get_luaojb_name() = 0;
};

class TaskGroupCellRT;
class TaskCellRT;

//�������ݱ���ص�
class ITaskDataUpdateCB
{
public:
	//�����µ�������
	virtual void notify_new_taskgroup( TaskGroupCellRT* gcrt) = 0;
	//�����߽����¸��ڵ�
	virtual void notify_forward_nextcell_taskgroup(TaskGroupCellRT* gcrt) = 0;
	//�����߽���
	virtual void notify_taskgroup_end(TaskGroupCellRT* gcrt) = 0;

	//�û���ȡһ��������
	virtual void notify_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc) = 0;
	//���һ������
	virtual void notify_end_task(TaskGroupCellRT* gcrt, TaskCellRT* tc) = 0;
};

//������ IGlobalDataEnv,IUserDataEnv
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
