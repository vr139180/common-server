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

package config

import (
	"encoding/xml"
	"gamelib/service"
	"strings"
)

// 配置工具
type ConfigTool struct {
	SvrType service.ServiceType //服务类型

	Nets int    //netthreads
	Ip   string //监听ip
	Port int    //监听port
	//扩展配置
	ExtParams map[string]string
	cnfProps  map[string]string //配置选项

	GlobalOpt GlobalOption //全局配置

	LogLevel string
	LogDir   string
	LogFile  string

	ConfigSource IConfigSource //配置源
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
	c.Nets = svr.Nets
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
