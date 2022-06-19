package mailbox

import (
	"cmslib/logx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"mailservice/data/db/entity"
	"mailservice/g"
	"time"

	"google.golang.org/protobuf/proto"
	"xorm.io/xorm"
)

//---------------------------------------------------------------------------------------
type dbNewUserMailCmd struct {
	mailc *gpro.Mail_NewMailReq

	ctrl *MailBoxCtrl

	mdata *gpro.MailNormalItem
}

func newDBNewUserMailCmd(p *gpro.Mail_NewMailReq, ctrl *MailBoxCtrl) (c *dbNewUserMailCmd) {
	c = new(dbNewUserMailCmd)
	c.mailc = p
	c.ctrl = ctrl

	return
}

func (c *dbNewUserMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	bean := &entity.DBUserMail{}
	bean.SType = c.mailc.Stype
	bean.SenderIid = c.mailc.SenderIid
	bean.ReceiverIid = c.mailc.ReceiverIid
	bean.Title = c.mailc.GetTitle()
	bean.Contents = c.mailc.GetContents()
	bean.Attachment = int8(c.mailc.GetAttachment())
	bean.AttachInfo = c.mailc.GetAttachinfo()
	bean.CreateTime = time.Now()

	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err2 := session.Begin()
		if err2 != nil {
			return nil, err2
		}

		affect, err2 := session.Insert(bean)

		if err2 != nil {
			session.Rollback()
			return nil, err2
		}

		if affect > 0 {
			result = 0
			c.mdata = dbToRedisBeanOfUserMail(bean)
		}

		err2 = session.Commit()
		return nil, err2
	})

	if err != nil {
		result = 1
		logx.Errorf("new a user mail failed, err:%s", err.Error())
	}

	if result == 0 {
		logx.Debugf("new a user mail:%d success", bean.Iid)
		g.PostToSysMailProcessor(c)
	}

	ack := &gpro.Mail_NewMailAck{Result: int32(result)}
	if c.mdata != nil {
		ack.Mail = c.mdata
	}
	g.PostToSysMailProcessor(c)
}

func (c *dbNewUserMailCmd) ProcessMail() {
	c.ctrl.NotifyNewUserMail(c.mdata)
}

//---------------------------------------------------------------------------------------
type dbLoadUserMailCmd struct {
	ubox   *entity.DBUserMailBox
	mitems []*gpro.MailNormalItem

	RoleIid int64
	holder  *MailBoxHolder
	token   *gpro.UserToken
	id      int
	pro     proto.Message
}

func newDBLoadUserMailCmd(roleiid int64, holder *MailBoxHolder, token *gpro.UserToken, id int, pro proto.Message) (c *dbLoadUserMailCmd) {
	c = new(dbLoadUserMailCmd)
	c.holder = holder
	c.RoleIid = roleiid
	c.token = token
	c.id = id
	c.pro = pro
	c.ubox = nil

	return
}

func (c *dbLoadUserMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		var mitems []entity.DBUserMail
		err1 = session.Where("receiver_iid = ?", c.RoleIid).Asc("iid").Find(&mitems)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if len(mitems) > 0 {
			for _, v := range mitems {
				rd := dbToRedisBeanOfUserMail(&v)
				c.mitems = append(c.mitems, rd)
			}
		}

		ubox := &entity.DBUserMailBox{RoleIid: c.RoleIid}
		ok, err1 := session.Get(ubox)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if !ok {
			ubox.SysMailSync = 0

			_, err1 = session.Insert(ubox)
			if err1 != nil {
				session.Rollback()
				return nil, err1
			}
		}

		c.ubox = ubox

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		logx.Errorf("dbLoadUserMailCmd - get user:%d mailbox failed, err:%s", c.RoleIid, err.Error())
		return
	}

	g.PostMailProcessor(c.holder.loopIndex, c)
}

func (c *dbLoadUserMailCmd) ProcessMail() {
	mbox := c.holder.CacheUserMailBox(c.RoleIid, c.ubox, c.mitems, c.token)
	if mbox != nil {
		c.holder.triggerNetProcess(mbox, c.id, c.pro)
	}
}

//---------------------------------------------------------------------------------------
type dbSaveSystemMailCmd struct {
	umails []*gpro.MailNormalItem

	mails       []*gpro.MailSystemItem
	syncMailIid int64

	RoleIid int64
	holder  *MailBoxHolder
}

func newDBSaveSystemMailCmd(mails []*gpro.MailSystemItem, mailiid int64, roleiid int64, h *MailBoxHolder) (c *dbSaveSystemMailCmd) {
	c = new(dbSaveSystemMailCmd)
	c.mails = mails
	c.syncMailIid = mailiid
	c.RoleIid = roleiid
	c.holder = h

	logx.Debugf("newDBSaveSystemMailCmd mails:%d", len(mails))

	return
}

