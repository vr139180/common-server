package entity

//用户基本信息
type UserRoleInfo struct {
	RoleIid int64 `xorm:"pk 'role_iid'"`
}

func (u UserRoleInfo) TableName() string {
	return "role_baseinfo"
}
