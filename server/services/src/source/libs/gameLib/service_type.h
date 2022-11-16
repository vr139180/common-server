// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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
		case NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER:
			ret = "datarouter";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_STATE:
			ret = "state";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_GATE:
			ret = "gate";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_HOME:
			ret = "home";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER:
			ret = "svrrouter";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_UNION:
			ret = "union";
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
		case NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER:
			ret = "ftrouter";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_GAME:
			ret = "game";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_MATCHMAKING:
			ret = "matchmaking";
			break;
		case NETSERVICE_TYPE::ERK_SERVICE_DATASYNC:
			ret = "datasync";
			break;
		}

		return ret;
	}
};


#endif //__SERVICE_TYPE_H__
