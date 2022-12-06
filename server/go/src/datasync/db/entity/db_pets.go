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
