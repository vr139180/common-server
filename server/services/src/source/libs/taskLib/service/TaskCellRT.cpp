#include <taskLib/service/TaskCellRT.h>

#include <taskLib/service/TaskGroupCellRT.h>
#include <taskLib/service/TaskResultLua.h>

#include <gameLib/LogExt.h>

TaskCellRT::TaskCellRT():task_meta_( 0)
, parent_(0), b_cycle_task_(false), cycle_num_(1)
{
}

TaskCellRT* TaskCellRT::build_taskrt(const PRO::DBUserTaskItem& item, TaskMetaBase* pmeta, TaskGroupCellRT* p)
{
	TaskCellRT *rt = new TaskCellRT();
	std::unique_ptr<TaskCellRT> ptr(rt);

	rt->iid_ = item.iid();

	rt->task_iid_ = item.task_iid();
	rt->task_meta_ = pmeta;
	if (pmeta->get_impl() == TASKIMPL_CPP)
	{
		rt->task_cpp_.reset(ITaskImplCpp::build_cppimpl_from_name(pmeta->get_impl().c_str()));
	}

	rt->qstate_ = (PRO::TASK_STATE)item.qstate();
	for (google::protobuf::Map<std::string, S_INT_64>::const_iterator iter = item.task_datas().datas().begin();
		iter != item.task_datas().datas().end(); ++iter)
	{
		rt->task_datas_[iter->first] = iter->second;
	}
	
	rt->b_cycle_task_ = item.cycle_task();
	rt->cycle_num_ = item.cycle_num();

	rt->parent_ = p;

	return ptr.release();
}

TaskCellRT* TaskCellRT::new_task_rt(TaskMetaBase* pmeta, TaskGroupCellRT* p)
{
	TaskCellRT *rt = new TaskCellRT();
	std::unique_ptr<TaskCellRT> ptr(rt);

	rt->task_iid_ = pmeta->get_iid();
	rt->task_meta_ = pmeta;
	if (pmeta->get_impl() == TASKIMPL_CPP)
	{
		rt->task_cpp_.reset(ITaskImplCpp::build_cppimpl_from_name(pmeta->get_impl().c_str()));
	}

	rt->qstate_ = PRO::TASKSTATE_WAIT;
	rt->b_cycle_task_ = pmeta->is_cycle_task();
	rt->cycle_num_ = pmeta->cycle_num();

	rt->parent_ = p;

	return ptr.release();
}

bool TaskCellRT::can_get_task(ITaskContext* tc)
{
	std::string impl = task_meta_->get_impl();
	if (impl == TASKIMPL_LUA)
	{
		TaskMetaLua* plua = dynamic_cast<TaskMetaLua*>(task_meta_);
		ConditionMetaLua* pCond = plua->get_getcondition();
		if (pCond == 0 || !pCond->need_check())
			return true;

		//genv_:LobbyService, mydata_:LobbyUser
		ScriptContext* luactx = tc->get_lua_context();
		tc->get_globalevn()->task_luacontext_begin(tc->get_globalevn(), tc->get_userenv());

		TaskResultLua luaret;
		luactx->regist_2_context<TaskResultLua>("cret_", &luaret);

		luactx->run_script(pCond->get_checkfun());

		luactx->remove_4_context( "cret_");
		tc->get_globalevn()->task_luacontext_end();

		logDebug(out_script, "task system- call task lua script result:%d", luaret.get_result());

		if (luaret.is_failed())
			return false;
		return luaret.get_result() == 0;
	}
	else if (impl == TASKIMPL_XML)
	{
		TaskMetaXml* pxml = dynamic_cast<TaskMetaXml*>(task_meta_);
		ConditionsMetaXml* pCond = pxml->get_getcondition();
		if (pCond == 0)
			return true;

		return check_xml_conditions(tc, pCond->get_condition());
	}
	else if( impl == TASKIMPL_CPP)
	{
		if (!is_task_cpp_valide())
			return false;

		return task_cpp_->can_gettask_check(tc);
	}

	return false;
}

