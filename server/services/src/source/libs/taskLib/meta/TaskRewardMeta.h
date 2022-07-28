#ifndef __TASKREWARDMETA_H__
#define __TASKREWARDMETA_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/cpp/task_system.pb.h>

class TaskRewardMeta
{
protected:
	TaskRewardMeta() {}

public:
	static TaskRewardMeta* build_from_xml(tinyxml2::XMLElement* e);

	class TaskRewardItem
	{
	public:
		TaskRewardItem() {}
		TaskRewardItem(PRO::TASKREWARD_TYPE t, S_INT_32 item, S_INT_32 num);
	public:
		PRO::TASKREWARD_TYPE type;
		S_INT_32	item_iid;
		S_INT_32	nums;
	};

public:
	bool have_rewards() { return items.size() > 0; }
	const std::vector<TaskRewardItem>& get_rewards() { return items; }

protected:
	std::vector<TaskRewardItem>	items;
};

#endif //__TASKREWARDMETA_H__
