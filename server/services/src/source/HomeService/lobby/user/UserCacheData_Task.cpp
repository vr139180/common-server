#include "lobby/user/UserCacheData.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>

#include <taskLib/service/TaskCellRT.h>

#include "dbs/DBSCtrl.h"
#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

//-----------------------------------------------------------------------------------------
void UserCacheData::notify_new_taskgroup(TaskGroupCellRT* gcrt)
{
	this->task_new_group(gcrt);
}

void UserCacheData::notify_forward_nextcell_taskgroup(TaskGroupCellRT* gcrt)
{
	this->task_group_nextcell(gcrt);
}

void UserCacheData::notify_taskgroup_end(TaskGroupCellRT* gcrt)
{
	this->task_group_end(gcrt);
}

void UserCacheData::notify_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc)
{
	this->task_user_gettask(gcrt, tc);
}

void UserCacheData::notify_end_task(TaskGroupCellRT* gcrt, TaskCellRT* tc)
{
	this->task_user_submittask(gcrt, tc);
}

void UserCacheData::task_new_group(TaskGroupCellRT* gcrt)
{
	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);

	PRO::DBUserTaskGroup* pGroup = task_data_.new_taskgroup();
	pGroup->set_ver_(newver);

	pGroup->set_iid(gcrt->get_iid());

	pGroup->set_role_iid(this->role_iid_);
	pGroup->set_task_group(gcrt->get_groupiid());
	pGroup->set_group_cell(gcrt->get_groupcell_iid());
	pGroup->set_gstate( (S_INT_32)gcrt->get_gstate());

	//TODO:level
	pGroup->set_trigg_level(0);
	pGroup->set_createtime(OSSystem::mOS->GetUnixTimestamp());
	pGroup->set_endtime(0);

	//cell_data
	google::protobuf::Map<S_INT_32, S_INT_32>& pData = *(pGroup->mutable_cell_data()->mutable_datas());
	const boost::unordered_map<S_INT_32, PRO::TASK_STATE>& celldata = gcrt->get_celldata();
	for (boost::unordered_map<S_INT_32, PRO::TASK_STATE>::const_iterator citer = celldata.begin(); citer != celldata.end(); ++citer)
	{
		pData[citer->first] = (S_INT_32)citer->second;
	}

	this->save_all(false, rdv);
}

void UserCacheData::task_group_nextcell(TaskGroupCellRT* gcrt)
{
	PRO::DBUserTaskGroup* pGroup = task_data_.get_taskgroup_for_update( gcrt->get_groupiid());
	if (pGroup == 0)
		return;

	RedisClient* rdv = svrApp.get_redisclient();
	S_UINT_32 newver = this->inc_cachever(rdv);

	pGroup->set_ver_(newver);
	pGroup->set_group_cell(gcrt->get_groupcell_iid());
	pGroup->set_gstate((S_INT_32)gcrt->get_gstate());

	pGroup->clear_cell_data();
	google::protobuf::Map<S_INT_32, S_INT_32>& pData = *(pGroup->mutable_cell_data()->mutable_datas());
	const boost::unordered_map<S_INT_32, PRO::TASK_STATE>& celldata = gcrt->get_celldata();
	for (boost::unordered_map<S_INT_32, PRO::TASK_STATE>::const_iterator citer = celldata.begin(); citer != celldata.end(); ++citer)
	{
		pData[citer->first] = (S_INT_32)citer->second;
	}

	this->save_all(false, rdv);
}

void UserCacheData::task_group_end(TaskGroupCellRT* gcrt)
{
	PRO::DBUserTaskGroupEnd* pGroup = task_data_.end_taskgroup(gcrt->get_groupiid(), gcrt->get_gstate());
	if (pGroup == 0)
		return;

	RedisClient* rdv = svrApp.get_redisclient();
	S_UINT_32 newver = this->inc_cachever(rdv);

	pGroup->set_ver_(newver);
}

void UserCacheData::task_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc)
{
	RedisClient* rdv = svrApp.get_redisclient();
	S_UINT_32 newver = this->inc_cachever(rdv);

	PRO::DBUserTaskItem* ptask = task_data_.new_taskitem();
	ptask->set_ver_(newver);

	ptask->set_iid(tc->get_iid());
	ptask->set_role_iid(this->role_iid_);
	ptask->set_task_iid(tc->get_taskiid());
	ptask->set_my_taskgroup(gcrt->get_iid());
	ptask->set_task_group(gcrt->get_groupiid());
	ptask->set_qstate( (S_INT_32)tc->get_qstate());
	//TODO:
	ptask->set_accept_level(1);
	ptask->set_cycle_task(tc->is_cycle_task());
	ptask->set_cycle_num(tc->get_cycle_num());
	ptask->set_createtime(OSSystem::mOS->GetUnixTimestamp());
	ptask->set_lastupdatetime(ptask->createtime());
	ptask->set_source_iid("");

	//cell_data
	google::protobuf::Map<std::string, S_INT_64>& pData1 = *(ptask->mutable_task_datas()->mutable_datas());
	const boost::unordered_map<std::string, S_INT_64>& tdata = tc->get_taskdatas();
	for (boost::unordered_map<std::string, S_INT_64>::const_iterator citer = tdata.begin(); citer != tdata.end(); ++citer)
	{
		pData1[citer->first] = citer->second;
	}

	//update taskgroup
	PRO::DBUserTaskGroup* pGroup = task_data_.get_taskgroup_for_update(gcrt->get_groupiid());
	if (pGroup != 0)
	{
		pGroup->set_ver_(newver);

		pGroup->clear_cell_data();
		google::protobuf::Map<S_INT_32, S_INT_32>& pData2 = *(pGroup->mutable_cell_data()->mutable_datas());
		const boost::unordered_map<S_INT_32, PRO::TASK_STATE>& celldata = gcrt->get_celldata();
		for (boost::unordered_map<S_INT_32, PRO::TASK_STATE>::const_iterator citer = celldata.begin(); citer != celldata.end(); ++citer)
		{
			pData2[citer->first] = (S_INT_32)citer->second;
		}
	}

	this->save_all(false, rdv);
}

