#ifndef __ROUTERCONFIG_H__
#define __ROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class RouterConfig
{
public:
	RouterConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;

	//redis配置
	config::RedisOption	redis_;
};

#endif //__ROUTERCONFIG_H__
