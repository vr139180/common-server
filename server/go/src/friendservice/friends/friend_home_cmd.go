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
	"cmslib/logx"
	"cmslib/utilc"
	"friendservice/data/db/entity"
	"friendservice/g"
	"gamelib/protobuf/gpro"
	"time"

	"google.golang.org/protobuf/proto"
	"xorm.io/xorm"
)

//---------------------------------------------------------------------------------------
type dbLoadFriendHomeCmd struct {
	frds    []*gpro.FriendRelation
	invites []*gpro.FriendInviteItem

	RoleIid int64
	holder  *FriendsHolder
	token   *gpro.UserToken
	id      int
	pro     proto.Message
}

func newDBLoadFriendHomeCmd(roleiid int64, holder *FriendsHolder, token *gpro.UserToken, id int, pro proto.Message) (c *dbLoadFriendHomeCmd) {
	c = new(dbLoadFriendHomeCmd)
	c.holder = holder
	c.RoleIid = roleiid
	c.token = token
	c.id = id
	c.pro = pro

	return
}

func (c *dbLoadFriendHomeCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		sql1 := `
select f.iid as iid, f.frdiid as friend_iid, r.nickname as nickname from (
(select iid, friend_iid as frdiid from friend_relationship where myself_iid = ?)
union
(select iid, myself_iid as frdiid from friend_relationship where friend_iid = ?)
) f , role_baseinfo r 
where f.frdiid = r.role_iid order by f.iid asc
`
		var frds []entity.DBFriendRelationView
		err1 = session.SQL(sql1, c.RoleIid, c.RoleIid).Find(&frds)
		if err1 != nil {
			return nil, err1
		}

		sql2 := `
select f.iid as iid, f.from_roleiid as from_roleiid, f.invite_time as invite_time, r.nickname as invite_name 
  from friend_invites f, role_baseinfo r 
  where f.to_roleiid=? and f.from_roleiid = r.role_iid 
  order by f.iid asc
`
		var invites []entity.DBFriendInviteView
		err1 = session.SQL(sql2, c.RoleIid).Find(&invites)
		if err1 != nil {
			return nil, err1
		}

		if len(frds) > 0 {
			for _, d := range frds {
				logx.Debugf("friend info iid:%d friendiid:%d nick:%s", d.Iid, d.FriendIid, d.NickName)
				item := dbToRedisBeanOfFriendRelation(&d)
				c.frds = append(c.frds, item)
			}
		}

		if len(invites) > 0 {
			for _, d := range invites {
				logx.Debugf("invite info iid:%d fromiid:%d time:%s nick:%s", d.Iid, d.FromRoleIid, d.InviteTime, d.InviteName)
				item := dbToRedisBeanOfFriendInvite(&d)
				c.invites = append(c.invites, item)
			}
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		logx.Errorf("dbLoadFriendHomeCmd - get user:%d friends failed, err:%s", c.RoleIid, err.Error())
		return
	}

	g.PostFriendProcessor(c.holder.loopIndex, c)
}

func (c *dbLoadFriendHomeCmd) ProcessFriend() {
	home := c.holder.CacheFriendHome(c.RoleIid, c.frds, c.invites, c.token)
	if home != nil {
		c.holder.triggerNetProcess(home, c.id, c.pro)
	}
}

//---------------------------------------------------------------------------------------
type dbInviteFriendCmd struct {
	invite *gpro.FriendInviteItem

	FromRoleIid int64
	RoleIid     int64
	holder      *FriendsHolder
	token       *gpro.UserToken
}

func newDBInviteFriendCmd(roleiid int64, fromiid int64, holder *FriendsHolder, token *gpro.UserToken) (c *dbInviteFriendCmd) {
	c = new(dbInviteFriendCmd)
	c.holder = holder
	c.RoleIid = roleiid
	c.token = token
	c.FromRoleIid = fromiid

	return
}

