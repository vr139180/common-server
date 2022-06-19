package verification

import (
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/utilc"
)

type sessionTimeoutCheck struct {
	session   gnet.Conn
	startTime int64
}

func newSessionTimeoutCheck(c gnet.Conn) (s *sessionTimeoutCheck) {
	s = new(sessionTimeoutCheck)
	s.session = c
	s.startTime = utilc.GetTimestamp()

	return s
}

func (l *VerifyControl) RegistNewSession(c gnet.Conn) {
	defer l.cond.L.Unlock()
	l.cond.L.Lock()

	tc := newSessionTimeoutCheck(c)
	l.session2Wait[c] = tc
	l.waitList.Add(tc)
}

func (l *VerifyControl) RemoveNetSession(c gnet.Conn) {
	defer l.cond.L.Unlock()
	l.cond.L.Lock()

	d, ok := l.session2Wait[c]
	if !ok {
		return
	}

	delete(l.session2Wait, c)
	ind := l.waitList.IndexOf(d)
	if ind != -1 {
		l.waitList.Remove(ind)
	}
}

func (l *VerifyControl) registerTimer() {
	l.softTimer.AddTimer(3*1000, l.OnNetSessionTimeout)
}

func (l *VerifyControl) OnNetSessionTimeout(int64, int, int64) (finish bool) {
	//logic
	now := utilc.GetTimestamp()

	defer l.cond.L.Unlock()
	l.cond.L.Lock()

	rm := []*sessionTimeoutCheck{}
	totle := l.waitList.Size()
	for ind := 0; ind < totle; ind++ {
		d, ok := l.waitList.Get(ind)
		if !ok {
			return
		}
		s := d.(*sessionTimeoutCheck)
		if s.startTime+3000 > now {
			return
		}

		rm = append(rm, s)
	}

	if len(rm) > 0 {
		for _, c := range rm {
			ind := l.waitList.IndexOf(c)
			if ind == -1 {
				continue
			}

			l.waitList.Remove(ind)
			delete(l.session2Wait, c.session)

			c.session.Close()
		}

		logx.Debugf("login service check client not send login request:%d within 3 second.", len(rm))
	}

	return
}
