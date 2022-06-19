package entity

import "time"

type DBUserUnBindItem struct {
	Iid        int64     `xorm:"pk 'iid'"`
	RoleIid    int64     `xorm:"'role_iid'"`
	IType      int32     `xorm:"'itype'"`
	ItemResIid int32     `xorm:"'item_resiid'"`
	ItemNum    int32     `xorm:"'item_num'"`
	SourceType int32     `xorm:"'source_type'"`
	SourceIid  int32     `xorm:"'source_iid'"`
	CreateTime time.Time `xorm:"'createtime'"`
}

func (s DBUserUnBindItem) TableName() string {
	return "user_unbind_item"
}