func (c *dbInviteFriendCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		rls, err1 := session.Where("role_iid = ?", c.RoleIid).Count(&entity.UserRoleInfo{})
		if err1 != nil {
			session.Commit()
			return nil, err1
		}

		if rls == 0 {
			result = 3
			session.Commit()
			return nil, nil
		}

		//test relation exist....
		cnt, err1 := session.Where("(myself_iid = ? and friend_iid = ?) or (myself_iid = ? and friend_iid = ?)", c.RoleIid, c.FromRoleIid, c.FromRoleIid, c.RoleIid).Count(&entity.DBFriendRelation{})
		if err1 != nil {
			session.Commit()
			return nil, err1
		}

		if cnt > 0 {
			//ready exist
			result = 2
			session.Commit()
			return nil, nil
		}

		invite := &entity.DBFriendInvite{FromRoleIid: c.FromRoleIid, ToRoleIid: c.RoleIid}
		exist, err1 := session.Get(invite)
		if err1 != nil {
			session.Commit()
			return nil, err1
		}

		invite.InviteTime = time.Now()

		if exist {
			session.Update(invite)
		} else {
			session.Insert(invite)
		}

		sql2 := `
select f.iid as Iid, f.from_roleiid as from_roleiid, f.invite_time as invite_time, r.nickname as invite_name 
  from friend_invites f, role_baseinfo r 
  where f.from_roleiid = ? and f.to_roleiid = ? and f.from_roleiid = r.role_iid 
  order by f.iid asc
`
		var invites []entity.DBFriendInviteView
		err1 = session.SQL(sql2, c.FromRoleIid, c.RoleIid).Find(&invites)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if len(invites) == 1 {

			d := &invites[0]
			logx.Debugf("invite info iid:%d fromiid:%d time:%s nick:%s", d.Iid, d.FromRoleIid, d.InviteTime, d.InviteName)

			c.invite = dbToRedisBeanOfFriendInvite(&invites[0])
			result = 0
		} else {
			session.Rollback()
			return nil, nil
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		result = 1
		logx.Errorf("dbInviteFriendCmd - invite friends:%d failed, err:%s", c.FromRoleIid, err.Error())
	}

	ack := &gpro.Frd_FriendInviteAck{Utoken: c.token, InviteIid: c.RoleIid, Result: int32(result)}
	if c.invite != nil {
		ack.Invite = c.invite
	}
	g.SendMsgToRouter(ack)

	if result == 0 {
		g.PostFriendProcessor(c.holder.loopIndex, c)
	}
}

func (c *dbInviteFriendCmd) ProcessFriend() {
	c.holder.NewFriendInvite(c.RoleIid, c.invite)
}

//---------------------------------------------------------------------------------------
type dbInviteConfirmCmd struct {
	relation *gpro.FriendRelation

	Agree   bool
	Iid     int64
	FromIid int64
	RoleIid int64
	holder  *FriendsHolder
	token   *gpro.UserToken
}

func newDBInviteConfirmCmd(roleiid int64, fromiid int64, iid int64, agree bool, holder *FriendsHolder, token *gpro.UserToken) (c *dbInviteConfirmCmd) {
	c = new(dbInviteConfirmCmd)
	c.holder = holder
	c.RoleIid = roleiid
	c.FromIid = fromiid
	c.token = token
	c.Iid = iid
	c.Agree = agree

	return
}

