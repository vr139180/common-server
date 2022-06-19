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
