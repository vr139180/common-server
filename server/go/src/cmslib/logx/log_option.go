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

package logx

import (
	"path"
	"path/filepath"
)

type LoggerGlobalOption struct {
	Level string //日志级别
}

// 文件滚动日志配置
// 每天一个日志，日志文件名 xxxx_年月日.xxx
type FileRotateLogOption struct {
	BasePath      string //存放路径
	FileName      string //文件名称，xxxx_%Y%m%d.xxx
	RemainFileNum uint   //保存文件数
}

// 构造文件路径
func (f *FileRotateLogOption) GetFilePath() string {
	fext := path.Ext(f.FileName)
	fprefix := f.FileName[0 : len(f.FileName)-len(fext)]

	file := fprefix + "_%Y%m%d%H" + fext

	file = filepath.Join(f.BasePath, file)
	return file
}

func (f *FileRotateLogOption) GetLinkFile() string {
	return filepath.Join(f.BasePath, f.FileName)
}

// 控制台日志输出
type ConsoleLogOption struct {
}

// es日志记录，未实现
type ElasticLogOption struct {
}
