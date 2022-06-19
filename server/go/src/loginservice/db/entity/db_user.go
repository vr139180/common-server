package entity

//用户基本信息
type UserAccount struct {
	UserIid  int64  `xorm:"'user_iid'"`
	Account  string `xorm:"'account'"`
	Password string `xorm:"'pwd'"`
}

func (u UserAccount) TableName() string {
	return "user_account"
}
