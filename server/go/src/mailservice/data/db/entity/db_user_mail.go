package entity

import "time"

type DBUserMail struct {
	Ver_ uint32 `xorm:"'ver_'"`

	Iid         int64     `xorm:"pk autoincr 'iid'"`
	SType       int32     `xorm:"'stype'"`
	SenderIid   int64     `xorm:"'sender_iid'"`
	ReceiverIid int64     `xorm:"'receiver_iid'"`
	Attachment  int8      `xorm:"'attachment'"`
	AttachInfo  string    `xorm:"'attachinfo'"`
	Title       string    `xorm:"'title'"`
	Contents    string    `xorm:"'contents'"`
	CreateTime  time.Time `xorm:"'createtime'"`
	ReadTime    time.Time `xorm:"'readtime'"`
}

func (u DBUserMail) TableName() string {
	return "user_mail"
}
