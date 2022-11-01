package service

import "gamelib/protobuf/gpro"

type ServiceType int

const (
	ServiceType_None          ServiceType = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_NONE)
	ServiceType_ServiceRouter             = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_SVRROUTER)
	ServiceType_FightRouter               = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_FIGHTROUTER)
	ServiceType_DataRouter                = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_DATAROUTER)
	ServiceType_State                     = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_STATE)
	ServiceType_Gate                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_GATE)
	ServiceType_Home                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_HOME)
	ServiceType_Login                     = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_LOGIN)
	ServiceType_Union                     = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_UNION)
	ServiceType_Game                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_GAME)
	ServiceType_DataSync                  = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_DATASYNC)
	ServiceType_Chat                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_CHAT)
	ServiceType_Mail                      = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MAIL)
	ServiceType_Friend                    = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_FRIEND)
	ServiceType_MatchMaking               = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MATCHMAKING)
	ServiceType_Max                       = ServiceType(gpro.ERK_SERVICETYPE_ERK_SERVICE_MAX)
)

func ServiceTypeToString(t ServiceType) (r string) {
	switch t {
	case ServiceType_State:
		r = "state"
	case ServiceType_Gate:
		r = "gate"
	case ServiceType_Home:
		r = "home"
	case ServiceType_Login:
		r = "login"
	case ServiceType_Union:
		r = "union"
	case ServiceType_Game:
		r = "game"
	case ServiceType_DataSync:
		r = "datasync"
	case ServiceType_ServiceRouter:
		r = "svrrouter"
	case ServiceType_FightRouter:
		r = "ftrouter"
	case ServiceType_DataRouter:
		r = "datarouter"
	case ServiceType_Chat:
		r = "chat"
	case ServiceType_Mail:
		r = "mail"
	case ServiceType_Friend:
		r = "friend"
	case ServiceType_MatchMaking:
		r = "matchmaking"
	default:
		r = "none"
	}

	return
}