void UserCacheData::task_user_submittask(TaskGroupCellRT* gcrt, TaskCellRT* tc)
{
	RedisClient* rdv = svrApp.get_redisclient();
	S_UINT_32 newver = this->inc_cachever(rdv);

	PRO::DBUserTaskGroup* pGroup = task_data_.get_taskgroup_for_update(gcrt->get_groupiid());
	if (pGroup)
	{
		pGroup->set_ver_(newver);

		pGroup->clear_cell_data();
		google::protobuf::Map<S_INT_32, S_INT_32>& pData2 = *(pGroup->mutable_cell_data()->mutable_datas());
		const boost::unordered_map<S_INT_32, PRO::TASK_STATE>& celldata = gcrt->get_celldata();
		for (boost::unordered_map<S_INT_32, PRO::TASK_STATE>::const_iterator citer = celldata.begin(); citer != celldata.end(); ++citer)
		{
			pData2[citer->first] = (S_INT_32)citer->second;
		}
	}

	PRO::DBUserTaskEndItem* pitem = task_data_.end_taskitem(tc->get_taskiid(), tc->get_qstate());
	if (pitem)
	{
		pGroup->set_ver_(newver);
	}

	this->save_all(false, rdv);
}

BasicProtocol* UserCacheData::task_get_waitlist()
{
	PRO::Task_WaitList_ack *ack = new PRO::Task_WaitList_ack();
	std::unique_ptr<PRO::Task_WaitList_ack> ptr(ack);

	const boost::unordered_map<S_INT_32, TaskCellRT*>& ws = task_resolver_.get_waittask_list();
	for (boost::unordered_map<S_INT_32, TaskCellRT*>::const_iterator citer = ws.begin(); citer != ws.end(); ++citer)
	{
		ack->add_task_iids(citer->first);
	}

	return ptr.release();
}

BasicProtocol* UserCacheData::task_get_mytasks()
{
	PRO::Task_MyTaskList_ack *ack = new PRO::Task_MyTaskList_ack();
	std::unique_ptr<PRO::Task_MyTaskList_ack> ptr(ack);

	ack->mutable_groups()->CopyFrom(task_data_.get_taskgroups());
	ack->mutable_tasks()->CopyFrom(task_data_.get_tasks());

	return ptr.release();
}

BasicProtocol* UserCacheData::task_get_from_waitlist(S_INT_32 taskid)
{
	PRO::Task_GetTask_ack *ack = new PRO::Task_GetTask_ack();
	std::unique_ptr<PRO::Task_GetTask_ack> ptr(ack);

	ack->set_task_iid(taskid);

	if (task_resolver_.get_task_from_waitlist(taskid))
	{
		PRO::DBUserTaskItem* pitem = task_data_.get_taskitem(taskid);
		if (pitem == 0)
			ack->set_result(1);
		else
		{
			ack->set_result(0);
			ack->mutable_task()->CopyFrom(*pitem);
		}
	}
	else
	{
		ack->set_result(1);
	}

	return ptr.release();
}

BasicProtocol* UserCacheData::task_submit_one(S_INT_32 taskid)
{
	PRO::Task_SubmitTask_ack *ack = new PRO::Task_SubmitTask_ack();
	std::unique_ptr<PRO::Task_SubmitTask_ack> ptr(ack);

	ack->set_task_iid(taskid);
	S_INT_32 ret = task_resolver_.submit_task(taskid);
	ack->set_result(ret);

	return ptr.release();
}

BasicProtocol* UserCacheData::task_giveup_task(S_INT_32 taskid)
{
	PRO::Task_GiveupTask_ack *ack = new PRO::Task_GiveupTask_ack();
	std::unique_ptr<PRO::Task_GiveupTask_ack> ptr(ack);

	ack->set_task_iid(taskid);
	S_INT_32 ret = task_resolver_.giveup_task(taskid);
	ack->set_result(ret);

	return ptr.release();
}
