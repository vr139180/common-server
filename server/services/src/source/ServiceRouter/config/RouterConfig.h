#ifndef __ROUTERCONFIG_H__
#define __ROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

typedef struct tagChatHashPlot
{
	int chatmax;
}ChatHashPlot;

typedef struct tagMailHashPlot
{
	int mailmax;
}MailHashPlot;

typedef struct tagFriendHashPlot
{
	int frdmax;
}FriendHashPlot;

class RouterConfig
{
public:
	RouterConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//服务线程数量
	int service_thread_num_;
	//chathash策略
	ChatHashPlot chathash_plot_;

	MailHashPlot mailhash_plot_;

	FriendHashPlot friendhash_plot_;

	//redis配置
	config::RedisOption	redis_;
};

#endif //__ROUTERCONFIG_H__
