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


package friends

import (
	"friendservice/g"
	"friendservice/xinf"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

type FriendsCtrl struct {
	//和processor处理loop对应的对象池
	//在loop goroutine中处理不需要考虑 同步的问题
	loopHolders []*FriendsHolder
	holder_num  int
}

func NewFriendsCtrl(loopnum int) (c *FriendsCtrl) {
	c = new(FriendsCtrl)
	c.init_ctrl(loopnum)

	return
}

func (cc *FriendsCtrl) init_ctrl(loopnum int) {
	cc.holder_num = loopnum
	for ind := 0; ind < cc.holder_num; ind++ {
		c := newFriendsHolder(ind, cc)
		cc.loopHolders = append(cc.loopHolders, c)
	}
}

func (cc *FriendsCtrl) getHashKeyOfRole(rid int64) int {
	return int(rid % int64(cc.holder_num))
}

func (cc *FriendsCtrl) DoFriendsMaintance(loopIndex int) {
	ch := cc.loopHolders[loopIndex]
	//tnow1 := utilc.GetTimestamp()
	ch.OneDayMaintance()
	//tnow2 := utilc.GetTimestamp()
}

//run in net goroutine
func (cc *FriendsCtrl) ProcessNetCmd(id int, pro proto.Message) {
	if id == int(gpro.FRIEND_PROTYPE_FRD_FRIENDINVITE_REQ) {
		msg := pro.(*gpro.Frd_FriendInviteReq)

		_, roleiid := xinf.ParseUserGate(uint64(msg.Utoken.GetGiduid()))
		//不能自己邀请自己
		if roleiid == msg.InviteIid {
			return
		}

		//invite将会被派发到被邀请人的goroutine去
		lind := cc.getHashKeyOfRole(msg.InviteIid)
		mh := cc.loopHolders[lind]
		cmd := newDBInviteFriendCmd(msg.InviteIid, roleiid, mh, msg.Utoken)
		g.PostDBProcessor(cmd)
	} else if id == int(gpro.FRIEND_PROTYPE_FRD_FRIENDCHANGEOTHER_NTF) {
		msg := pro.(*gpro.Frd_FriendChangeOtherNtf)
		lind := cc.getHashKeyOfRole(msg.NotifyRoleiid)
		mh := cc.loopHolders[lind]
		cmd := newFriendNetCmdHandle(nil, id, pro, mh.OnFriendOtherChangeNotify)
		g.PostFriendProcessor(lind, cmd)
	} else {
		var ut *gpro.UserToken
		switch id {
		case int(gpro.FRIEND_PROTYPE_FRD_INVITECONFIRM_REQ):
			msg := pro.(*gpro.Frd_InviteConfirmReq)
			ut = msg.Utoken
		case int(gpro.FRIEND_PROTYPE_FRD_FRIENDDELETE_REQ):
			msg := pro.(*gpro.Frd_FriendDeleteReq)
			ut = msg.Utoken
		case int(gpro.FRIEND_PROTYPE_FRD_FRIENDLIST_REQ):
			msg := pro.(*gpro.Frd_FriendListReq)
			ut = msg.Utoken
		}

		if ut == nil {
			return
		}

		_, uid := xinf.ParseUserGate(uint64(ut.GetGiduid()))
		lind := cc.getHashKeyOfRole(uid)
		mh := cc.loopHolders[lind]
		cmd := newFriendNetCmdHandle(ut, id, pro, mh.OnNetCmdHander)
		g.PostFriendProcessor(lind, cmd)
	}
}
