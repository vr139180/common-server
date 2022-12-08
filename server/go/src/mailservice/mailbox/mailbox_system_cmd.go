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
	"cmslib/logx"
	"cmslib/protocolx"
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
	mailc *protocolx.NetProtocol

	mdata *gpro.MailSystemItem
}

func newDBNewSysMailCmd(p *protocolx.NetProtocol) (c *dbNewSysMailCmd) {
	c = new(dbNewSysMailCmd)
	c.mailc = p

	return
}

func (c *dbNewSysMailCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	req := c.mailc.Msg.(*gpro.Mail_SystemMailReq)

	bean := &entity.DBSysMail{}
	bean.Title = req.GetTitle()
	bean.Contents = req.GetContents()
	bean.Attachment = int8(req.GetAttachment())
	bean.AttachInfo = req.GetAttachinfo()
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
