package entity

type DBUserMailBox struct {
	Ver_ uint32 `xorm:"'ver_'"`

	RoleIid     int64 `xorm:"pk 'role_iid'"`
	SysMailSync int64 `xorm:"'sysmail_sync'"`
}

func (u DBUserMailBox) TableName() string {
	return "user_mailbox"
}
