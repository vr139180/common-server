package configsvr

import (
	"encoding/xml"
	"gamelib/config"
)

// 服务器配置
type ResServiceConfig struct {
	XMLName xml.Name `xml:"res"`

	NetProcessorNum int                `xml:"netprocessor,attr"` //netprocessor
	RedisOpt        config.RedisOption `xml:"redis"`
}
