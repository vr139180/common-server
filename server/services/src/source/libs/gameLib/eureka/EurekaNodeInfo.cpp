#include "gameLib/eureka/EurekaNodeInfo.h"

#include <boost/json.hpp>
#include <cmsLib/json/JSONUtil.h>

EurekaNodeInfo::EurekaNodeInfo():iid( 0)
,port( 0)
,token( 0)
{
}

EurekaNodeInfo& EurekaNodeInfo::operator = (const EurekaNodeInfo& v)
{
	this->iid = v.iid;
	this->ip = v.ip;
	this->port = v.port;
	this->token = v.token;

	return *this;
}

EurekaNodeInfo* EurekaNodeInfo::clone()
{
	EurekaNodeInfo* r = new EurekaNodeInfo();
	*r = (*this);
	return r;
}

bool EurekaNodeInfo::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", iid},
		{"ip", ip},
		{"port", port},
		{"token", token}
	};

	val = boost::json::serialize(json);

	return true;
}

bool EurekaNodeInfo::from_json(boost::json::value& root)
{
	boost::json::object obj = root.as_object();

	this->iid = JSONUtil::get_int64( obj, "iid", 0);
	this->ip =JSONUtil::get_string( obj, "ip", "");
	this->port = JSONUtil::get_value<int>(obj, "port", 0);
	this->token = JSONUtil::get_int64(obj, "token", 0);

	return true;
}
