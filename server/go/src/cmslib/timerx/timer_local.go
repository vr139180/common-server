package timerx

import (
	"cmslib/utilc"
)

//-----------------------------------------------------------------------
type timerObject struct {
	beforeNode *timerObject
	nextNode   *timerObject

	tid      int64
	lastTime int64
	fun      TimerFunction
}

func newTimerObject(id int64, fun TimerFunction) (to *timerObject) {
	to = new(timerObject)
	to.tid = id
	to.fun = fun
	to.lastTime = utilc.GetTimestamp()

	return
}

func (to *timerObject) isTimeout(now int64, interval int) bool {
	if (to.lastTime + int64(interval)) > now {
		return false
	}

	to.lastTime = now
	return true
}

//-----------------------------------------------------------------------
type timerSequence struct {
	header *timerObject
	tail   *timerObject
	timers map[int64]*timerObject

	timeInverval int
	gid          int64
}

func newTimerSequence(step int) (s *timerSequence) {
	s = new(timerSequence)
	s.timeInverval = step
	s.gid = 1
	s.timers = make(map[int64]*timerObject)

	return
}

func (s *timerSequence) Release() {
	s.header = nil
	s.tail = nil
	s.timers = make(map[int64]*timerObject)
}

func (s *timerSequence) genTid() int64 {
	s.gid++
	return s.gid
}

func (s *timerSequence) addTimer(fun TimerFunction) (tid int64) {
	tid = s.genTid()
	tobj := newTimerObject(tid, fun)

	s.timers[tid] = tobj

	if s.header == nil {
		s.tail = tobj
		s.header = tobj
	} else {
		s.tail.nextNode = tobj
		tobj.beforeNode = s.tail
		s.tail = tobj
	}

	return
}

func (s *timerSequence) delTimer(tid int64) {
	tobj, ok := s.timers[tid]
	if !ok {
		return
	}

	delete(s.timers, tid)

	bobj := tobj.beforeNode
	nobj := tobj.nextNode

	if bobj != nil {
		bobj.nextNode = nobj
	}
	if nobj != nil {
		nobj.beforeNode = bobj
	}

	if s.header == tobj {
		s.header = nobj
	}
	if s.tail == tobj {
		s.tail = bobj
	}
}

func (s *timerSequence) tick(now int64) {
	cnt := len(s.timers)
	icount := 0

	for s.header != nil && icount < cnt {
		icount++

		if !s.header.isTimeout(now, s.timeInverval) {
			break
		}

		bfinish := false
		oldid := s.header.tid

		bfinish = s.header.fun(now, s.timeInverval, s.header.tid)

		if s.header == nil {
			break
		}

		if bfinish && s.header.tid == oldid {
			s.delTimer(s.header.tid)
			continue
		}

		if s.header != s.tail {
			nobj := s.header.nextNode
			nobj.beforeNode = nil
			s.header.nextNode = nil

			s.header.beforeNode = s.tail
			s.tail.nextNode = s.header

			s.tail = s.header
			s.header = nobj
		}
	}
}