func (c *dbSaveSystemMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return false, err1
		}

		for _, m := range c.mails {
			logx.Debugf("dbSaveSystemMailCmd -> save system mail:%d role:%d", m.Iid, c.RoleIid)
			smr := &entity.DBUserSysMail{RoleIid: c.RoleIid, SysMailIid: m.Iid}
			has, err2 := session.Exist(smr)
			if err2 != nil {
				session.Rollback()
				return false, err2
			}
			//如果已经获取则忽略
			if has {
				continue
			}

			_, err2 = session.Insert(smr)
			if err2 != nil {
				session.Rollback()
				return false, err2
			}

			dm := sysMailToDBUserMail(m, c.RoleIid)
			_, err2 = session.Insert(dm)
			if err2 != nil {
				session.Rollback()
				return false, err2
			}

			mx := dbToRedisBeanOfUserMail(dm)
			c.umails = append(c.umails, mx)
		}

		//修改user mailbox的sysmailsync字段，标识已经获取的系统邮件的最大值
		if len(c.umails) > 0 {
			mbox := &entity.DBUserMailBox{RoleIid: c.RoleIid, SysMailSync: c.syncMailIid}
			_, err2 := session.Where("sysmail_sync < ?", c.syncMailIid).Update(mbox)
			if err2 != nil {
				session.Rollback()
				return false, err2
			}
		}

		err1 = session.Commit()
		return true, err1
	})

	if err != nil {
		logx.Errorf("save system mails failed, err:%s", err.Error())
		return
	}

	g.PostMailProcessor(c.holder.loopIndex, c)
}

func (c *dbSaveSystemMailCmd) ProcessMail() {
	c.holder.CacheUserMailFromSystemMail(c.umails, c.syncMailIid, c.RoleIid)
}

//---------------------------------------------------------------------------------------
type dbReadMailCmd struct {
	mailiid int64
	RoleIid int64
	holder  *MailBoxHolder
	token   *gpro.UserToken
}

func newDBReadMailCmd(mid int64, roleiid int64, h *MailBoxHolder, token *gpro.UserToken) (c *dbReadMailCmd) {
	c = new(dbReadMailCmd)
	c.mailiid = mid
	c.RoleIid = roleiid
	c.holder = h
	c.token = token

	return
}

func (c *dbReadMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		mail := &entity.DBUserMail{Iid: c.mailiid, ReadTime: time.Now()}
		affect, err1 := session.Where("iid = ? and receiver_iid=? and readtime is null", c.mailiid, c.RoleIid).Cols("readtime").Update(mail)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if affect > 0 {
			result = 0
		} else {
			result = 3 //already read
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		result = 1
		logx.Errorf("read user:%d mail:%d failed, err:%s", c.RoleIid, c.mailiid, err.Error())
	}

	ack := &gpro.Mail_ReadMailAck{Result: int32(result), MailIid: c.mailiid}
	ack.Utoken = c.token
	g.SendMsgToRouter(ack)

	if result == 0 {
		g.PostMailProcessor(c.holder.loopIndex, c)
	}
}

func (c *dbReadMailCmd) ProcessMail() {
	c.holder.ReadMail(c.RoleIid, c.mailiid, c.token)
}

//---------------------------------------------------------------------------------------
type dbDeleteMailCmd struct {
	mailiid int64
	RoleIid int64
	holder  *MailBoxHolder
	token   *gpro.UserToken
}

func newDBDeleteMailCmd(mid int64, roleiid int64, h *MailBoxHolder, token *gpro.UserToken) (c *dbDeleteMailCmd) {
	c = new(dbDeleteMailCmd)
	c.mailiid = mid
	c.RoleIid = roleiid
	c.holder = h
	c.token = token

	return
}

func (c *dbDeleteMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		mail := &entity.DBUserMail{Iid: c.mailiid, ReceiverIid: c.RoleIid}
		affect, err1 := session.Delete(mail)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if affect > 0 {
			result = 0
		} else {
			result = 2
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		result = 1
		logx.Errorf("delete user:%d mail:%d failed, err:%s", c.RoleIid, c.mailiid, err.Error())
	}

	ack := &gpro.Mail_DeleteMailAck{Result: int32(result), MailIid: c.mailiid}
	ack.Utoken = c.token

	g.SendMsgToRouter(ack)

	if result == 0 {
		g.PostMailProcessor(c.holder.loopIndex, c)
	}
}

func (c *dbDeleteMailCmd) ProcessMail() {
	c.holder.DeleteMail(c.RoleIid, c.mailiid, c.token)
}

//---------------------------------------------------------------------------------------
func sysMailToDBUserMail(s *gpro.MailSystemItem, roleiid int64) (d *entity.DBUserMail) {
	d = new(entity.DBUserMail)

	d.SType = int32(MailTypeUnion_Index)
	d.SenderIid = s.SenderIid
	d.ReceiverIid = roleiid
	d.Attachment = int8(s.Attachment)
	d.AttachInfo = s.Attachinfo
	d.Title = s.Title
	d.Contents = s.Contents
	d.CreateTime = utilc.SecondToTime(s.Createtime)

	return
}

func dbToRedisBeanOfUserMail(db *entity.DBUserMail) (rd *gpro.MailNormalItem) {
	rd = new(gpro.MailNormalItem)

	rd.Iid = db.Iid
	rd.Stype = db.SType
	rd.SenderIid = db.SenderIid
	rd.ReceiverIid = db.ReceiverIid
	rd.Attachment = int32(db.Attachment)
	rd.Attachinfo = db.AttachInfo
	rd.Title = db.Title
	rd.Contents = db.Contents
	rd.Createtime = utilc.GetTimeSecond2(db.CreateTime)
	rd.Readtime = utilc.GetTimeSecond2(db.ReadTime)

	return
}
