package db

type DBUserBagItem struct {
	Ver_       uint32 `xorm:"'ver_'"`
	Iid        int64  `xorm:"pk 'iid'"`
	RoleIid    int64  `xorm:"'role_iid'"`
	IType      int32  `xorm:"'itype'"`
	ItemResIid int32  `xorm:"'item_resiid'"`
	ItemNum    int32  `xorm:"'item_num'"`
	SlotNum    int32  `xorm:"'slot_num'"`
}

func (s DBUserBagItem) TableName() string {
	return "user_bag_item"
}
