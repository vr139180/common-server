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

package mysqlx

import (
	"cmslib/logx"
	"time"

	_ "github.com/go-sql-driver/mysql"
	"xorm.io/xorm"
)

// mysql封装
type MysqlClient struct {
	*xorm.Engine
}

// 构建一个mysqlclient
func NewMysqlClient(opt MySqlOption) (mdb *MysqlClient, err error) {
	db, err := xorm.NewEngine("mysql", opt.DNS)
	if err != nil {
		return nil, err
	}

	mdb = new(MysqlClient)
	db.ShowSQL(opt.ShowSQL)
	db.SetLogger(logx.GetDefaultLogger())
	db.SetMaxIdleConns(opt.MaxIdleConns)
	db.SetMaxOpenConns(opt.MaxOpenConns)
	db.SetConnMaxLifetime(time.Second * time.Duration(opt.MaxLiveTime))

	err = db.Ping()
	if err != nil {
		return nil, err
	}

	mdb.Engine = db

	return mdb, err
}
