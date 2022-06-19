#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__

#include <string>

class GameConfig
{
public:
	GameConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//服务线程数量
	int service_thread_num_;
};

#endif //__GAMECONFIG_H__
