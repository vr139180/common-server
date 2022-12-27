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
	"cmslib/datas"
	"cmslib/protocolx"
	"cmslib/utilc"
	"fmt"
	"friendservice/g"
	"gamelib/protobuf/gpro"
	"gamelib/service"
	"strings"

	"google.golang.org/protobuf/proto"
)

const (
	//7 days
	USER_FRIENDSHOME_EXPIRESEC = 60 * 60 * 24 * 7
	//redis key
	REDIS_USERFRIENDHOME = "-FRD:FHOME:%d"
	//data properties
	USERFRIENDHOME_FRDLIST = "frd:%d"
	USERFRIENDHOME_IVTLIST = "ivt:%d"

	USERFRIENDHOME_PLACEHOLDER = "+"

	//30 min
	USER_FRIENDSINFO_EXPIRESEC = 60 * 30
	REDIS_USERINFO             = "-FRD:USER:%d"
)

//--------------------------------------------------------------------------------------------
type friendUserInfo struct {
	datas.DoubleLinkBase

	//好友信息
	data *gpro.FriendRelation
	//token
	online *gpro.FriendUserInfo

	//IDoubleLink members
	preObj  datas.IDoubleLink
	nextObj datas.IDoubleLink
}

//-----------------------------IDoubleLink----------------------------------------
func (p *friendUserInfo) DLGetPreObj() (n datas.IDoubleLink) {
	n = p.preObj
	return
}

func (p *friendUserInfo) DLSetPreObj(n datas.IDoubleLink) {
	p.preObj = n
}

func (p *friendUserInfo) DLGetNextObj() (n datas.IDoubleLink) {
	n = p.nextObj
	return
}

func (p *friendUserInfo) DLSetNextObj(n datas.IDoubleLink) {
	p.nextObj = n
}

//-------------------------------------------------------------------------------

func newFriendUserInfo(d *gpro.FriendRelation) (fi *friendUserInfo) {
	fi = new(friendUserInfo)
	fi.data = d
	fi.data.Online = false
	fi.online = nil

	return
}

func (fi *friendUserInfo) GetIid() int64 {
	return fi.data.Iid
}

func (fi *friendUserInfo) GetFriendIid() int64 {
	return fi.data.Friendiid
}

func (fi *friendUserInfo) GetRedisKey() string {
	return fmt.Sprintf(REDIS_USERINFO, fi.GetFriendIid())
}

func (fi *friendUserInfo) IsOnline() bool {
	return fi.online != nil
}

func (fi *friendUserInfo) SyncFriendInfo(data []byte) {
	if len(data) == 0 {
		fi.online = nil
		fi.data.Online = fi.IsOnline()
		return
	}

	if fi.online == nil {
		fi.online = new(gpro.FriendUserInfo)
	}

	err := utilc.BytesToProto(data, fi.online)
	if err != nil {
		fi.online = nil
	}
	fi.data.Online = fi.IsOnline()
}

//--------------------------------------------------------------------------------------------

type FriendHome struct {
	datas.DoubleLinkBase

	//key: roleiid
	friendDatas map[int64]*friendUserInfo
	friendLink  *datas.DoubleLinkUnSafe
	//iid,not friendiid
	maxfriend    int64
	totleFriends int

	//邀请
	invites   []*gpro.FriendInviteItem
	maxinvite int64

	//上次激活时间
	lastUpdate int64

	//user info
	inited bool

	//user token head
	userToken protocolx.UserToken
	//接收方
	RoleIid int64

	ch *FriendsHolder

	//IDoubleLink members
	preObj  datas.IDoubleLink
	nextObj datas.IDoubleLink
}

//-----------------------------IDoubleLink----------------------------------------
func (p *FriendHome) DLGetPreObj() (n datas.IDoubleLink) {
	n = p.preObj
	return
}

func (p *FriendHome) DLSetPreObj(n datas.IDoubleLink) {
	p.preObj = n
}

func (p *FriendHome) DLGetNextObj() (n datas.IDoubleLink) {
	n = p.nextObj
	return
}

func (p *FriendHome) DLSetNextObj(n datas.IDoubleLink) {
	p.nextObj = n
}

