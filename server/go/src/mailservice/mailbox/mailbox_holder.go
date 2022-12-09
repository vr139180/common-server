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
	"cmslib/logx"
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"mailservice/data/db/entity"
	"mailservice/g"
)

//--------------------------------------------------------------------------------------------
type MBOXHANDLER func(*protocolx.NetProtocol)

type mboxNetCmdHandler struct {
	pro *protocolx.NetProtocol
	f   MBOXHANDLER
}

func newMBoxNetCmdHandle(pro *protocolx.NetProtocol, f MBOXHANDLER) (c *mboxNetCmdHandler) {
	c = new(mboxNetCmdHandler)
	c.pro = pro
	c.f = f

	return
}

func (c *mboxNetCmdHandler) ProcessMail() {
	c.f(c.pro)
}

//--------------------------------------------------------------------------------------------
//对应processor.loop的mailbox存储结构
type MailBoxHolder struct {
	//对应processor.中的loop index
	loopIndex int

	//double link和hashmap组合的channel数据
	mailboxs map[int64]IMailBox
	//根据活动情况排列的 双向列表
	//方便对不活跃的channel进行维护
	mailboxsLink *datas.DoubleLinkUnSafe

	parent *MailBoxCtrl
}

func newMailBoxHolder(l int, p *MailBoxCtrl) (h *MailBoxHolder) {
	h = new(MailBoxHolder)
	h.loopIndex = l
	h.mailboxs = make(map[int64]IMailBox)
	h.mailboxsLink = datas.NewDoubleLinkUnSafe()

	h.parent = p

	return
}

func (ch *MailBoxHolder) GetMailBoxBy(receiver int64) IMailBox {
	d, ok := ch.mailboxs[receiver]
	if !ok {
		return nil
	}

	return d
}

func (ch *MailBoxHolder) OneDayMaintance() {

}

func (ch *MailBoxHolder) CacheUserMailBox(roleiid int64, ubox *entity.DBUserMailBox, mitems []*gpro.MailNormalItem, token protocolx.UserToken) IMailBox {
	mbox := ch.GetMailBoxBy(roleiid)
	if mbox == nil {
		userbox := newMailBoxRoleOfUser(MailBoxType_User, roleiid, ch)
		ch.mailboxsLink.AddHeadElement(userbox)
		ch.mailboxs[userbox.GetReceiver()] = userbox

		userbox.InitUserMailBox(ubox, mitems, token, 0)
		mbox = userbox

		logx.Infof("MailBoxHolder->CacheUserMailBox cache user:%d", roleiid)
	}

	return mbox
}

func (ch *MailBoxHolder) CacheUserMailFromSystemMail(mails []*gpro.MailNormalItem, sysmailsync int64, roleiid int64) {
	//cache to redis
	rd := g.GetRedis()
	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	if len(mails) > 0 {
		udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, roleiid)

		for _, d := range mails {
			dat := utilc.ProtoToBytes(d)
			if dat == nil {
				continue
			}

			k1 := rd.BuildKey(USERMAILBOX_DATA_MAILX, d.Iid)
			k2 := rd.BuildKey("%d", d.Iid)

			rd.HSet(udkey, k1, dat)
			rd.AddZset(udlskey, k2, float64(d.Iid))
		}
	}

	sv, err := rd.HGet(udkey, USERMAILBOX_DATA_SYSMAILSYNC)
	if err == nil {
		nsv := utilc.StringToI64(sv, 0)
		if sysmailsync > nsv {
			rd.HSet(udkey, USERMAILBOX_DATA_SYSMAILSYNC, sysmailsync)
		}
	}

	mbox := ch.GetMailBoxBy(roleiid)
	if mbox != nil {
		ubox := mbox.(*MailBoxRoleOfUser)
		ubox.SaveSystemMails(mails, sysmailsync)
	}
}

