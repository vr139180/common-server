#ifndef __ITASKRESTRIGGER_H__
#define __ITASKRESTRIGGER_H__

#include <taskLib/task_const.h>

//外部系统定义资源触发
class ITaskResTrigger
{
public:
	virtual void on_roleinfo_change() = 0;
	virtual void on_bag_change() = 0;
	virtual void on_building_change() = 0;
};

#endif //__ITASKRESTRIGGER_H__
