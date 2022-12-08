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


package channel

import "sync"

type hashKeyOfType struct {
	//映射关系
	cid2cindex map[int64]int
}

func (ht *hashKeyOfType) getLoopIndexByChannelId(cid int64) (int, bool) {
	v, ok := ht.cid2cindex[cid]
	return v, ok
}

func (ht *hashKeyOfType) unBindChannelId(cids []int64) {
	if len(cids) == 0 {
		return
	}

	for _, v := range cids {
		delete(ht.cid2cindex, v)
	}
}

func (ht *hashKeyOfType) bindChannelIdToLoopIndex(cid int64, cindex int) (int, bool) {

	v, ok := ht.cid2cindex[cid]
	if ok {
		return v, false
	}

	ht.cid2cindex[cid] = cindex

	return cindex, true
}

//hash之后存放 channelid和processor.loop的映射管理，goroutine安全
type hashKeyOfChannel struct {
	clock *sync.Cond

	//长度为ChannelTypeMax_Index的数组
	ctypes []*hashKeyOfType
}

func newHashKeyOfChannel() (hk *hashKeyOfChannel) {
	hk = new(hashKeyOfChannel)
	hk.clock = sync.NewCond(&sync.Mutex{})

	for ind := ChannelTypeGlobal_Indx; ind < ChannelTypeMax_Index; ind++ {
		d := new(hashKeyOfType)
		d.cid2cindex = make(map[int64]int)

		hk.ctypes = append(hk.ctypes, d)
	}

	return
}

func (hk *hashKeyOfChannel) getLoopIndexByChannelId(t ChannelType, cid int64) (int, bool) {
	if t <= ChannelTypeInvalide_index || t >= ChannelTypeMax_Index {
		return 0, false
	}

	defer func() {
		hk.clock.L.Unlock()
	}()

	hk.clock.L.Lock()

	return hk.ctypes[t].getLoopIndexByChannelId(cid)
}

//绑定channelid to processor.loopindex
//return true 返回cindex false返回已经存在的cindex
func (hk *hashKeyOfChannel) bindChannelIdToLoopIndex(t ChannelType, cid int64, cindex int) (int, bool) {

	defer func() {
		hk.clock.L.Unlock()
	}()

	hk.clock.L.Lock()

	return hk.ctypes[t].bindChannelIdToLoopIndex(cid, cindex)
}

func (hk *hashKeyOfChannel) unBindChannelId(t ChannelType, cids []int64) {
	if len(cids) == 0 {
		return
	}
	if t <= ChannelTypeInvalide_index || t >= ChannelTypeMax_Index {
		return
	}

	defer func() {
		hk.clock.L.Unlock()
	}()

	hk.clock.L.Lock()

	hk.ctypes[t].unBindChannelId(cids)
}
