package friends

import (
	"cmslib/datas"
	"cmslib/logx"
	"cmslib/utilc"
	"friendservice/g"
	"friendservice/xinf"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

//--------------------------------------------------------------------------------------------
type FRIENDHANDLER func(*gpro.UserToken, int, proto.Message)

type friendNetCmdHandler struct {
	iid   int
	pro   proto.Message
	token *gpro.UserToken
	f     FRIENDHANDLER
}

func newFriendNetCmdHandle(token *gpro.UserToken, iid int, pro proto.Message, f FRIENDHANDLER) (c *friendNetCmdHandler) {
	c = new(friendNetCmdHandler)
	c.token = token
	c.iid = iid
	c.pro = pro
	c.f = f

	return
}

func (c *friendNetCmdHandler) ProcessFriend() {
	c.f(c.token, c.iid, c.pro)
}

//--------------------------------------------------------------------------------------------
//对应processor.loop的friends存储结构
type FriendsHolder struct {
	//对应processor.中的loop index
	loopIndex int

	//double link和hashmap组合的channel数据
	friends map[int64]*FriendHome
	//根据活动情况排列的 双向列表
	//方便对不活跃的channel进行维护
	friendsLink *datas.DoubleLinkUnSafe

	parent *FriendsCtrl
}

func newFriendsHolder(l int, p *FriendsCtrl) (h *FriendsHolder) {
	h = new(FriendsHolder)
	h.loopIndex = l
	h.friends = make(map[int64]*FriendHome)
	h.friendsLink = datas.NewDoubleLinkUnSafe()

	h.parent = p

	return
}

func (ch *FriendsHolder) GetFriendHomeBy(roleiid int64) *FriendHome {
	d, ok := ch.friends[roleiid]
	if !ok {
		return nil
	}

	return d
}

func (ch *FriendsHolder) OneDayMaintance() {

}

func (ch *FriendsHolder) CacheFriendHome(roleiid int64, frds []*gpro.FriendRelation, invites []*gpro.FriendInviteItem, token *gpro.UserToken) *FriendHome {
	home := ch.GetFriendHomeBy(roleiid)
	if home == nil {
		home = newFriendHome(roleiid, ch)
		ch.friends[home.RoleIid] = home
		ch.friendsLink.AddHeadElement(home)

		home.InitFriendHome(frds, invites, token)

		logx.Infof("FriendsHolder->CacheFriendHome cache user friend home:%d", roleiid)
	}

	return home
}

func (ch *FriendsHolder) NewFriendInvite(roleiid int64, invite *gpro.FriendInviteItem) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERFRIENDHOME, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	dat := utilc.ProtoToBytes(invite)
	if dat == nil {
		return
	}

	k1 := rd.BuildKey(USERFRIENDHOME_IVTLIST, invite.Iid)
	rd.HSet(udkey, k1, dat)

	home := ch.GetFriendHomeBy(roleiid)
	if home != nil {
		home.NewFriendInvite(invite)
	}
}

func (ch *FriendsHolder) InviteConfirm(roleiid int64, inviteiid int64, agree bool, relation *gpro.FriendRelation, token *gpro.UserToken) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERFRIENDHOME, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	//remove invite
	k1 := rd.BuildKey(USERFRIENDHOME_IVTLIST, inviteiid)
	rd.HDel(udkey, []string{k1})

	if agree {
		k2 := rd.BuildKey(USERFRIENDHOME_FRDLIST, relation.Friendiid)
		dat := utilc.ProtoToBytes(relation)
		if dat != nil {
			rd.HSet(udkey, k2, dat)
		}

		//双方互加
		udkey2 := rd.BuildKey(REDIS_USERFRIENDHOME, relation.Friendiid)
		exist, err = rd.KeyExist(udkey2)
		if err == nil && exist {
			relation2 := (proto.Clone(relation)).(*gpro.FriendRelation)
			relation2.Friendiid = roleiid

			k2 = rd.BuildKey(USERFRIENDHOME_FRDLIST, relation2.Friendiid)
			dat = utilc.ProtoToBytes(relation2)
			if dat != nil {
				rd.HSet(udkey2, k2, dat)
			}

			//notify user new friends
			ntf := &gpro.Frd_FriendChangeOtherNtf{NotifyRoleiid: relation.Friendiid, Addordel: true, Friendiid: roleiid, Friend: relation2}
			g.SendMsgToRouter(ntf)
		}
	}

	home := ch.GetFriendHomeBy(roleiid)
	if home != nil {
		home.InviteConfirmB(inviteiid, agree, relation, token)
	}
}

