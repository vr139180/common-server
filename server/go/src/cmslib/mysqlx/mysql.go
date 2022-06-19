package mysqlx

import (
	"cmslib/logx"
	"encoding/xml"
	"time"

	_ "github.com/go-sql-driver/mysql"
	"xorm.io/xorm"
)

// mysql配置
type MySqlOption struct {
	XMLName xml.Name `xml:"db"`

	DNS          string `xml:"dns,attr"`
	MaxIdleConns int    `xml:"maxidleconns,attr"`
	MaxOpenConns int    `xml:"maxopenconns,attr"`
	MaxLiveTime  int    `xml:"maxlivetime,attr"`
	ShowSQL      bool   `xml:"showsql,attr"`
}

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
