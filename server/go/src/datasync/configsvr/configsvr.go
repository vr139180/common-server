package configsvr

import (
	"cmslib/mysqlx"
	"encoding/xml"
	"gamelib/config"
)

// 服务器配置
type DataSyncConfig struct {
	XMLName xml.Name `xml:"datasync"`

	SerializeLoopNum int `xml:"sloop,attr"`

	RedisOpt config.RedisOption `xml:"redis"`
	DBOpt    mysqlx.MySqlOption `xml:"db"`
}
