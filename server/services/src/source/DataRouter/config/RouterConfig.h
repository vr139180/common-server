#ifndef __ROUTERCONFIG_H__
#define __ROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class RouterConfig
{
public:
	RouterConfig(){}

public:
	//�ֳ�һ��tickѭ������
	int loopnum_;

	//redis����
	config::RedisOption	redis_;
};

#endif //__ROUTERCONFIG_H__
