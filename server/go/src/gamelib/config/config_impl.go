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
	"cmslib/utilc"
	"errors"
	"io/ioutil"
	"os"
	"path/filepath"
)

type IConfigSource interface {
	// 初始化source参数
	InitConfig(props map[string]string)
	// 获取相对目录的文件内容
	GetTextFile(file string) ([]byte, error)
}

// 缺省接口实现
type BaseConfigSource struct {
	Props map[string]string
}

func (c *BaseConfigSource) InitConfig(props map[string]string) {
	c.Props = props
}

func (c *BaseConfigSource) GetTextFile(file string) ([]byte, error) {
	return nil, errors.New("not implement GetTextFile function")
}

// 本地文件系统的配置来源
type FileConfigSource struct {
	BaseConfigSource
}

func (f *FileConfigSource) GetTextFile(file string) ([]byte, error) {
	basedir, err := utilc.GetBinPath()
	if err != nil {
		return nil, err
	}

	dfile := filepath.Join(basedir, f.Props["dirbase"], file)

	fs, err := os.Open(dfile)
	if err != nil {
		return nil, err
	}

	defer fs.Close()

	data, err := ioutil.ReadAll(fs)
	if err != nil {
		return nil, err
	}

	return data, nil
}

// http来源的配置
type HttpConfigSource struct {
	BaseConfigSource
}

func (h *HttpConfigSource) GetTextFile(file string) ([]byte, error) {
	return nil, nil
}
