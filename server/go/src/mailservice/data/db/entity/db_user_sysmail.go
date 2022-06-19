package entity

type DBUserSysMail struct {
	Iid        int64 `xorm:"pk 'iid'"`
	RoleIid    int64 `xorm:"'role_iid'"`
	SysMailIid int64 `xorm:"'sys_mailiid'"`
}

func (u DBUserSysMail) TableName() string {
	return "user_sysmail"
}
