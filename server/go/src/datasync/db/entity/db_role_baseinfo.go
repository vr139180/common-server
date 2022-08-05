package entity

import "time"

type RoleBaseInfo struct {
	Ver_ uint32 `xorm:"'ver_'"`

	RoleIid   int64     `xorm:"pk 'role_iid'"`
	UserIid   int64     `xorm:"'user_iid'"`
	Nickname  string    `xorm:"'nickname'"`
	Registime time.Time `xorm:"'registime'"`
	Levels    int32     `xorm:"'levels'"`
}

func (r RoleBaseInfo) TableName() string {
	return "role_baseinfo"
}
