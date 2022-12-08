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
