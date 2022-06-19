package config

import (
	"encoding/xml"
	"gamelib/service"
	"strings"
)

// 配置工具
type ConfigTool struct {
	SvrType  service.ServiceType //服务类型
	AppIid   int64               //服务iid
	AppToken int64               //服务token

	Ip       string //监听ip
	Port     int    //监听port
	LogLevel string
	LogDir   string
	LogFile  string
	//扩展配置
	ExtParams map[string]string

	cnfProps     map[string]string //配置选项
	ConfigSource IConfigSource     //配置源

	GlobalOpt GlobalOption //全局配置
}

func NewConfig(stype service.ServiceType) (*ConfigTool, error) {
	conf := new(ConfigTool)
	conf.SvrType = stype

	conf.cnfProps = make(map[string]string)

	err := conf.loadLocalConfig()
	if err != nil {
		return nil, err
	}

	return conf, nil
}

func (c *ConfigTool) loadLocalConfig() error {
	svr, err := loadSVRConfig("svrno")
	if err != nil {
		return err
	}

	c.Ip = svr.Ip
	c.Port = svr.Port
	c.LogLevel = svr.LogLevel
	c.LogDir = svr.LogDir
	c.LogFile = svr.LogFile
	c.ExtParams = svr.GetExtParams()

	if svr.IsFileSrc() {
		c.cnfProps["dirbase"] = svr.FileSrc.DataDir

		c.ConfigSource = new(FileConfigSource)
	} else {
		c.cnfProps["url"] = svr.HttpSrc.Url

		c.ConfigSource = new(HttpConfigSource)
	}

	c.ConfigSource.InitConfig(c.cnfProps)

	return nil
}

func (c *ConfigTool) GetExtParamByKey(k string) (v string, ok bool) {
	ok = true
	if c.ExtParams == nil {
		ok = false
		return
	}

	k = strings.ToLower(k)
	v, ok = c.ExtParams[k]

	return
}

// 加载全局配置
func (c *ConfigTool) LoadGlobalConfig() error {
	fstr := "/system/global.xml"
	cstr, err := c.ConfigSource.GetTextFile(fstr)
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &c.GlobalOpt)
	if err != nil {
		return err
	}

	return nil
}

// 加载配置文件
func (c *ConfigTool) LoadConfigContent(file string) ([]byte, error) {
	cstr, err := c.ConfigSource.GetTextFile(file)
	if err != nil {
		return nil, err
	}

	return cstr, nil
}
