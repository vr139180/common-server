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

type RoleBaseInfo struct {
	Ver_ uint32 `xorm:"'ver_'"`

	RoleIid   int64     `xorm:"pk 'role_iid'"`
	UserIid   int64     `xorm:"'user_iid'"`
	Nickname  string    `xorm:"'nickname'"`
	Registime time.Time `xorm:"'registime'"`
	Levels    int32     `xorm:"'levels'"`
	LocX      float32   `xorm:"'location_x'"`
	LocY      float32   `xorm:"'location_y'"`
	LocZ      float32   `xorm:"'location_z'"`
}

func (r RoleBaseInfo) TableName() string {
	return "role_baseinfo"
}
