#ifndef __GATECONFIG_H__
#define __GATECONFIG_H__

#include <string>

class GateConfig
{
public:
	GateConfig(){}

public:
	//�ֳ�һ��tickѭ������
	int loopnum_;
	//�����߳�����
	int service_thread_num_;
};

#endif //__GATECONFIG_H__
