#ifndef __EUREKANODEINFO_H__
#define __EUREKANODEINFO_H__

#include <string>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/redis/RedisClient.h>

class EurekaNodeInfo : public IRedisSerializer
{
public:
	EurekaNodeInfo();

	EurekaNodeInfo* clone();

	EurekaNodeInfo& operator = (const EurekaNodeInfo& v);

	bool is_same_node(S_INT_64 token) { return this->token == token; }

protected:
	//------------------implementation IRedisSerializer---------------------
	virtual bool to_json(std::string& val);
	virtual bool from_json(boost::json::value& root);

public:
	//服务id
	S_INT_64	iid;
	//node的ip地址
	std::string ip;
	//node的端口
	int port;
	//node的token
	S_INT_64	token;
};

#endif //__EUREKANODEINFO_H__
