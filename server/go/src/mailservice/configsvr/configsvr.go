package configsvr

import (
	"cmslib/mysqlx"
	"encoding/xml"
	"gamelib/config"
)

// 服务器配置
type MailServiceConfig struct {
	XMLName xml.Name `xml:"mail"`

	MLProcessorNum int                `xml:"mlprocessor,attr"` //mlprocessor
	RedisOpt       config.RedisOption `xml:"redis"`
	DBOpt          mysqlx.MySqlOption `xml:"db"`
}
