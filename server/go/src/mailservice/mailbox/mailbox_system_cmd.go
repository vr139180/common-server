package mailbox

import (
	"cmslib/logx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"mailservice/data/db/entity"
	"mailservice/g"
	"time"

	"xorm.io/xorm"
)

type rdLoadSysMailCmd struct {
	ctrl *MailBoxCtrl
}

func newRDLoadSysMailCmd(ctrl *MailBoxCtrl) (c *rdLoadSysMailCmd) {
	c = new(rdLoadSysMailCmd)
	c.ctrl = ctrl

	return
}

//run in processor goroutine
func (c *rdLoadSysMailCmd) ProcessMail() {
	c.ctrl.sysMailBox.LoadMailBox()
}

//-----------------------------------------------------------------------------------------
type rdSyncSysMailCmd struct {
	ctrl *MailBoxCtrl
}

func newRDSyncSysMailCmd(ctrl *MailBoxCtrl) (c *rdSyncSysMailCmd) {
	c = new(rdSyncSysMailCmd)
	c.ctrl = ctrl

	return
}

func (c *rdSyncSysMailCmd) ProcessMail() {
	c.ctrl.sysMailBox.SyncMailBox()
}

//-----------------------------------------------------------------------------------------------------
type dbNewSysMailCmd struct {
	mailc *gpro.Mail_SystemMailReq

	mdata *gpro.MailSystemItem
}

func newDBNewSysMailCmd(p *gpro.Mail_SystemMailReq) (c *dbNewSysMailCmd) {
	c = new(dbNewSysMailCmd)
	c.mailc = p

	return
}

func (c *dbNewSysMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	bean := &entity.DBSysMail{}
	bean.Title = c.mailc.GetTitle()
	bean.Contents = c.mailc.GetContents()
	bean.Attachment = int8(c.mailc.GetAttachment())
	bean.AttachInfo = c.mailc.GetAttachinfo()
	bean.CreateTime = time.Now()

	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err2 := session.Begin()
		if err2 != nil {
			return nil, err2
		}

		_, err2 = session.Insert(bean)

		if err2 != nil {
			session.Rollback()
			return nil, err2
		}

		err2 = session.Commit()
		return nil, err2
	})

	if err != nil {
		logx.Errorf("new a system mail failed, err:%s", err.Error())

		ack := new(gpro.Mail_SystemMailAck)
		ack.Result = 1
		g.SendMsgToRouter(ack)
	} else {
		c.mdata = dbToRedisBeanOfSysMail(bean)

		logx.Debugf("new a system mail:%d success", bean.Iid)

		g.PostToSysMailProcessor(c)
	}
}

func (c *dbNewSysMailCmd) ProcessMail() {
	rdutil := g.GetRedis()

	ack := new(gpro.Mail_SystemMailAck)

	d := utilc.ProtoToBytes(c.mdata)
	if d != nil {
		sid := utilc.I64ToString(c.mdata.Iid)
		rdutil.HSet(REDIS_SYSMAIL_DATA, sid, d)
		rdutil.AddZset(REDIS_SYSMAIL_LS, sid, float64(c.mdata.Iid))

		ack.Result = 0
		ack.Mail = c.mdata
	} else {
		ack.Result = 1
	}

	g.SendMsgToRouter(ack)
}

//---------------------------------------------------------------------------------------
func dbToRedisBeanOfSysMail(db *entity.DBSysMail) (rd *gpro.MailSystemItem) {
	rd = new(gpro.MailSystemItem)

	rd.Iid = db.Iid
	rd.SenderIid = 0
	rd.Title = db.Title
	rd.Contents = db.Contents
	rd.Attachment = int32(db.Attachment)
	rd.Attachinfo = db.AttachInfo
	rd.Createtime = utilc.GetTimeSecond2(db.CreateTime)

	return
}