bool TaskCellRT::check_xml_conditions(ITaskContext* tc, TaskXmlCondition* pcond)
{
	if (pcond->is_leaf())
	{
		const std::vector<XmlObjectvie*>& objs = pcond->get_objectives();
		if (objs.size() == 0)
			return true;

		std::vector<bool> allret;
		for (size_t ii = 0; ii < objs.size(); ++ii)
		{
			XmlObjectvie* xobj = objs[ii];
			S_INT_64 retval = 0;
			if (tc->run_xmlobjective(xobj->get_funname(), xobj->get_params(), retval) && xobj->check_result(retval))
			{
				allret.push_back(true);
			}
			else
			{
				allret.push_back(false);
			}
		}

		return pcond->check_results(allret);
	}
	else
	{
		const std::vector<TaskXmlCondition*>& cconds = pcond->get_childen_conditions();
		if (cconds.size() == 0)
			return false;

		std::vector<bool> allret;
		for (size_t ii = 0; ii < cconds.size(); ++ii)
		{
			bool v = this->check_xml_conditions(tc, cconds[ii]);
			allret.push_back(v);
		}

		return pcond->check_results(allret);
	}

	return false;
}

void TaskCellRT::user_gettask(S_INT_64 iid, ITaskContext* tc)
{
	this->iid_ = iid;
	this->qstate_ = PRO::TASKSTATE_ACCEPT;
	++this->cycle_num_;

	std::string impl = task_meta_->get_impl();
	if (impl == TASKIMPL_LUA)
	{
		TaskMetaLua* plua = dynamic_cast<TaskMetaLua*>(task_meta_);
		ConditionMetaLua* pCond = plua->get_getcondition();
		if (pCond != 0 && pCond->need_confirm())
		{
			ScriptContext* luactx = tc->get_lua_context();
			//genv_:LobbyService, mydata_:LobbyUser
			tc->get_globalevn()->task_luacontext_begin(tc->get_globalevn(), tc->get_userenv());

			TaskResultLua luaret;
			luactx->regist_2_context<TaskResultLua>("cret_", &luaret);

			luactx->run_script(pCond->get_confirmfun());

			luactx->remove_4_context("cret_");

			tc->get_globalevn()->task_luacontext_end();
		}
	}
	else if (impl == TASKIMPL_CPP)
	{
		if (!is_task_cpp_valide())
			return;

		task_cpp_->gettask_confirm(tc);
	}
}

S_INT_32 TaskCellRT::submit_task(ITaskContext* tc)
{
	std::string impl = task_meta_->get_impl();
	if (impl == TASKIMPL_LUA)
	{
		TaskMetaLua* plua = dynamic_cast<TaskMetaLua*>(task_meta_);
		ConditionMetaLua* pCond = plua->get_submitcondition();
		if (pCond == 0 || !pCond->need_check())
			return true;

		ScriptContext* luactx = tc->get_lua_context();
		//genv_:LobbyService, mydata_:LobbyUser
		tc->get_globalevn()->task_luacontext_begin(tc->get_globalevn(), tc->get_userenv());

		TaskResultLua luaret;
		luactx->regist_2_context<TaskResultLua>("cret_", &luaret);

		luactx->run_script(pCond->get_checkfun());

		luactx->remove_4_context("cret_");
		tc->get_globalevn()->task_luacontext_end();

		if (luaret.is_failed())
			return 2; //system error
		return luaret.get_result();
	}
	else if (impl == TASKIMPL_XML)
	{
		TaskMetaXml* pxml = dynamic_cast<TaskMetaXml*>(task_meta_);
		ConditionsMetaXml* pCond = pxml->get_submitcondition();
		if (pCond == 0)
			return 0;

		if (check_xml_conditions(tc, pCond->get_condition()))
			return 0;
		else
			return 3; //condition error
	}
	else if (impl == TASKIMPL_CPP)
	{
		if (!is_task_cpp_valide())
			return 2; //system error

		return task_cpp_->submit_task(tc);
	}

	return 0;
}
