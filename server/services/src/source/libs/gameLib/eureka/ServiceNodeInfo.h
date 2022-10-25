#ifndef __SERVICENODEINFO_H__
#define __SERVICENODEINFO_H__

#include <string>
#include <boost/unordered_map.hpp>

#include <cmsLib/core_type.h>
#include <cmsLib/redis/RedisClient.h>
#include <gameLib/service_type.h>

class ServiceNodeInfo : public IRedisSerializer
{
public:
	ServiceNodeInfo();

	ServiceNodeInfo* clone();

	ServiceNodeInfo& operator = (const ServiceNodeInfo& v);

	std::string get_extparam_bykey(const char* key);

protected:
	//------------------implementation IRedisSerializer---------------------
	virtual bool to_json(std::string& val);
	virtual bool from_json(boost::json::value& root);

public:
	//����id
	S_INT_64	iid;
	S_INT_64	token;
	NETSERVICE_TYPE	type;
	//��չ��Ϣ
	boost::unordered_map<std::string, std::string> extparams;

	//node��ip��ַ
	std::string ip;
	//node�Ķ˿�
	int port;
};

#endif //__SERVICENODEINFO_H__
