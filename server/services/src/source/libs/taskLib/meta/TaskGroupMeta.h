#ifndef __TASKGROUPMETA_H__
#define __TASKGROUPMETA_H__

#include <boost/unordered_map.hpp>

#include <cmsLib/util/XmlUtil.h>

#include <taskLib/meta/TaskGroupCellMeta.h>

class TaskGroupMeta
{
	typedef boost::unordered_map<int, TaskGroupCellMeta*> CELLS_MAP;
private:
	TaskGroupMeta() {}

	void release();

public:
	static TaskGroupMeta* load_taskgroup(const char* filename);
	~TaskGroupMeta();

public:
	int get_iid() { return group_iid_; }

protected:
	bool load_from_xml(tinyxml2::XMLElement* e);

private:
	CELLS_MAP	cells_;

	//group info
	int	group_iid_;
	int start_celliid_;
	int end_celliid_;
	std::string version_;

	//cell iid seed
	int last_cell_iid_;
};

#endif //__TASKGROUPMETA_H__
