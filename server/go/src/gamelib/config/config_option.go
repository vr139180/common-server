package config

import "encoding/xml"

// 全局配置
type GlobalOption struct {
	XMLName    xml.Name `xml:"global"`
	SvrNumMax  int      `xml:"svrnum_min,attr"` //最大服务数
	EurekaIp   string   `xml:"eip,attr"`        //连接的eureka节点ip
	EurekaPort int      `xml:"eport,attr"`
}

// redis配置
type RedisOption struct {
	XMLName       xml.Name `xml:"redis"`
	Name          string   `xml:"name,attr"` //配置名称 master, slave
	Ip            string   `xml:"ip,attr"`   //ip
	Port          int      `xml:"port,attr"`
	Auth          string   `xml:"auth,attr"` //口令
	Db            int      `xml:"db,attr"`   //db
	SocketTimeout int      `xml:"socket_timeout,attr"`
}
