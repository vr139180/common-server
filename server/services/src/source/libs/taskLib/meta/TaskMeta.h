#ifndef __TASKMETA_H__
#define __TASKMETA_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>
#include <taskLib/task_const.h>
#include <taskLib/meta/ConditionsMeta.h>

class TaskGroupCellMeta;

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

private:
	int task_iid_;
	//�Ƿ�ѭ������
	bool is_cycle_task_;
	//�����ѭ������
	int  cycle_num_;
	//�Ƿ������������
	bool allow_giveup_;
	//����������
	std::vector<eTriggerType> trigger_events_;

	TaskGroupCellMeta*	parent_;
};

class TaskMetaCpp : public TaskMetaBase
{
	friend class TaskMetaBase;
private:
	TaskMetaCpp();

public:

	virtual std::string get_impl() { return "cpp"; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

private:
	std::string impl_name_;
};

class TaskMetaLua : public TaskMetaBase
{
	friend class TaskMetaBase;
private:
	TaskMetaLua();

public:

	virtual std::string get_impl() { return "lua"; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

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
	virtual std::string get_impl() { return "xml"; }
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

private:
	std::shared_ptr<IConditionsMeta>	get_cond_;
	std::shared_ptr<IConditionsMeta>	submit_cond_;
};

#endif //__TASKMETA_H__
