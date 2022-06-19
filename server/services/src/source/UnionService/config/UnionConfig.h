#ifndef __UNIONCONFIG_H__
#define __UNIONCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class UnionConfig
{
public:
	UnionConfig(){}

public:
	//�ֳ�һ��tickѭ������
	int loopnum_;
	//�����߳�����
	int service_thread_num_;

	//redis����
	config::RedisOption	redis_;
};

#endif //__UNIONCONFIG_H__
