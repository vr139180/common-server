#ifndef __ITASKENV_H__
#define __ITASKENV_H__

#include <cmsLib/prolib/core_type.h>
#include <taskLib/task_const.h>

/************************************************************************/
/* 定义了外部环境的资源接口                                                                     */
/************************************************************************/
//全局数据环境
//定义的接口函数
class IGlobalDataEnv
{
public:
};

//eTriggerType::TT_ROLEINFO_CHANGE
//xml定义中支持的函数类型
//xml只支持实时的get类型的条件判断
//对于如需要存储临时数据等稍复杂的用lua脚本实现
typedef int(*tsxml_check_count_p0)(void);
typedef int(*tsxml_check_count_p1)(S_INT_64 id);

//用户数据环境
//需要用户模块实现该结构，如果支持lua环境，需要绑定对应的函数到lua
class IUserDataEnv
{
public:
	//eTriggerType::TT_ROLEINFO_CHANGE
	virtual int get_role_level() = 0;

	//eTriggerType::TT_BAG_CHANGE
	virtual int get_bag_itemnum(S_INT_64 itemiid) = 0;

public:
	//lua环境中的名字 u_roleiid
	virtual std::string get_luaojb_name() = 0;

	
};

#endif //__ITASKENV_H__
