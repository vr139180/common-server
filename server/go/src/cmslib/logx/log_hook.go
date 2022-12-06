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
	"os"
	"time"

	rotatelogs "github.com/lestrrat-go/file-rotatelogs"
	"github.com/rifflock/lfshook"
	"github.com/sirupsen/logrus"
)

// 构造滚动日志器
func (log *loggerOfGoLang) newLfsHook(opt FileRotateLogOption) error {

	writer, err := rotatelogs.New(
		opt.GetFilePath(),
		// WithLinkName为最新的日志建立软连接，以方便随着找到当前日志文件
		rotatelogs.WithLinkName(opt.GetLinkFile()),

		// WithRotationTime设置日志分割的时间，这里设置为一天分割一次
		rotatelogs.WithRotationTime(time.Hour*24),

		// WithMaxAge和WithRotationCount二者只能设置一个，
		// WithMaxAge设置文件清理前的最长保存时间，
		// WithRotationCount设置文件清理前最多保存的个数。
		//rotatelogs.WithMaxAge(time.Hour*24),
		rotatelogs.WithRotationCount(opt.RemainFileNum),
	)

	if err != nil {
		log.Errorf("config rotate file log for logger error: %v", err)
		return err
	}

	writerMap := lfshook.WriterMap{
		logrus.DebugLevel: writer,
		logrus.InfoLevel:  writer,
		logrus.WarnLevel:  writer,
		logrus.ErrorLevel: writer,
		logrus.FatalLevel: writer,
		logrus.PanicLevel: writer,
	}

	lfsHook := lfshook.NewHook(writerMap, &logrus.TextFormatter{DisableColors: true, TimestampFormat: timestampFormat})

	log.logger.AddHook(lfsHook)

	return nil
}

// 控制台日志输出
func (log *loggerOfGoLang) newConsoleLog(opt ConsoleLogOption) error {

	log.logger.Out = os.Stdout

	//设置日期格式
	log.logger.SetFormatter(&logrus.TextFormatter{
		TimestampFormat: timestampFormat,
	})

	return nil
}
