package entity

import "time"

type DBFriendInvite struct {
	Iid         int64     `xorm:"pk autoincr 'iid'"`
	FromRoleIid int64     `xorm:"'from_roleiid'"`
	ToRoleIid   int64     `xorm:"'to_roleiid'"`
	InviteTime  time.Time `xorm:"'invite_time'"`
}

func (u DBFriendInvite) TableName() string {
	return "friend_invites"
}

type DBFriendInviteView struct {
	Iid         int64     `xorm:"'iid'"`
	FromRoleIid int64     `xorm:"'from_roleiid'"`
	InviteTime  time.Time `xorm:"'invite_time'"`
	InviteName  string    `xorm:"'invite_name'"`
}
