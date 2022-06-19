#ifndef __HOMECONFIG_H__
#define __HOMECONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>
#include <gameLib/config/DatabaseOption.h>

class HomeConfig
{
public:
	HomeConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//服务线程数量
	int service_thread_num_;
	//db线程数
	int db_thread_nums_;

	//redis配置
	config::RedisOption	redis_;
	//db配置
	config::DatabaseOption db_;
};

#endif //__HOMECONFIG_H__
