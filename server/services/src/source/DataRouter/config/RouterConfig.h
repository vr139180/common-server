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
	//�ֳ�һ��tickѭ������
	int loopnum_;
	//�����߳�����
	int service_thread_num_;
	//chathash����
	ChatHashPlot chathash_plot_;

	MailHashPlot mailhash_plot_;

	FriendHashPlot friendhash_plot_;

	//redis����
	config::RedisOption	redis_;
};

#endif //__ROUTERCONFIG_H__