func (c *dbInviteConfirmCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	//0 success
	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			session.Commit()
			return nil, err1
		}

		invite := &entity.DBFriendInvite{Iid: c.Iid, ToRoleIid: c.RoleIid}
		affect, err1 := session.Delete(invite)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if affect <= 0 {
			result = 2
			session.Rollback()

			return nil, nil
		}

		if c.Agree {
			cnt, err1 := session.Where("(myself_iid = ? and friend_iid = ?) or (myself_iid = ? and friend_iid = ?)", c.RoleIid, c.FromIid, c.FromIid, c.RoleIid).Count(&entity.DBFriendRelation{})
			if err1 != nil {
				session.Rollback()
				return nil, err1
			}

			if cnt <= 0 {
				//创建关系表
				a1 := &entity.DBFriendRelation{FriendIid: c.FromIid, MyselfIid: c.RoleIid, CreateTime: time.Now()}
				session.Insert(a1)
			}

			sql1 := `
select f.iid as Iid, f.frdiid as friend_iid, r.nickname as nickname from (
(select iid, friend_iid as frdiid from friend_relationship where myself_iid = ? and friend_iid = ?)
union
(select iid, myself_iid as frdiid from friend_relationship where myself_iid = ? and friend_iid = ?)
) f , role_baseinfo r 
where f.frdiid = r.role_iid order by f.iid asc
			`
			var frds []entity.DBFriendRelationView
			err1 = session.SQL(sql1, c.RoleIid, c.FromIid, c.FromIid, c.RoleIid).Find(&frds)
			if err1 != nil {
				session.Rollback()
				return nil, err1
			}

			if len(frds) != 1 {
				session.Rollback()
				return nil, nil
			}

			result = 0
			c.relation = dbToRedisBeanOfFriendRelation(&frds[0])
		} else {
			result = 0
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		logx.Errorf("dbInviteConfirmCmd - invite confirm user:%d iid:%d failed, err:%s", c.RoleIid, c.Iid, err.Error())
		result = 1
	}

	//处理失败
	ack := &gpro.Frd_InviteConfirmAck{Utoken: c.token, Iid: c.Iid, Agree: c.Agree, Result: int32(result)}
	if c.relation != nil {
		ack.Friend = proto.Clone(c.relation).(*gpro.FriendRelation)
	}
	g.SendMsgToRouter(ack)

	if result == 0 {
		g.PostFriendProcessor(c.holder.loopIndex, c)
	}
}

func (c *dbInviteConfirmCmd) ProcessFriend() {
	c.holder.InviteConfirm(c.RoleIid, c.Iid, c.Agree, c.relation, c.token)
}

//---------------------------------------------------------------------------------------
type dbFriendDeleteCmd struct {
	Iid       int64
	RoleIid   int64
	FriendIid int64
	holder    *FriendsHolder
	token     *gpro.UserToken
}

func newDBFriendDeleteCmd(roleiid int64, frdiid int64, iid int64, holder *FriendsHolder, token *gpro.UserToken) (c *dbFriendDeleteCmd) {
	c = new(dbFriendDeleteCmd)
	c.holder = holder
	c.RoleIid = roleiid
	c.FriendIid = frdiid
	c.Iid = iid
	c.token = token

	return
}

func (c *dbFriendDeleteCmd) RunInDBProcessor() {
	db := g.GetDBClient()

	result := 1
	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err1 := session.Begin()
		if err1 != nil {
			return nil, err1
		}

		a1 := &entity.DBFriendRelation{Iid: c.Iid}
		affect, err1 := session.Delete(a1)
		if err1 != nil {
			session.Rollback()
			return nil, err1
		}

		if affect <= 0 {
			result = 2
		} else {
			result = 0
		}

		err1 = session.Commit()
		return nil, err1
	})

	if err != nil {
		logx.Errorf("dbFriendDeleteCmd - delete friend iid:%d failed, err:%s", c.Iid, err.Error())
	}

	ack := &gpro.Frd_FriendDeleteAck{Utoken: c.token, Friendiid: c.FriendIid, Result: int32(result)}
	g.SendMsgToRouter(ack)

	if result == 0 {
		g.PostFriendProcessor(c.holder.loopIndex, c)
	}
}

func (c *dbFriendDeleteCmd) ProcessFriend() {
	c.holder.DeleteFriend(c.RoleIid, c.FriendIid)
}

//---------------------------------------------------------------------------------------

func dbToRedisBeanOfFriendInvite(db *entity.DBFriendInviteView) (rd *gpro.FriendInviteItem) {
	rd = new(gpro.FriendInviteItem)

	rd.Iid = db.Iid
	rd.FromIid = db.FromRoleIid
	rd.Invitetime = utilc.GetTimeSecond2(db.InviteTime)
	rd.Nickname = db.InviteName

	return
}

func dbToRedisBeanOfFriendRelation(db *entity.DBFriendRelationView) (rd *gpro.FriendRelation) {
	rd = new(gpro.FriendRelation)

	rd.Iid = db.Iid
	rd.Friendiid = db.FriendIid
	rd.Nickname = db.NickName
	rd.Online = false

	return
}
