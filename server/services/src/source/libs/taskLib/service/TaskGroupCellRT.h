#ifndef __TASKGROUPCELLRT_H__
#define __TASKGROUPCELLRT_H__

#include <boost/unordered_map.hpp>
#include <gameLib/protobuf/Proto_all.h>
#include <taskLib/meta/TaskGroupMeta.h>
#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/TaskCellRT.h>

class TaskGroupCellRT
{
	typedef boost::unordered_map<S_INT_32, PRO::TASK_STATE>	CELL_DATA;
	typedef boost::unordered_map<S_INT_32, TaskCellRT*>		TASK_RT;
public:
	static TaskGroupCellRT* build_taskgroup_rt(const PRO::DBUserTaskGroup& dat, TaskGroupMeta* pGroup);
	//�����µ�������
	//δ����iid
	static TaskGroupCellRT* new_taskgroup_rt(S_INT_64 giid, TaskGroupMeta* pGroup);

	TaskGroupCellRT();
	~TaskGroupCellRT();
	S_INT_64 get_iid() { return giid_; }
	S_INT_32 get_groupiid() { return group_iid_; }
	S_INT_32 get_groupcell_iid() { return cell_iid_; }
	PRO::TASK_GROUP_STATE get_gstate() { return gstate_; }
	const CELL_DATA& get_celldata() { return datas_; }

	TaskCellRT* init_processing_task(const PRO::DBUserTaskItem& item);
	void init_wait_tasks(std::list<TaskCellRT*>& ts);

	TaskCellRT* get_task_byiid(S_INT_32 iid);

	//�ƽ���ǰgroup cell, return true:�����¸��ڵ㣬 false:ά�ֵ�ǰ�ڵ�
	bool forward_groupcell();
	//������һ��group cell��ֻ����һ��
	//true, waits: ���ؿɽӵ����� groups:���ش����������� start:��һ�γ�ʼ��
	bool goto_nextgroupcell(ITaskContext* tc, std::list<TaskCellRT*>& waits, std::vector<int>& groups, bool start = false);
	//�������Ƿ����
	bool is_groupend();

protected:
	void release();

	//����ת������
	bool check_conditions(ITaskContext* tc, TaskXmlCondition* pcond);

protected:
	S_INT_64				giid_;

	S_INT_32				group_iid_;
	TaskGroupMeta*			group_meta_;
	PRO::TASK_GROUP_STATE	gstate_;

	S_INT_32			cell_iid_;
	TaskGroupCellMeta*	group_cell_meta_;
	CELL_DATA			datas_;

	//����ʹ�õ������ļ��е�task_iid
	TASK_RT		processing_tasks_;
	TASK_RT		wait_tasks_;
	TASK_RT		all_tasks_;
};

#endif //__TASKGROUPCELLRT_H__
