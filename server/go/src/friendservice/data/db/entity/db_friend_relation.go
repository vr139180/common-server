package entity

import "time"

type DBFriendRelation struct {
	Iid        int64     `xorm:"pk autoincr 'iid'"`
	MyselfIid  int64     `xorm:"'myself_iid'"`
	FriendIid  int64     `xorm:"'friend_iid'"`
	CreateTime time.Time `xorm:"'createtime'"`
}

func (u DBFriendRelation) TableName() string {
	return "friend_relationship"
}

type DBFriendRelationView struct {
	Iid       int64  `xorm:"'iid'"`
	FriendIid int64  `xorm:"'friend_iid'"`
	NickName  string `xorm:"'nickname'"`
}
