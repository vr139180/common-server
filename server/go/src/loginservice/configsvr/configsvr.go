package configsvr

import (
	"cmslib/mysqlx"
	"encoding/xml"
	"gamelib/config"
)

// 服务器配置
type LoginServiceConfig struct {
	XMLName xml.Name `xml:"login"`

	VerifyLoopNum int `xml:"vloop,attr"`
	RegistLoopNum int `xml:"rloop,attr"`

	RedisOpt config.RedisOption `xml:"redis"`
	DBOpt    mysqlx.MySqlOption `xml:"db"`
}
