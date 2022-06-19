#ifndef __SERVICE_TYPE_H__
#define __SERVICE_TYPE_H__

#include <string>

#include <gameLib/protobuf/Proto_all.h>

typedef PRO::ERK_SERVICETYPE NETSERVICE_TYPE;

class NetServiceType
{
public:
	static std::string to_string(NETSERVICE_TYPE t) {
		std::string ret = "";
		switch (t)
		{
		case NETSERVICE_TYPE::ERK_SERVICE_RES:
			ret = "res";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_GATE:
			ret = "gate";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_HOME:
			ret = "home";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_LOGIN:
			ret = "login";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_UNION:
			ret = "union";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_GAME:
			ret = "game";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_DATASYNC:
			ret = "datasync";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_ROUTER:
			ret = "router";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_CHAT:
			ret = "chat";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_MAIL:
			ret = "mail";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_FRIEND:
			ret = "friend";
			break;
		}

		return ret;
	}
};


#endif //__SERVICE_TYPE_H__
