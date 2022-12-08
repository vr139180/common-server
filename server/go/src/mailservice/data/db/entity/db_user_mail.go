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

type DBUserMail struct {
	Ver_ uint32 `xorm:"'ver_'"`

	Iid         int64     `xorm:"pk autoincr 'iid'"`
	SType       int32     `xorm:"'stype'"`
	SenderIid   int64     `xorm:"'sender_iid'"`
	ReceiverIid int64     `xorm:"'receiver_iid'"`
	Attachment  int8      `xorm:"'attachment'"`
	AttachInfo  string    `xorm:"'attachinfo'"`
	Title       string    `xorm:"'title'"`
	Contents    string    `xorm:"'contents'"`
	CreateTime  time.Time `xorm:"'createtime'"`
	ReadTime    time.Time `xorm:"'readtime'"`
}

func (u DBUserMail) TableName() string {
	return "user_mail"
}
