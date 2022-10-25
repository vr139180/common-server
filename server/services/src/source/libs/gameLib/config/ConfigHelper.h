#ifndef __CONFIGHELPER_H__
#define __CONFIGHELPER_H__

#include <map>
#include <list>
#include <string>
#include <vector>

#include <cmsLib/core_type.h>
#include "gameLib/config/GlobalOption.h"
#include "gameLib/config/EurekaConfig.h"
#include <gameLib/service_type.h>

namespace tinyxml2 {
	class XMLElement;
}

class ConfigHelper
{
private:
	ConfigHelper();

public:
	static ConfigHelper& instance();

	~ConfigHelper();

	bool init_config(NETSERVICE_TYPE mytype);

	NETSERVICE_TYPE get_type() { return type_; }
	S_INT_64 get_appsn(){ return appsn_;}
	S_INT_64 get_token() { return token_; }

	int get_cpunum(){ return cpunum_;}
	std::string get_ip() { return ip_; }
	int get_port() { return port_; }

	config::GlobalOption& get_globaloption() { return global_; }

public:
	//加载eureka配置项
	EurekaConfig* load_eureka_config();
 
protected:
	bool load_svrno();
	bool load_globaloption();

	void release();

private:
	NETSERVICE_TYPE	type_;

	S_INT_64	appsn_;
	S_INT_64	token_;

	int cpunum_;
	std::string ip_;
	int port_;

	std::map< std::string, std::string> cnfprop_;
	//全局配置
	config::GlobalOption global_;
};

#endif	//__CONFIGHELPER_H__