func (ch *MailBoxHolder) NewUserMail(mail *gpro.MailNormalItem) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, mail.ReceiverIid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	dat := utilc.ProtoToBytes(mail)
	if dat == nil {
		return
	}

	k1 := rd.BuildKey(USERMAILBOX_DATA_MAILX, mail.Iid)
	rd.HSet(udkey, k1, dat)

	udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, mail.ReceiverIid)
	k2 := rd.BuildKey("%d", mail.Iid)
	rd.AddZset(udlskey, k2, float64(mail.Iid))

	mbox := ch.GetMailBoxBy(mail.ReceiverIid)
	if mbox != nil {
		mbox.NewMail(mail)
	}
}

func (ch *MailBoxHolder) ReadMail(roleiid int64, mailiid int64, token protocolx.UserToken) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	k1 := rd.BuildKey(USERMAILBOX_DATA_MAILX, mailiid)
	d, err := rd.HGet(udkey, k1)
	if err != nil {
		return
	}

	item := &gpro.MailNormalItem{}
	err = utilc.BytesToProto([]byte(d), item)
	if err != nil {
		return
	}
	item.Readtime = utilc.GetTimeSecond()
	dat := utilc.ProtoToBytes(item)
	if dat == nil {
		return
	}
	rd.HSet(udkey, k1, dat)

	mbox := ch.GetMailBoxBy(roleiid)
	if mbox != nil {
		mbox.ReadMail(token, mailiid)
	}

}

func (ch *MailBoxHolder) DeleteMail(roleiid int64, mailiid int64, token protocolx.UserToken) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}
	udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, roleiid)

	k1 := rd.BuildKey(USERMAILBOX_DATA_MAILX, mailiid)
	k2 := rd.BuildKey("%d", mailiid)
	rd.HDel(udkey, []string{k1})
	rd.ZsetDel(udlskey, []interface{}{k2})

	mbox := ch.GetMailBoxBy(roleiid)
	if mbox != nil {
		mbox.DeleteMail(token, mailiid)
	}
}

//run in the boxholder goroutine
func (ch *MailBoxHolder) OnNetCmdHanderl(pro *protocolx.NetProtocol) {

	roleid := pro.GetRoleIid()

	mbox := ch.GetMailBoxBy(roleid)
	if mbox == nil {
		ubox, success := initUserMailBoxFromRedis(roleid, pro.GetUserToken(), ch)
		if success {
			mbox = ubox
			ch.mailboxsLink.AddHeadElement(ubox)
			ch.mailboxs[ubox.GetReceiver()] = ubox
		} else {
			//load from database
			cmd := newDBLoadUserMailCmd(roleid, ch, pro)
			g.PostDBProcessor(cmd)
		}
	}

	if mbox != nil {
		ch.triggerNetProcess(mbox, pro)
	}
}

func (ch *MailBoxHolder) triggerNetProcess(mbox IMailBox, pro *protocolx.NetProtocol) {
	id := pro.GetMsgId()
	if id == uint16(gpro.MAIL_PROTYPE_MAIL_USERONLINE_ACTIVE) {
		msg := pro.Msg.(*gpro.Mail_UserOnlineActive)
		ubox := mbox.(*MailBoxRoleOfUser)
		ch.mailboxsLink.ElementMoveToHead(ubox)
		ubox.mailBoxActive(pro.GetUserToken(), msg.Firstactive, msg.LastMailiid)

	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_MAILGET_REQ) {
		mbox.GetMailsFromBox(pro)
	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_READMAIL_REQ) {
		msg := pro.Msg.(*gpro.Mail_ReadMailReq)
		cmd := newDBReadMailCmd(msg.MailIid, mbox.GetReceiver(), ch, pro.GetUserToken())
		g.PostDBProcessor(cmd)
	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_DELETEMAIL_REQ) {
		msg := pro.Msg.(*gpro.Mail_DeleteMailReq)
		cmd := newDBDeleteMailCmd(msg.MailIid, mbox.GetReceiver(), ch, pro.GetUserToken())
		g.PostDBProcessor(cmd)
	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_UNBINDATTACHS_REQ) {
	}
}