//-------------------------------------------------------------------------------

func newFriendHome(roleiid int64, ch *FriendsHolder) (f *FriendHome) {
	f = new(FriendHome)

	f.inited = false
	f.ch = ch
	f.RoleIid = roleiid
	f.lastUpdate = utilc.GetTimestamp()

	f.friendDatas = make(map[int64]*friendUserInfo)
	f.friendLink = datas.NewDoubleLinkUnSafe()

	return
}

func initFriendHomeFromRedis(roleiid int64, shead protocolx.UserToken, ch *FriendsHolder) (u *FriendHome, success bool) {
	success = false
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERFRIENDHOME, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	dats, err := rd.HGetAll(udkey)
	if err != nil {
		return
	}

	var frds []*gpro.FriendRelation
	var invites []*gpro.FriendInviteItem

	for k, v := range dats {
		if strings.HasPrefix(k, "frd:") {
			item := &gpro.FriendRelation{}
			err = utilc.BytesToProto([]byte(v), item)
			if err != nil {
				continue
			}

			frds = append(frds, item)

		} else if strings.HasPrefix(k, "ivt:") {
			item := &gpro.FriendInviteItem{}
			err = utilc.BytesToProto([]byte(v), item)
			if err != nil {
				continue
			}

			invites = addInviteToSort(invites, item)
		}
	}

	u = newFriendHome(roleiid, ch)
	u.InitFriendHome(frds, invites, shead)

	success = true
	return
}

func (f *FriendHome) InitFriendHome(frds []*gpro.FriendRelation, invites []*gpro.FriendInviteItem, shead protocolx.UserToken) {
	if f.inited {
		return
	}
	f.inited = true

	f.invites = invites
	if len(f.invites) > 0 {
		f.maxinvite = f.invites[len(f.invites)-1].Iid
	} else {
		f.maxinvite = 0
	}

	if len(frds) > 0 {
		for _, d := range frds {
			fi := newFriendUserInfo(d)
			f.friendDatas[fi.GetFriendIid()] = fi
			f.friendLink.AddTailElement(fi)
			if f.maxfriend < d.Iid {
				f.maxfriend = d.Iid
			}
		}
	}
	f.totleFriends = len(f.friendDatas)

	f.saveAllToRedis()

	f.SyncUserToken(shead)
}

func (f *FriendHome) saveAllToRedis() {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERFRIENDHOME, f.RoleIid)
	ok, err := rd.KeyExist(udkey)
	//redis中存在
	if err != nil || ok {
		return
	}

	//占位数据
	rd.HSet(udkey, USERFRIENDHOME_PLACEHOLDER, USERFRIENDHOME_PLACEHOLDER)

	//save items
	for _, d := range f.friendDatas {
		dat := utilc.ProtoToBytes(d.data)
		if dat == nil {
			continue
		}

		k1 := rd.BuildKey(USERFRIENDHOME_FRDLIST, d.GetFriendIid())
		rd.HSet(udkey, k1, dat)
	}

	//save invites
	if len(f.invites) > 0 {
		for _, d := range f.invites {
			dat := utilc.ProtoToBytes(d)
			if dat == nil {
				continue
			}

			k1 := rd.BuildKey(USERFRIENDHOME_IVTLIST, d.Iid)
			rd.HSet(udkey, k1, dat)
		}

	}

	rd.Expire(udkey, USER_FRIENDSHOME_EXPIRESEC)
}

func (f *FriendHome) SyncUserToken(token protocolx.UserToken) {
	f.userToken = token

	/*
		rd := g.GetRedis()
		udkey := rd.BuildKey(REDIS_USERINFO, f.RoleIid)
		dat := utilc.ProtoToBytes(&f.userToken)
		if dat != nil {
			rd.AddEX(udkey, dat, USER_FRIENDSINFO_EXPIRESEC)
		}
	*/
}

func (f *FriendHome) SendNetProtocol(msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)
	head := pro.WriteHead()
	head.Token = f.userToken
	head.RoleId = f.RoleIid
	head.ToType = int8(service.ServiceType_Gate)
	head.FromType = int8(service.ServiceType_Friend)

	g.SendNetToRouter(pro)
}

