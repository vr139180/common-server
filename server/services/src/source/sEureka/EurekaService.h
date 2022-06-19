#ifndef __EUREKASERVICE_H__
#define __EUREKASERVICE_H__

#include <boost/thread.hpp>

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/config/EurekaConfig.h>

class EurekaService
{
public:
	EurekaService();
	virtual ~EurekaService();

	virtual void init(EurekaConfig* conf);
	virtual void start();
	virtual void stop();

protected:
	void thread_worker();

protected:
	//�����߳�ʹ�õ�redis��һ���߳�һ��redis����
	RedisClient			redis_;

	volatile bool						will_quit_;
	boost::shared_ptr<boost::thread>	thread_;
	int									loop_num_;

};

#endif //__EUREKASERVICE_H__
