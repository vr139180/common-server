package entity

import "time"

type UserHome struct {
	Ver_ uint32 `xorm:"'ver_'"`

	RoleIid        int64     `xorm:"pk 'role_iid'"`
	HomeName       string    `xorm:"'home_name'"`
	GroundResId    int64     `xorm:"'ground_resid'"`
	LookAt         string    `xorm:"'look_at'"`
	GeoPos         string    `xorm:"'geo_pos'"`
	ResideTime     int32     `xorm:"'reside_time'"`
	LastResideDate time.Time `xorm:"'last_residedate'"`
	Levels         int32     `xorm:"'levels'"`
}

func (u UserHome) TableName() string {
	return "user_home"
}

//------------------------------------------------------------------------
type UserHomeStructure struct {
	Ver_ uint32 `xorm:"'ver_'"`

	BuildingIid    int64  `xorm:"pk 'building_iid'"`
	HomeIid        int64  `xorm:"'home_iid'"`
	ParentBuilding int64  `xorm:"'parent_building'"`
	BuildingResId  int64  `xorm:"'building_resid'"`
	LookAt         string `xorm:"'look_at'"`
	BuildingPos    string `xorm:"'building_pos'"`
	Levels         int32  `xorm:"'levels'"`
}

func (h UserHomeStructure) TableName() string {
	return "user_home_structure"
}
