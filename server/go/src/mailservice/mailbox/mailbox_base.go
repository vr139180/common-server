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

package mailbox

import (
	"cmslib/datas"
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

type IMailBox interface {
	GetType() MailBoxType
	GetReceiver() int64

	GetLoopIndex() int
	//是否超时无激活
	IsActiveTimeout(tnow int64) bool

	SyncUserToken(token protocolx.UserToken)

	NewMail(mail *gpro.MailNormalItem)

	GetMailsFromBox(pro *protocolx.NetProtocol)
	ReadMail(token protocolx.UserToken, mailiid int64)
	DeleteMail(token protocolx.UserToken, mailiid int64)
}

//----------------------------------implement IMailBox interface-------------------
type MailBoxBase struct {
	datas.DoubleLinkBase

	//mail排序，从小到大
	mails gpro.MailNormalItems

	//上次激活时间
	lastUpdate int64

	//base info
	ctype MailBoxType
	//接收方
	receiver int64
	//分配到的channel goroutine index
	loopIndex int
}

func (cb *MailBoxBase) InitBase(t MailBoxType, cid int64, cind int) {
	cb.ctype = t
	cb.receiver = cid
	cb.loopIndex = cind
	cb.lastUpdate = utilc.GetTimestamp()
	cb.mails.Reset()
}

func (cb *MailBoxBase) GetType() MailBoxType {
	return cb.ctype
}

func (cb *MailBoxBase) GetLoopIndex() int {
	return cb.loopIndex
}

func (cb *MailBoxBase) GetReceiver() int64 {
	return cb.receiver
}

func (cb *MailBoxBase) IsActiveTimeout(tnow int64) bool {
	return (cb.lastUpdate <= tnow)
}

func (cb *MailBoxBase) cloneMailNormalItem(mail *gpro.MailNormalItem) *gpro.MailNormalItem {
	return proto.Clone(mail).(*gpro.MailNormalItem)
}

func (cb *MailBoxBase) queryMails(mailiid int64, next bool, num int) (dat []*gpro.MailNormalItem) {
	if len(cb.mails.Mails) == 0 {
		return
	}

	var mailiidNotExist bool
	mailiidNotExist = true
	mlen := len(cb.mails.Mails) - 1

	if mailiid > 0 {
		for i := mlen; i >= 0; i-- {
			d := cb.mails.Mails[i]
			if d.Iid == mailiid {
				mailiidNotExist = false

				//开始获取指定的邮件数
				for j := 1; j <= num; j++ {
					ind := i
					if next {
						ind = ind - j
						if ind < 0 {
							break
						}

						dat = cb.insertMailList(dat, cb.mails.Mails[ind])
					} else {
						ind = ind + j
						if ind > mlen {
							break
						}

						dat = append(dat, cb.mails.Mails[ind])
					}
				}

				break
			}
		}
	}

	//如果传入de mailiid不存在，默认从最大的邮件开始获取num数量的邮件
	if mailiidNotExist {
		i := mlen
		j := num
		for i >= 0 && j > 0 {
			d := cb.mails.Mails[i]
			dat = cb.insertMailList(dat, d)

			i--
			j--
		}
	}

	return
}

func (cb *MailBoxBase) getMail(mailiid int64) *gpro.MailNormalItem {
	mlen := len(cb.mails.Mails) - 1
	if mlen < 0 {
		return nil
	}

	for i := mlen; i >= 0; i-- {
		d := cb.mails.Mails[i]
		if d.Iid == mailiid {
			return d
		}
	}

	return nil
}

func (cb *MailBoxBase) deleteOneMail(mailiid int64) *gpro.MailNormalItem {
	mlen := len(cb.mails.Mails) - 1
	if mlen < 0 {
		return nil
	}

	for i := mlen; i >= 0; i-- {
		d := cb.mails.Mails[i]
		if d.Iid == mailiid {
			copy(cb.mails.Mails[i:], cb.mails.Mails[i+1:])
			cb.mails.Mails = cb.mails.Mails[:mlen]
			return d
		}
	}

	return nil
}

func (cb *MailBoxBase) AddNewMail(mail *gpro.MailNormalItem) bool {
	if len(cb.mails.Mails) == 0 {
		cb.mails.Mails = append(cb.mails.Mails, mail)
		return true
	} else {
		ind := -1
		for i := len(cb.mails.Mails) - 1; i >= 0; i-- {
			d := cb.mails.Mails[i]
			if d.Iid > mail.Iid {
				continue
			} else if d.Iid == mail.Iid {
				return false
			} else {
				ind = i
				ind++
				if ind >= len(cb.mails.Mails) {
					ind = -1
				}
				break
			}
		}

		if ind == -1 {
			cb.mails.Mails = append(cb.mails.Mails, mail)
		} else {
			cb.mails.Mails = append(cb.mails.Mails, nil)
			copy(cb.mails.Mails[ind+1:], cb.mails.Mails[ind:])
			cb.mails.Mails[ind] = mail
		}

		return true
	}
}

func (cb *MailBoxBase) insertMailList(ms []*gpro.MailNormalItem, mail *gpro.MailNormalItem) []*gpro.MailNormalItem {
	ms = append(ms, nil)
	copy(ms[1:], ms[0:])
	ms[0] = mail

	return ms
}

func (cb *MailBoxBase) SyncUserToken(token protocolx.UserToken) {
}

func (cb *MailBoxBase) NewMail(mail *gpro.MailNormalItem) {
}

func (cb *MailBoxBase) GetMailsFromBox(pro *protocolx.NetProtocol) {
}

func (cb *MailBoxBase) ReadMail(token protocolx.UserToken, mailiid int64) {
}

func (cb *MailBoxBase) DeleteMail(token protocolx.UserToken, mailiid int64) {
}

//-----------------------------------------------------------------
func NewOneMailBox(t MailBoxType, receiver int64, ch *MailBoxHolder) (c IMailBox) {
	c = nil
	if t == MailBoxType_User {
		c = newMailBoxRoleOfUser(t, receiver, ch)
	}

	return
}