func (ch *FriendsHolder) DeleteFriend(roleiid int64, friendiid int64) {
	rd := g.GetRedis()

	udkey := rd.BuildKey(REDIS_USERFRIENDHOME, roleiid)
	exist, err := rd.KeyExist(udkey)
	if err != nil || !exist {
		return
	}

	k2 := rd.BuildKey(USERFRIENDHOME_FRDLIST, friendiid)
	rd.HDel(udkey, []string{k2})

	//删除好友方数据
	udkey = rd.BuildKey(REDIS_USERFRIENDHOME, friendiid)
	exist, err = rd.KeyExist(udkey)
	if err == nil && exist {
		k2 = rd.BuildKey(USERFRIENDHOME_FRDLIST, roleiid)
		rd.HDel(udkey, []string{k2})
	}
	ntf := &gpro.Frd_FriendChangeOtherNtf{NotifyRoleiid: friendiid, Addordel: false, Friendiid: roleiid}
	g.SendMsgToRouter(ntf)

	home := ch.GetFriendHomeBy(roleiid)
	if home != nil {
		home.DeleteFriendB(friendiid)
	}
}

func (ch *FriendsHolder) OnFriendOtherChangeNotify(_ *gpro.UserToken, id int, pro proto.Message) {
	msg := pro.(*gpro.Frd_FriendChangeOtherNtf)

	//只需要移除内存中的数据，不需要更新redis数据
	home := ch.GetFriendHomeBy(msg.NotifyRoleiid)
	if home != nil {
		home.FriendChangeNotify(msg.Friendiid, msg.Addordel, msg.Friend)
	}
}

//run in the boxholder goroutine
func (ch *FriendsHolder) OnNetCmdHander(token *gpro.UserToken, id int, pro proto.Message) {

	_, useriid := xinf.ParseUserGate(uint64(token.GetGiduid()))

	frdHome := ch.GetFriendHomeBy(useriid)
	if frdHome == nil {
		frdHome2, success := initFriendHomeFromRedis(useriid, token, ch)
		if success {
			frdHome = frdHome2
			ch.friendsLink.AddHeadElement(frdHome)
			ch.friends[frdHome.RoleIid] = frdHome
		} else {
			cmd := newDBLoadFriendHomeCmd(useriid, ch, token, id, pro)
			g.PostDBProcessor(cmd)
		}
	}

	if frdHome != nil {
		ch.triggerNetProcess(frdHome, id, pro)
	}

}

func (ch *FriendsHolder) triggerNetProcess(home *FriendHome, id int, pro proto.Message) {
	if id == int(gpro.FRIEND_PROTYPE_FRD_INVITECONFIRM_REQ) {
		msg := pro.(*gpro.Frd_InviteConfirmReq)
		home.InviteConfirmA(msg.Iid, msg.Agree, msg.Utoken)
	} else if id == int(gpro.FRIEND_PROTYPE_FRD_FRIENDDELETE_REQ) {
		msg := pro.(*gpro.Frd_FriendDeleteReq)
		home.DeleteFriendA(msg.Friendiid, msg.Utoken)
	} else if id == int(gpro.FRIEND_PROTYPE_FRD_FRIENDLIST_REQ) {
		msg := pro.(*gpro.Frd_FriendListReq)
		home.QueryFriends(msg)
	}
}