func (f *FriendHome) SendNetProtocolTo(gto service.ServiceType, msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)

	head := pro.WriteHead()
	head.Token = f.userToken
	head.RoleId = f.RoleIid
	head.ToType = int8(gto)
	head.FromType = int8(service.ServiceType_Friend)

	g.SendNetToRouter(pro)
}

func (f *FriendHome) cloneFriendInvite(iv *gpro.FriendInviteItem) *gpro.FriendInviteItem {
	return (proto.Clone(iv)).(*gpro.FriendInviteItem)
}

func (f *FriendHome) cloneFriendRelation(iv *gpro.FriendRelation) *gpro.FriendRelation {
	return (proto.Clone(iv)).(*gpro.FriendRelation)
}

func (f *FriendHome) NewFriendInvite(invite *gpro.FriendInviteItem) {
	f.invites = addInviteToSort(f.invites, invite)
	if f.maxinvite < invite.Iid {
		f.maxinvite = invite.Iid
	}
}

func (f *FriendHome) getFriendInvite(iid int64) *gpro.FriendInviteItem {
	if len(f.invites) > 0 {
		for _, d := range f.invites {
			if d.Iid == iid {
				return d
			}
		}
	}

	return nil
}

func (f *FriendHome) deleteFriendInvite(iid int64) {
	mlen := len(f.invites)
	if mlen > 0 {
		for i, d := range f.invites {
			if d.Iid == iid {
				copy(f.invites[i:], f.invites[i+1:])
				f.invites = f.invites[:mlen-1]
				return
			}
		}
	}
}

func (f *FriendHome) getFriendBy(friendiid int64) *friendUserInfo {
	fi, exist := f.friendDatas[friendiid]
	if !exist {
		return nil
	}

	return fi
}

func (f *FriendHome) InviteConfirmA(iid int64, agree bool, shead protocolx.UserToken) {

	invite := f.getFriendInvite(iid)
	if invite == nil {
		ack := &gpro.Frd_InviteConfirmAck{Iid: iid, Agree: agree, Result: 2}
		f.SendNetProtocol(ack)
		return
	}

	cmd := newDBInviteConfirmCmd(f.RoleIid, invite.FromIid, iid, agree, f.ch, shead)
	g.PostDBProcessor(cmd)
}

func (f *FriendHome) InviteConfirmB(inviteiid int64, agree bool, relation *gpro.FriendRelation, shead protocolx.UserToken) {
	f.deleteFriendInvite(inviteiid)

	if agree {
		if f.getFriendBy(relation.Friendiid) != nil {
			return
		}

		fi := newFriendUserInfo(relation)

		f.friendDatas[fi.GetFriendIid()] = fi
		f.friendLink.AddHeadElement(fi)
		if f.maxfriend < relation.Iid {
			f.maxfriend = relation.Iid
		}
		f.totleFriends = len(f.friendDatas)
	}
}

func (f *FriendHome) FriendChangeNotify(friendiid int64, addordel bool, relation *gpro.FriendRelation) {
	if addordel {
		fi := f.getFriendBy(friendiid)
		if fi != nil {
			return
		}

		fi = newFriendUserInfo(relation)

		f.friendDatas[fi.GetFriendIid()] = fi
		f.friendLink.AddHeadElement(fi)
		if f.maxfriend < relation.Iid {
			f.maxfriend = relation.Iid
		}
		f.totleFriends = len(f.friendDatas)
	} else {
		fi := f.getFriendBy(friendiid)
		if fi == nil {
			return
		}

		delete(f.friendDatas, friendiid)
		f.friendLink.DelElement(fi)
		f.totleFriends = len(f.friendDatas)
	}
}

func (f *FriendHome) DeleteFriendA(frdiid int64, shead protocolx.UserToken) {

	fi := f.getFriendBy(frdiid)
	if fi == nil {
		ack := &gpro.Frd_FriendDeleteAck{Friendiid: frdiid, Result: 2}
		f.SendNetProtocol(ack)
		return
	}

	cmd := newDBFriendDeleteCmd(f.RoleIid, frdiid, fi.GetIid(), f.ch, shead)
	g.PostDBProcessor(cmd)
}

