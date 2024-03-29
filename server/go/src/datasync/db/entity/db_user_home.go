// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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
