package datas

import (
	"sync"
)

// 双向链表接口
type IDoubleLink interface {
	//单元重置
	DLGetPreObj() (n IDoubleLink)
	DLSetPreObj(n IDoubleLink)
	DLGetNextObj() (n IDoubleLink)
	DLSetNextObj(n IDoubleLink)
	DLAddHead(n IDoubleLink)
	DLAddTail(n IDoubleLink)
	DLDelSelf()
}

// 简单封装,
type DoubleLinkBase struct {
	//IDoubleLink members
	preObj  IDoubleLink
	nextObj IDoubleLink
}

//-------------------------IDoubleLink interface-------------------------------------
func (p *DoubleLinkBase) DLGetPreObj() (n IDoubleLink) {
	n = p.preObj
	return
}

func (p *DoubleLinkBase) DLSetPreObj(n IDoubleLink) {
	p.preObj = n
}

func (p *DoubleLinkBase) DLGetNextObj() (n IDoubleLink) {
	n = p.nextObj
	return
}

func (p *DoubleLinkBase) DLSetNextObj(n IDoubleLink) {
	p.nextObj = n
}

func (p *DoubleLinkBase) DLAddHead(n IDoubleLink) {
	pre := p.preObj
	next := n.DLGetNextObj()

	if pre != nil {
		pre.DLSetNextObj(n)
	}
	n.DLSetPreObj(pre)

	n.DLSetNextObj(p)
	p.DLSetPreObj(n)

	if next != nil {
		next.DLSetPreObj(nil)
	}
}

func (p *DoubleLinkBase) DLAddTail(n IDoubleLink) {
	next := p.nextObj
	pre := n.DLGetPreObj()

	if next != nil {
		next.DLSetPreObj(n)
	}
	n.DLSetNextObj(next)

	p.DLSetNextObj(n)
	n.DLSetPreObj(p)

	if pre != nil {
		pre.DLSetNextObj(nil)
	}
}

func (p *DoubleLinkBase) DLDelSelf() {
	pre := p.preObj
	next := p.nextObj

	if pre != nil {
		pre.DLSetNextObj(next)
	}
	if next != nil {
		next.DLSetPreObj(pre)
	}

	p.preObj = nil
	p.nextObj = nil
}

//--------------------------------------------------------------------------------------
type DoubleLinkUnSafe struct {
	head IDoubleLink
	tail IDoubleLink
}

func NewDoubleLinkUnSafe() (d *DoubleLinkUnSafe) {
	d = new(DoubleLinkUnSafe)
	d.head = nil
	d.tail = nil

	return
}

func (d *DoubleLinkUnSafe) ElementMoveToHead(e IDoubleLink) {
	if e == nil {
		return
	}

	if d.head == d.tail {
		return
	}
	if e == d.head {
		return
	}

	d.DelElement(e)
	d.AddHeadElement(e)
}

func (d *DoubleLinkUnSafe) AddHeadElement(e IDoubleLink) {
	if e == nil {
		return
	}

	if d.head == nil {
		d.head = e
		d.tail = d.head
	} else {
		d.head.DLAddHead(e)
		d.head = e
	}
}

func (d *DoubleLinkUnSafe) AddTailElement(e IDoubleLink) {
	if e == nil {
		return
	}

	if d.tail == nil {
		d.head = e
		d.tail = e
	} else {
		d.tail.DLAddTail(e)
		d.tail = e
	}
}

func (d *DoubleLinkUnSafe) DelElement(e IDoubleLink) {
	if e == nil {
		return
	}

	if d.head == e {
		n := d.head.DLGetNextObj()
		e.DLDelSelf()
		d.head = n

		if d.head == nil {
			d.tail = nil
		}

	} else {
		p := d.tail.DLGetPreObj()
		e.DLDelSelf()

		if d.tail == e {
			d.tail = p
		}
	}

}

func (d *DoubleLinkUnSafe) ClearAllElement() {

	for {
		if d.head == nil {
			break
		}

		d.DelElement(d.head)
	}
}

func (d *DoubleLinkUnSafe) PopupHeadElement() IDoubleLink {

	if d.head == nil {
		return nil
	} else if d.head == d.tail {
		r := d.head
		d.head = nil
		d.tail = nil
		return r
	}

	r := d.head
	r.DLSetNextObj(nil)

	n := d.head.DLGetNextObj()
	n.DLSetPreObj(nil)

	d.head = n

	return r
}

func (d *DoubleLinkUnSafe) PopupTailElement() IDoubleLink {

	if d.tail == nil {
		return nil
	} else if d.head == d.tail {
		r := d.tail
		d.head = nil
		d.tail = nil
		return r
	}

	r := d.tail
	r.DLSetPreObj(nil)

	n := d.tail.DLGetPreObj()
	n.DLSetNextObj(nil)

	d.tail = n

	return r
}

func (d *DoubleLinkUnSafe) GetTailElement() IDoubleLink {
	return d.tail
}

func (d *DoubleLinkUnSafe) GetHeadElement() IDoubleLink {
	return d.head
}

//--------------------------------------------------------------------------------------
// coroutine安全的双向链表
type DoubleLinkSafe struct {
	DoubleLinkUnSafe
	//head IDoubleLink
	//tail IDoubleLink

	cond *sync.Cond
}

func NewDoubleLinkSafe() (d *DoubleLinkSafe) {
	d = new(DoubleLinkSafe)
	d.cond = sync.NewCond(&sync.Mutex{})
	d.head = nil
	d.tail = nil

	return
}

func (d *DoubleLinkSafe) ElementMoveToHead(e IDoubleLink) {
	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	d.DoubleLinkUnSafe.ElementMoveToHead(e)
}

func (d *DoubleLinkSafe) AddHeadElement(e IDoubleLink) {
	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	d.DoubleLinkUnSafe.AddHeadElement(e)
}

func (d *DoubleLinkSafe) AddTailElement(e IDoubleLink) {
	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	d.DoubleLinkUnSafe.AddTailElement(e)
}

func (d *DoubleLinkSafe) DelElement(e IDoubleLink) {
	defer d.cond.L.Unlock()

	d.cond.L.Lock()

	d.DoubleLinkUnSafe.DelElement(e)
}

func (d *DoubleLinkSafe) ClearAllElement() {

	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	d.DoubleLinkUnSafe.ClearAllElement()
}

func (d *DoubleLinkSafe) PopupHeadElement() IDoubleLink {
	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	return d.DoubleLinkUnSafe.PopupHeadElement()
}

func (d *DoubleLinkSafe) PopupTailElement() IDoubleLink {
	defer d.cond.L.Unlock()
	d.cond.L.Lock()

	return d.DoubleLinkUnSafe.PopupTailElement()
}
