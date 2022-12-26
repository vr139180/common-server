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
	DLAddHead(obj IDoubleLink, n IDoubleLink)
	DLAddTail(obj IDoubleLink, n IDoubleLink)
	DLDelSelf(obj IDoubleLink)
}

// 简单封装,
type DoubleLinkBase struct {
}

//-------------------------IDoubleLink interface-------------------------------------

func (p *DoubleLinkBase) DLAddHead(obj IDoubleLink, n IDoubleLink) {
	pre := obj.DLGetPreObj()
	next := n.DLGetNextObj()

	if pre != nil {
		pre.DLSetNextObj(n)
	}
	n.DLSetPreObj(pre)

	n.DLSetNextObj(obj)
	obj.DLSetPreObj(n)

	if next != nil {
		next.DLSetPreObj(nil)
	}
}

func (p *DoubleLinkBase) DLAddTail(obj IDoubleLink, n IDoubleLink) {
	next := obj.DLGetNextObj()
	pre := n.DLGetPreObj()

	if next != nil {
		next.DLSetPreObj(n)
	}
	n.DLSetNextObj(next)

	obj.DLSetNextObj(n)
	n.DLSetPreObj(obj)

	if pre != nil {
		pre.DLSetNextObj(nil)
	}
}

func (p *DoubleLinkBase) DLDelSelf(obj IDoubleLink) {
	pre := obj.DLGetPreObj()
	next := obj.DLGetNextObj()

	if pre != nil {
		pre.DLSetNextObj(next)
	}
	if next != nil {
		next.DLSetPreObj(pre)
	}

	obj.DLSetPreObj(nil)
	obj.DLSetNextObj(nil)
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
		d.head.DLAddHead(d.head, e)
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
		d.tail.DLAddTail(d.tail, e)
		d.tail = e
	}
}

func (d *DoubleLinkUnSafe) DelElement(e IDoubleLink) {
	if e == nil {
		return
	}

	if d.head == e {
		n := d.head.DLGetNextObj()
		e.DLDelSelf(e)
		d.head = n

		if d.head == nil {
			d.tail = nil
		}

	} else {
		p := d.tail.DLGetPreObj()
		e.DLDelSelf(e)

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
