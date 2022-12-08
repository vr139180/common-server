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

import (
	"chatservice/g"
	"cmslib/logx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

const hashmap_multipy = 2

type ChannelCtrl struct {
	//和processor处理loop对应的对象池
	//在loop goroutine中处理不需要考虑 同步的问题
	loopHolders []*ChannelHolder
	holder_num  int

	//各类channel iid映射到 对应processor loop的数据结构
	//对象数量为 holder_num的 hashmap_multipy 整数倍，每个数据结构 goroutine安全，
	//hashmap_multipy用来控制锁的数量
	hashkey_num int64
	hashKeys    []*hashKeyOfChannel
}

func NewChannelCtrl(loopnum int) (c *ChannelCtrl) {
	c = new(ChannelCtrl)
	c.init_ctrl(loopnum)

	return
}

func (cc *ChannelCtrl) init_ctrl(loopnum int) {
	cc.holder_num = loopnum
	for ind := 0; ind < cc.holder_num; ind++ {
		c := newChannelHolder(ind, cc)
		cc.loopHolders = append(cc.loopHolders, c)
	}

	cc.hashkey_num = int64(loopnum * hashmap_multipy)
	for ind := 0; ind < int(cc.hashkey_num); ind++ {
		c := newHashKeyOfChannel()
		cc.hashKeys = append(cc.hashKeys, c)
	}
}

func (cc *ChannelCtrl) getHashKeyOfChannelIid(cid int64) int {
	return int(cid % cc.hashkey_num)
}

func (cc *ChannelCtrl) getCanAssignLoop(t ChannelType) int {
	minval := 0
	cindex := 0

	for ind := 0; ind < cc.holder_num; ind++ {
		if ind == 0 {
			minval = cc.loopHolders[ind].GetChannelNumOfType(t)
			cindex = ind
			continue
		}

		nv := cc.loopHolders[ind].GetChannelNumOfType(t)
		if nv < minval {
			minval = nv
			cindex = ind
		}
	}

	return cindex
}

//return true: already exist false: not exist
func (cc *ChannelCtrl) getLoopFromChannelIid(t ChannelType, cid int64) (int, bool) {
	hkindex := cc.getHashKeyOfChannelIid(cid)
	hkc := cc.hashKeys[hkindex]
	ncid, ok := hkc.getLoopIndexByChannelId(t, cid)
	if !ok {
		//未绑定，需要进行绑定
		ncid = cc.getCanAssignLoop(t)
		//获取更新的邦定值
		ncid, ok = hkc.bindChannelIdToLoopIndex(t, cid, ncid)
		if ok {
			//调整balance的计算值
			cc.loopHolders[ncid].IncPreNum(t)
			return ncid, false
		}
	}

	return ncid, true
}

func (cc *ChannelCtrl) getLoopFromChannelIidMustExist(t ChannelType, cid int64) (int, bool) {
	hkindex := cc.getHashKeyOfChannelIid(cid)
	hkc := cc.hashKeys[hkindex]
	ncid, ok := hkc.getLoopIndexByChannelId(t, cid)

	return ncid, ok
}

func (cc *ChannelCtrl) UserActiveChannel(t ChannelType, cid int64, u *UserInfo) {
	cindex, exist := cc.getLoopFromChannelIid(t, cid)
	ch := cc.loopHolders[cindex]

	pp := new(UserActiveChannelProcessor)
	pp.ctype = t
	pp.cid = cid
	pp.channelNotExist = !exist
	pp.uinfo = u
	pp.ch = ch

	g.PostChatProcessor(cindex, pp)
}

func (cc *ChannelCtrl) UserSaySomthing(t ChannelType, cid int64, msg *gpro.Chat_UserMsgSay) {
	cindex, ok := cc.getLoopFromChannelIidMustExist(t, cid)
	if !ok {
		return
	}

	ch := cc.loopHolders[cindex]

	pp := new(UserSayProcessor)
	pp.ctype = t
	pp.cid = cid
	pp.msg = msg
	pp.ch = ch

	g.PostChatProcessor(cindex, pp)
}

func (cc *ChannelCtrl) DoChannelMaintance(loopIndex int) {
	ch := cc.loopHolders[loopIndex]
	tnow1 := utilc.GetTimestamp()
	ch.OneDayMaintance()
	tnow2 := utilc.GetTimestamp()

	logx.Infof("############## channel maintance loopindex:%d used times:%d milli############## ", loopIndex, (tnow2 - tnow1))
}

func (cc *ChannelCtrl) UnBindChannelId(t ChannelType, cids []int64) {
	if len(cids) == 0 {
		return
	}

	ckey := make(map[int][]int64)
	for _, id := range cids {
		hind := cc.getHashKeyOfChannelIid(id)
		dd, ok := ckey[hind]
		if !ok {
			dd = []int64{id}
			ckey[hind] = dd
		} else {
			dd = append(dd, id)
			ckey[hind] = dd
		}
	}

	for k, dd := range ckey {
		if len(dd) == 0 {
			continue
		}
		cc.hashKeys[k].unBindChannelId(t, dd)
	}
}