func (f *FriendHome) DeleteFriendB(friendiid int64) {
	fi := f.getFriendBy(friendiid)
	if fi == nil {
		return
	}

	delete(f.friendDatas, friendiid)
	f.friendLink.DelElement(fi)
	f.totleFriends = len(f.friendDatas)
}

func (f *FriendHome) QueryFriends(req *gpro.Frd_FriendListReq, shead protocolx.UserToken) {
	f.SyncUserToken(shead)

	ack := new(gpro.Frd_FriendListAck)
	ack.Next = req.Next

	//----invite----
	ivtlen := len(f.invites)
	ivind := -1
	for i := ivtlen - 1; i >= 0; i-- {
		d := f.invites[i]
		if d.Iid > req.LastInvite {
			continue
		}
		if d.Iid <= req.LastInvite {
			ivind = i
			break
		}
	}

	if ivind <= 0 {
		ivind = 0
	}

	cnt := req.Num
	if cnt < 1 || cnt > 20 {
		cnt = 20
	}
	for i := ivind; i < ivtlen && cnt > 0; i++ {
		cnt--
		ack.Invites = append(ack.Invites, f.cloneFriendInvite(f.invites[i]))
	}
	ack.Totleinvite = int32(ivtlen)

	//-----------friends------------------
	frdlen := len(f.friendDatas)
	ack.Totlefriend = int32(frdlen)

	bind := int(req.Friendindex)
	if bind >= frdlen || bind < 0 {
		bind = 0
		ack.Next = true
	}

	ind := -1
	var dindex datas.IDoubleLink
	for d := f.friendLink.GetHeadElement(); d != nil; d = d.DLGetNextObj() {
		ind++
		if bind == ind {
			dindex = d
			break
		}
	}

	if dindex == nil {
		dindex = f.friendLink.GetHeadElement()
	}

	cnt = req.Num
	if cnt < 1 || cnt > 20 {
		cnt = 20
	}
	var frds []*friendUserInfo
	for dindex != nil && cnt > 0 {
		d := dindex.(*friendUserInfo)
		frds = append(frds, d)

		cnt--
		if ack.Next {
			dindex = dindex.DLGetNextObj()
		} else {
			dindex = dindex.DLGetPreObj()
		}
	}

	if len(frds) > 0 {
		keys := make(map[string]*friendUserInfo)
		for _, d := range frds {
			keys[d.GetRedisKey()] = d
		}

		f.syncFriendUserInfo(keys)

		for _, d := range frds {
			ack.Friends = append(ack.Friends, f.cloneFriendRelation(d.data))
		}
	}

	f.SendNetProtocol(ack)
}

func (f *FriendHome) syncFriendUserInfo(keys map[string]*friendUserInfo) {
	kk := make([]string, 0, len(keys))
	for k := range keys {
		kk = append(kk, k)
	}

	rd := g.GetRedis()
	dat, err := rd.GetKeys(kk)
	if err != nil {
		return
	}

	for k, v := range dat {
		fi, exist := keys[k]
		if !exist {
			continue
		}

		if v == nil {
			fi.SyncFriendInfo(nil)
		} else {
			sv := v.(string)
			fi.SyncFriendInfo([]byte(sv))
		}
	}
}

//--------------------------------------------------------------------------------------
func addInviteToSort(dat []*gpro.FriendInviteItem, iv *gpro.FriendInviteItem) []*gpro.FriendInviteItem {
	if len(dat) == 0 {
		dat = append(dat, iv)
		return dat
	}

	ind := -1
	for i := len(dat) - 1; i >= 0; i-- {
		d := dat[i]
		if d.Iid > iv.Iid {
			continue
		} else if d.Iid == iv.Iid {
			d.Invitetime = iv.Invitetime
			return dat
		} else {
			ind = i
			ind++
			if ind >= len(dat) {
				ind = -1
			}
			break
		}
	}

	if ind == -1 {
		dat = append(dat, iv)
	} else {
		dat = append(dat, nil)
		copy(dat[ind+1:], dat[ind:])
		dat[ind] = iv
	}

	return dat
}
