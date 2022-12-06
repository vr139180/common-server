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
	"cmslib/gnet/pkg/logging"

	"github.com/sirupsen/logrus"
	xormlog "xorm.io/xorm/log"
)

// 日期格式
const timestampFormat = "2006-01-02 15:04:05"

type loggerOfGoLang struct {
	logger  *logrus.Logger
	showSQL bool
}

var log = &loggerOfGoLang{logger: logrus.New(), showSQL: false}

func (log *loggerOfGoLang) Debugf(format string, args ...interface{}) {
	log.logger.Debugf(format, args...)
}

// Infof logs messages at INFO level.
func (log *loggerOfGoLang) Infof(format string, args ...interface{}) {
	log.logger.Infof(format, args...)
}

// Warnf logs messages at WARN level.
func (log *loggerOfGoLang) Warnf(format string, args ...interface{}) {
	log.logger.Warnf(format, args...)
}

// Errorf logs messages at ERROR level.
func (log *loggerOfGoLang) Errorf(format string, args ...interface{}) {
	log.logger.Errorf(format, args...)
}

// Fatalf logs messages at FATAL level.
func (log *loggerOfGoLang) Fatalf(format string, args ...interface{}) {
	log.logger.Fatalf(format, args...)
}

func (log *loggerOfGoLang) initGlobalLogger(opt LoggerGlobalOption) error {
	lv, err := logrus.ParseLevel(opt.Level)
	if err != nil {
		log.logger.Errorf("config console log for logger error: %v", err)
		return err
	}

	log.logger.SetLevel(lv)

	return nil
}

//--------------------------------xorm log require-------------------------------------
func (log *loggerOfGoLang) Debug(args ...interface{}) {
	log.logger.Debug(args...)
}

// Infof logs messages at INFO level.
func (log *loggerOfGoLang) Info(args ...interface{}) {
	log.logger.Info(args...)
}

// Warnf logs messages at WARN level.
func (log *loggerOfGoLang) Warn(args ...interface{}) {
	log.logger.Warn(args...)
}

// Errorf logs messages at ERROR level.
func (log *loggerOfGoLang) Error(args ...interface{}) {
	log.logger.Error(args...)
}

func (log *loggerOfGoLang) Level() xormlog.LogLevel {
	v := log.logger.GetLevel()
	return xormlog.LogLevel(v)
}
func (log *loggerOfGoLang) SetLevel(l xormlog.LogLevel) {
}

func (log *loggerOfGoLang) ShowSQL(show ...bool) {
	if len(show) == 0 {
		log.showSQL = true
		return
	}
	log.showSQL = show[0]
}
func (log *loggerOfGoLang) IsShowSQL() bool {
	return log.showSQL
}

//--------------------------------------------------------------------------------------
func GetDefaultLogger() logging.Logger {
	return log
}

// 初始化logger
func InitLogger(options ...interface{}) error {

	for _, v := range options {
		switch v := v.(type) {
		case FileRotateLogOption:
			err := log.newLfsHook(v)
			if err != nil {
				return err
			}
		case ConsoleLogOption:
			err := log.newConsoleLog(v)
			if err != nil {
				return err
			}
		case LoggerGlobalOption:
			err := log.initGlobalLogger(v)
			if err != nil {
				return err
			}
		case ElasticLogOption:
		}
	}

	return nil
}

func Debugf(format string, args ...interface{}) {
	log.logger.Debugf(format, args...)
}

// Infof logs messages at INFO level.
func Infof(format string, args ...interface{}) {
	log.logger.Infof(format, args...)
}

// Warnf logs messages at WARN level.
func Warnf(format string, args ...interface{}) {
	log.logger.Warnf(format, args...)
}

// Errorf logs messages at ERROR level.
func Errorf(format string, args ...interface{}) {
	log.logger.Errorf(format, args...)
}

// Fatalf logs messages at FATAL level.
func Fatalf(format string, args ...interface{}) {
	log.logger.Fatalf(format, args...)
}

// 切换日志等级
func SetLoggerLevel(level string) error {
	lv, err := logrus.ParseLevel(level)
	if err != nil {
		log.logger.Errorf("config console log for logger error: %v", err)
		return err
	}

	log.logger.SetLevel(lv)

	return nil
}
