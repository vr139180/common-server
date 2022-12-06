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

package service

import "gamelib/protobuf/gpro"

type ServiceType int

const (
	ServiceType_None          ServiceType = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_NONE)
	ServiceType_Eureka                    = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_EUREKA)
	ServiceType_DataRouter                = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_DATAROUTER)
	ServiceType_State                     = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_STATE)
	ServiceType_Gate                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_GATE)
	ServiceType_Home                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_HOME)
	ServiceType_ServiceRouter             = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_SVRROUTER)
	ServiceType_Union                     = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_UNION)
	ServiceType_Chat                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_CHAT)
	ServiceType_Mail                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MAIL)
	ServiceType_Friend                    = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_FRIEND)
	ServiceType_FightRouter               = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_FIGHTROUTER)
	ServiceType_Game                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_GAME)
	ServiceType_MatchMaking               = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MATCHMAKING)
	ServiceType_DataSync                  = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_DATASYNC)
	ServiceType_Max                       = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MAX)
)

func ServiceTypeToString(t ServiceType) (r string) {
	switch t {
	case ServiceType_Eureka:
		r = "eureka"
	case ServiceType_DataRouter:
		r = "datarouter"
	case ServiceType_State:
		r = "state"
	case ServiceType_Gate:
		r = "gate"
	case ServiceType_Home:
		r = "home"
	case ServiceType_ServiceRouter:
		r = "svrrouter"
	case ServiceType_Union:
		r = "union"
	case ServiceType_Chat:
		r = "chat"
	case ServiceType_Mail:
		r = "mail"
	case ServiceType_Friend:
		r = "friend"
	case ServiceType_FightRouter:
		r = "fightrouter"
	case ServiceType_Game:
		r = "game"
	case ServiceType_MatchMaking:
		r = "matchmaking"
	case ServiceType_DataSync:
		r = "datasync"
	default:
		r = "none"
	}

	return
}
