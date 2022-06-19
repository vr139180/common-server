package entity

import "time"

type UserPet struct {
	Ver_ uint32 `xorm:"'ver_'"`

	MyPetIid int64     `xorm:"pk 'mypet_iid'"`
	RoleIid  int64     `xorm:"'role_iid'"`
	PetIid   int64     `xorm:"'pet_iid'"`
	PetAge   int32     `xorm:"'pet_age'"`
	Birthday time.Time `xorm:"'birthday'"`
}

func (u UserPet) TableName() string {
	return "user_pets"
}
