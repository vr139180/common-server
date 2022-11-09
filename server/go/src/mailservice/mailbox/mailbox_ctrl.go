package mailbox

import (
	"cmslib/protocolx"
	"gamelib/protobuf/gpro"
	"mailservice/g"
)

type MailBoxCtrl struct {
	//和processor处理loop对应的对象池
	//在loop goroutine中处理不需要考虑 同步的问题
	loopHolders []*MailBoxHolder
	holder_num  int

	sysMailBox *MailBoxOfSystem
}

func NewMailBoxCtrl(loopnum int) (c *MailBoxCtrl) {
	c = new(MailBoxCtrl)
	c.init_ctrl(loopnum)

	return
}

func (cc *MailBoxCtrl) init_ctrl(loopnum int) {
	cc.holder_num = loopnum
	for ind := 0; ind < cc.holder_num; ind++ {
		c := newMailBoxHolder(ind, cc)
		cc.loopHolders = append(cc.loopHolders, c)
	}

	cc.sysMailBox = newSystemMailBox()
}

func (cc *MailBoxCtrl) getHashKeyOfReceiver(rid int64) int {
	return int(rid % int64(cc.holder_num))
}

//return true: already exist false: not exist
func (cc *MailBoxCtrl) getLoopFromReceiver(t MailBoxType, rid int64) int {
	return cc.getHashKeyOfReceiver(rid)
}

func (cc *MailBoxCtrl) DoMailBoxMaintance(loopIndex int) {
	ch := cc.loopHolders[loopIndex]
	//tnow1 := utilc.GetTimestamp()
	ch.OneDayMaintance()
	//tnow2 := utilc.GetTimestamp()
}

func (cc *MailBoxCtrl) LoadSystemMailBox() {
	cmd := newRDLoadSysMailCmd(cc)
	g.PostToSysMailProcessor(cmd)
}

func (cc *MailBoxCtrl) NotifyNewUserMail(mail *gpro.MailNormalItem) {
	receiver := mail.ReceiverIid
	lind := cc.getHashKeyOfReceiver(receiver)
	mh := cc.loopHolders[lind]
	mh.NewUserMail(mail)
}

//run in net goroutine
func (cc *MailBoxCtrl) ProcessNetCmd(pro *protocolx.NetProtocol) {
	id := pro.GetMsgId()
	if id == uint16(gpro.MAIL_PROTYPE_MAIL_SYSTEMMAIL_REQ) {
		cmd := newDBNewSysMailCmd(pro)
		g.PostDBProcessor(cmd)
	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_NEWMAIL_REQ) {
		cmd := newDBNewUserMailCmd(pro, cc)
		g.PostDBProcessor(cmd)
	} else if id == uint16(gpro.MAIL_PROTYPE_MAIL_SYSTEMMAIL_NTF) {
		cmd := newRDSyncSysMailCmd(cc)
		g.PostToSysMailProcessor(cmd)
	} else {
		switch int(id) {
		case int(gpro.MAIL_PROTYPE_MAIL_USERONLINE_ACTIVE):
		case int(gpro.MAIL_PROTYPE_MAIL_MAILGET_REQ):
		case int(gpro.MAIL_PROTYPE_MAIL_READMAIL_REQ):
		case int(gpro.MAIL_PROTYPE_MAIL_DELETEMAIL_REQ):
		case int(gpro.MAIL_PROTYPE_MAIL_UNBINDATTACHS_REQ):
			break
		default:
			return
		}

		uid := pro.GetTokenRoleIid()
		lind := cc.getHashKeyOfReceiver(uid)
		mh := cc.loopHolders[lind]
		cmd := newMBoxNetCmdHandle(pro, mh.OnNetCmdHanderl)
		g.PostMailProcessor(lind, cmd)
	}
}
