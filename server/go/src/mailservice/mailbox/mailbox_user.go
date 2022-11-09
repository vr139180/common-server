package mailbox

import (
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"mailservice/data/db/entity"
	"mailservice/g"
)

const (
	//7 days
	USER_MAILBOX_EXPIRESEC = 60 * 60 * 24 * 7
	//redis key
	REDIS_USERMAILBOX_DATA = "-MAIL:USER:%d"
	REDIS_USERMAILBOX_LS   = "-MAIL:USER:LS-%d"
	//ls结构中的占位mail，实际处理中需要过滤
	REDIS_USERMAILBOX_LS_PLACEHODER = "0"
	//data properties
	//邮件列表
	USERMAILBOX_DATA_MAIL  = "m:%s"
	USERMAILBOX_DATA_MAILX = "m:%d"
	//SysMailSync
	USERMAILBOX_DATA_SYSMAILSYNC = "ext:sss"
	//LastMailGet
	USERMAILBOX_DATA_LASTMAILGET = "ext:lmg"
)

type MailBoxRoleOfUser struct {
	MailBoxBase

	//已经同步到redis和database的系统邮件编号
	SysMailSync int64
	//最大的邮件iid
	MaxMailIid int64

	//mailget请求的最大mailiid
	MailGetMaxMail int64
	//notify mailiid,最后一次新邮件提示id
	NewMailNotifyIid int64

	//总的邮件数和未读邮件数
	totleNum  int
	unreadNum int

	//user info
	inited  bool
	userIid int64
	gateIid int64
	//user token head
	token protocolx.UserToken

	ch *MailBoxHolder
}

func newMailBoxRoleOfUser(t MailBoxType, receiver int64, ch *MailBoxHolder) (m *MailBoxRoleOfUser) {
	m = new(MailBoxRoleOfUser)
	m.InitBase(t, receiver, ch.loopIndex)
	m.inited = false
	m.ch = ch
	m.MaxMailIid = 0
	m.MailGetMaxMail = 0
	m.NewMailNotifyIid = 0
	m.totleNum = 0
	m.unreadNum = 0

	return
}

func initUserMailBoxFromRedis(roleiid int64, token protocolx.UserToken, ch *MailBoxHolder) (u *MailBoxRoleOfUser, success bool) {
	success = false

	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, roleiid)
	udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, roleiid)
	ok, err := rd.KeyExist(udkey)
	//redis中不存在
	if err != nil || !ok {
		return
	}

	//load from redis
	mkeys, _ := rd.GetZsetAllMOfKey(udlskey)
	mvals, _ := rd.HGetAll(udkey)

	u = newMailBoxRoleOfUser(MailBoxType_User, roleiid, ch)

	ubox := &entity.DBUserMailBox{}
	sv, exist := mvals[USERMAILBOX_DATA_SYSMAILSYNC]
	if exist {
		ubox.SysMailSync = utilc.StringToI64(sv, 0)
	} else {
		ubox.SysMailSync = 0
	}
	var lastMailGet int64
	sv, exist = mvals[USERMAILBOX_DATA_LASTMAILGET]
	if exist {
		lastMailGet = utilc.StringToI64(sv, 0)
	} else {
		lastMailGet = 0
	}

	var mitems []*gpro.MailNormalItem
	if len(mkeys) > 0 {
		for _, v := range mkeys {
			//过滤占位信息，用于保持ls结构
			if v == REDIS_USERMAILBOX_LS_PLACEHODER {
				continue
			}

			rk := rd.BuildKey(USERMAILBOX_DATA_MAIL, v)
			sv, exist = mvals[rk]
			if !exist {
				continue
			}

			it := &gpro.MailNormalItem{}
			err = utilc.BytesToProto([]byte(sv), it)
			if err != nil {
				continue
			}

			mitems = append(mitems, it)
		}

	}

	u.InitUserMailBox(ubox, mitems, token, lastMailGet)

	success = true
	return
}

func (u *MailBoxRoleOfUser) InitUserMailBox(ubox *entity.DBUserMailBox, mitems []*gpro.MailNormalItem, token protocolx.UserToken, mailget int64) {
	if u.inited {
		return
	}
	u.inited = true

	u.SysMailSync = ubox.SysMailSync
	u.MailGetMaxMail = mailget
	if len(mitems) > 0 {
		u.MaxMailIid = mitems[len(mitems)-1].Iid
		for _, d := range mitems {
			if d.Readtime <= 0 {
				u.unreadNum++
			}
		}
	} else {
		u.MaxMailIid = 0
	}

	u.totleNum = len(mitems)
	//save mails
	u.mails.Mails = mitems

	u.saveAllToRedis()

	u.SyncUserToken(token)
}

