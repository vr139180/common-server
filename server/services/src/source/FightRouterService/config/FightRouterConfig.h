#ifndef __FIGHTROUTERCONFIG_H__
#define __FIGHTROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class FightRouterConfig
{
public:
	FightRouterConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//服务线程数量
	int service_thread_num_;

	//redis配置
	config::RedisOption	redis_;
};

#endif //__FIGHTROUTERCONFIG_H__
