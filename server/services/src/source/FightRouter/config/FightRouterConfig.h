#ifndef __FIGHTROUTERCONFIG_H__
#define __FIGHTROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class FightRouterConfig
{
public:
	FightRouterConfig(){}

public:
	//�ֳ�һ��tickѭ������
	int loopnum_;
	//�����߳�����
	int service_thread_num_;

	//redis����
	config::RedisOption	redis_;
};

#endif //__FIGHTROUTERCONFIG_H__