func (u *MailBoxRoleOfUser) saveAllToRedis() {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, u.GetReceiver())
	ok, err := rd.KeyExist(udkey)
	//redis中存在
	if err != nil || ok {
		return
	}

	udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, u.GetReceiver())

	rd.HSet(udkey, USERMAILBOX_DATA_SYSMAILSYNC, u.SysMailSync)
	rd.HSet(udkey, USERMAILBOX_DATA_LASTMAILGET, u.MailGetMaxMail)

	//placeholder
	rd.AddZset(udlskey, REDIS_USERMAILBOX_LS_PLACEHODER, float64(0))

	if len(u.mails.Mails) > 0 {
		for _, d := range u.mails.Mails {
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

	rd.Expire(udkey, USER_MAILBOX_EXPIRESEC)
	rd.Expire(udlskey, USER_MAILBOX_EXPIRESEC)
}

func (u *MailBoxRoleOfUser) SyncUserToken(token protocolx.UserToken) {
	u.token = token

	u.gateIid = token.GetTokenGateIid()
	u.userIid = token.GetTokenRoleIid()
}

func (u *MailBoxRoleOfUser) SyncSystemMail() {
	sysBox := u.ch.parent.sysMailBox
	if sysBox == nil {
		return
	}

	mails, maxiid, ok := sysBox.GetSysMailsGreatIid(u.SysMailSync)
	if !ok {
		return
	}

	cmd := newDBSaveSystemMailCmd(mails, maxiid, u.GetReceiver(), u.ch)
	g.PostDBProcessor(cmd)
}

func (u *MailBoxRoleOfUser) SaveSystemMails(mails []*gpro.MailNormalItem, sysmailsync int64) {
	if u.SysMailSync < sysmailsync {
		u.SysMailSync = sysmailsync
	}

	if len(mails) > 0 {
		for _, d := range mails {
			u.NewMail(d)
		}
	}
}

func (u *MailBoxRoleOfUser) mailBoxActive(token protocolx.UserToken, first bool, lastMailGet int64) {

	if first {
		rd := g.GetRedis()
		udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, u.GetReceiver())

		udlskey := rd.BuildKey(REDIS_USERMAILBOX_LS, u.GetReceiver())
		rd.Expire(udkey, USER_MAILBOX_EXPIRESEC)
		rd.Expire(udlskey, USER_MAILBOX_EXPIRESEC)
	}

	if u.MailGetMaxMail != lastMailGet {
		rd := g.GetRedis()
		udkey := rd.BuildKey(REDIS_USERMAILBOX_DATA, u.GetReceiver())

		u.MailGetMaxMail = lastMailGet
		rd.HSet(udkey, USERMAILBOX_DATA_LASTMAILGET, u.MailGetMaxMail)
	}

	u.SyncUserToken(token)

	u.SyncSystemMail()

	u.triggerNewMailNotify()
}

func (u *MailBoxRoleOfUser) triggerNewMailNotify() {
	if u.NewMailNotifyIid >= u.MaxMailIid || u.MailGetMaxMail >= u.MaxMailIid {
		return
	}

	//only notify once
	u.NewMailNotifyIid = u.MaxMailIid

	msg := &gpro.Mail_NewMailNtf{}

	g.SendMsgToRouter(msg)
}

func (u *MailBoxRoleOfUser) NewMail(mail *gpro.MailNormalItem) {
	if u.AddNewMail(mail) {
		u.totleNum++
		u.unreadNum++

		if mail.Iid > u.MaxMailIid {
			u.MaxMailIid = mail.Iid
		}
	}
}

func (u *MailBoxRoleOfUser) GetMailsFromBox(pro *protocolx.NetProtocol) {
	req := pro.Msg.(*gpro.Mail_MailGetReq)
	u.SyncUserToken(pro.GetUserToken())

	num := int(req.Num)
	if num <= 0 || num > 20 {
		num = 20
	}

	dats := u.queryMails(req.Mailiid, req.Next, num)
	var cts []*gpro.MailNormalItem
	if len(dats) > 0 {
		for _, d := range dats {
			cts = append(cts, u.cloneMailNormalItem(d))
		}
	}

	ack := &gpro.Mail_MailGetAck{}
	ack.Mails = cts
	ack.Totle = int32(u.totleNum)

	g.SendMsgToRouter(ack)
}

func (u *MailBoxRoleOfUser) ReadMail(token protocolx.UserToken, mailiid int64) {
	u.SyncUserToken(token)

	mail := u.getMail(mailiid)
	if mail == nil {
		return
	}

	if mail.Readtime <= 0 {
		mail.Readtime = utilc.GetTimeSecond()
		u.unreadNum--
	}
}

func (u *MailBoxRoleOfUser) DeleteMail(token protocolx.UserToken, mailiid int64) {
	u.SyncUserToken(token)

	mail := u.deleteOneMail(mailiid)
	if mail != nil {
		u.totleNum = len(u.mails.Mails)
		if mail.Readtime <= 0 {
			u.unreadNum--
		}
	}
}
