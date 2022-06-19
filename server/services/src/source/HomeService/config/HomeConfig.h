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
	//�ֳ�һ��tickѭ������
	int loopnum_;
	//�����߳�����
	int service_thread_num_;
	//db�߳���
	int db_thread_nums_;

	//redis����
	config::RedisOption	redis_;
	//db����
	config::DatabaseOption db_;
};

#endif //__HOMECONFIG_H__
