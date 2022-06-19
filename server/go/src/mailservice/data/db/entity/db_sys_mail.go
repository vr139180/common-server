package entity

import "time"

type DBSysMail struct {
	Iid        int64     `xorm:"pk autoincr 'iid'"`
	Title      string    `xorm:"'title'"`
	Contents   string    `xorm:"'contents'"`
	Attachment int8      `xorm:"'attachment'"`
	AttachInfo string    `xorm:"'attachinfo'"`
	CreateTime time.Time `xorm:"'createtime'"`
}

func (s DBSysMail) TableName() string {
	return "sys_mail"
}
