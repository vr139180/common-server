#ifndef __GATECONFIG_H__
#define __GATECONFIG_H__

#include <string>

class GateConfig
{
public:
	GateConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//服务线程数量
	int service_thread_num_;
};

#endif //__GATECONFIG_H__
