#ifndef __REDISOPTION_H__
#define __REDISOPTION_H__

#include <string>

namespace tinyxml2 {
	class XMLElement;
}

namespace config {
	class RedisOption
	{
	public:
		RedisOption();

		void load_from_xml(tinyxml2::XMLElement* el);

	public:
		std::string name_;

		//用户名口令
		std::string ip_;
		int port_;
		std::string auth_;
		int db_;
		int socket_timeout_;
	};
}

#endif	//__REDISOPTION_H__
