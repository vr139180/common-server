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
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"mailservice/g"
	"sync"
)

const (
	REDIS_SYSMAIL_DATA = "-MAIL:SYS:DATA"
	REDIS_SYSMAIL_LS   = "-MAIL:SYS:LS"
)

type MailBoxOfSystem struct {
	//存在的系统邮件
	mails gpro.MailSystemItems

	//同步的最后一个系统邮件
	lastMailIid int64

	bInited bool
	cond    *sync.Cond
}

func newSystemMailBox() (m *MailBoxOfSystem) {
	m = new(MailBoxOfSystem)
	m.cond = sync.NewCond(&sync.Mutex{})
	m.bInited = false
	m.lastMailIid = 0

	return
}

func (s *MailBoxOfSystem) IsInit() bool {
	return s.bInited
}

func (s *MailBoxOfSystem) GetLastMailIid() int64 {
	return s.lastMailIid
}

func (s *MailBoxOfSystem) LoadMailBox() {
	rdutil := g.GetRedis()

	//small to big
	kys, err1 := rdutil.GetZsetAllMOfKey(REDIS_SYSMAIL_LS)
	if err1 != nil {
		logx.Errorf("MailBoxOfSystem->LoadMailBox REDIS_SYSMAIL_LS redis error:%s", err1.Error())
		return
	}

	vls, err2 := rdutil.HGetAll(REDIS_SYSMAIL_DATA)
	if err2 != nil {
		logx.Errorf("MailBoxOfSystem->LoadMailBox REDIS_SYSMAIL_DATA redis error:%s", err2.Error())
		return
	}

	var ml gpro.MailSystemItems
	var maxiid int64
	maxiid = 0

	for _, k := range kys {
		v, ok := vls[k]
		if !ok {
			continue
		}
		si := &gpro.MailSystemItem{}
		err3 := utilc.BytesToProto([]byte(v), si)
		if err3 != nil {
			logx.Errorf("MailBoxOfSystem->LoadMailBox parse mailitem:%s error:%s", k, err3.Error())
			continue
		}

		ml.Mails = append(ml.Mails, si)
		maxiid = si.Iid
	}

	s.cond.L.Lock()
	defer s.cond.L.Unlock()

	if s.bInited && s.lastMailIid >= maxiid {
		return
	}

	//ml 是从小到大排列的
	for _, d := range ml.Mails {
		if d.Iid <= s.lastMailIid {
			continue
		}

		s.mails.Mails = append(s.mails.Mails, d)
		s.lastMailIid = d.Iid
	}

	logx.Infof("-------mail-system init success, num:%d maxiid:%d", len(s.mails.Mails), s.lastMailIid)

	s.bInited = true
}

func (s *MailBoxOfSystem) SyncMailBox() {
	if !s.IsInit() {
		return
	}

	rdutil := g.GetRedis()

	kys, err1 := rdutil.GetZsetGreatMOfKey(REDIS_SYSMAIL_LS, utilc.I64ToString(s.lastMailIid))
	if err1 != nil {
		logx.Errorf("MailBoxOfSystem->SyncMailBox REDIS_SYSMAIL_LS redis error:%s", err1.Error())
		return
	}

	var ml gpro.MailSystemItems
	var maxiid int64
	maxiid = 0

	for _, k := range kys {
		v, err3 := rdutil.HGet(REDIS_SYSMAIL_DATA, k)
		if err3 != nil {
			logx.Errorf("MailBoxOfSystem->SyncMailBox get mail:%s failed error:%s", k, err3.Error())
			continue
		}

		si := &gpro.MailSystemItem{}
		err3 = utilc.BytesToProto([]byte(v), si)
		if err3 != nil {
			logx.Errorf("MailBoxOfSystem->SyncMailBox parse mailitem:%s error:%s", k, err3.Error())
			continue
		}

		ml.Mails = append(ml.Mails, si)
		maxiid = si.Iid
	}

	s.cond.L.Lock()
	defer s.cond.L.Unlock()

	if s.lastMailIid >= maxiid {
		return
	}

	for _, d := range ml.Mails {
		if d.Iid <= s.lastMailIid {
			continue
		}

		s.mails.Mails = append(s.mails.Mails, d)
		s.lastMailIid = d.Iid
	}
}

//获取比id大的系统邮件
func (s *MailBoxOfSystem) GetSysMailsGreatIid(id int64) (dat []*gpro.MailSystemItem, maxiid int64, ok bool) {
	ok = false

	s.cond.L.Lock()
	defer s.cond.L.Unlock()

	if s.lastMailIid <= id {
		return
	}

	sind := -1
	for ind := len(s.mails.Mails) - 1; ind >= 0; ind-- {
		d := s.mails.Mails[ind]
		if d.Iid <= id {
			break
		}
		if sind == -1 {
			maxiid = d.Iid
		}
		sind = ind
	}

	if sind != -1 {
		dat = s.mails.Mails[sind:]
		ok = true
	}

	return
}
