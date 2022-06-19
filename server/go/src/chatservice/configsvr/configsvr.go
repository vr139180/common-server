package configsvr

import (
	"encoding/xml"
	"gamelib/config"
)

// 服务器配置
type ChatServiceConfig struct {
	XMLName xml.Name `xml:"chat"`

	CHProcessorNum int                `xml:"chprocessor,attr"` //chprocessor
	RedisOpt       config.RedisOption `xml:"redis"`
}
