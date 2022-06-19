#ifndef __IEUREKACLIENTINTEGRATE_H__
#define __IEUREKACLIENTINTEGRATE_H__

#include <list>
#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/system/CommandBase.h>
#include <cmsLib/system/TimerContainer.h>

#include <gameLib/service_type.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

class IEurekaClientIntegrate
{
public:
	virtual ThreadLock& get_mth_threadlock() = 0;
	virtual void regist_eurekacommand(CommandBase *p) = 0;

	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) = 0;

	virtual void del_apptimer_proxy(TimerKey tid) = 0;

	//nodes中的指针不可以保存长期使用，不需要调用放主动释放
	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE, 
		std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids) = 0;

	//服务注册成功通知
	virtual void mth_service_registed(S_INT_64 sid) = 0;
	//和eureka所有的链接丢失，应用退出
	virtual void mth_eureka_losted() = 0;

	//succ == true pnode 如果 !=0 需要接收方负责释放
	virtual void on_mth_gatebindhome_ack( bool succ, ServiceNodeInfo* pnode, S_INT_64 gateid, S_INT_64 bindtoken){}
};

#endif //__IEUREKACLIENTINTEGRATE_H__
