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

package protobuf

import (
	"errors"
	"fmt"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

// protobuf适用的 gnet编解码器
type ProtobufFactory struct {
	nameToId    map[string]int
	idToMessage map[int]proto.Message
}

func NewProtobufFactory() *ProtobufFactory {
	fact := new(ProtobufFactory)

	fact.nameToId = make(map[string]int)
	fact.idToMessage = make(map[int]proto.Message)

	fact.initFactory()

	return fact
}

func (pb *ProtobufFactory) registMessage(msg proto.Message, id int) {
	fname := proto.MessageName(msg)
	pb.nameToId[string(fname)] = id
	pb.idToMessage[id] = msg
}

// Encoding....
func (pb *ProtobufFactory) ProtoToId(msg proto.Message) (id uint16, err error) {
	if msg == nil {
		return 0, errors.New("proto message cant be nil")
	}

	fname := proto.MessageName(msg)
	msgid, ok := pb.nameToId[string(fname)]
	if !ok {
		return 0, errors.New("proto message dont defined")
	}

	id = uint16(msgid)
	return
}

// Decoding...
func (pb *ProtobufFactory) IdToProto(id uint16) (pro proto.Message, err error) {
	msg, ok := pb.idToMessage[int(id)]
	if !ok {
		return nil, fmt.Errorf("message[id:%d] not defined", id)
	}

	pro = proto.Clone(msg)
	return
}

func (pb *ProtobufFactory) initFactory() {
	// 处理eureka注册中心节点同步
	pb.registMessage(&gpro.Erk_EurekaRegistReq{}, int(gpro.ERK_PROTYPE_ERK_EUREKAREGIST_REQ))
	pb.registMessage(&gpro.Erk_EurekaRegistAck{}, int(gpro.ERK_PROTYPE_ERK_EUREKAREGIST_ACK))
	pb.registMessage(&gpro.Erk_EurekaBindReq{}, int(gpro.ERK_PROTYPE_ERK_EUREKABIND_REQ))
	pb.registMessage(&gpro.Erk_EurekaBindAck{}, int(gpro.ERK_PROTYPE_ERK_EUREKABIND_ACK))
	pb.registMessage(&gpro.Erk_EurekaUpdateNtf{}, int(gpro.ERK_PROTYPE_ERK_EUREKAUPDATE_NTF))
	pb.registMessage(&gpro.Erk_MasterChangeNtf{}, int(gpro.ERK_PROTYPE_ERK_MASTERCHANGE_NTF))
	// 处理服务注册
	pb.registMessage(&gpro.Erk_ServiceRegistReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_REQ))
	pb.registMessage(&gpro.Erk_ServiceRegistAck{}, int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_ACK))
	pb.registMessage(&gpro.Erk_ServiceSyncNtf{}, int(gpro.ERK_PROTYPE_ERK_SERVICESYNC_NTF))
	pb.registMessage(&gpro.Erk_ServiceBindReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_REQ))
	pb.registMessage(&gpro.Erk_ServiceBindAck{}, int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_ACK))
	pb.registMessage(&gpro.Erk_ServiceSubscribeReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_REQ))
	pb.registMessage(&gpro.Erk_ServiceSubscribeNtf{}, int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_NTF))
	pb.registMessage(&gpro.Erk_RouterSubscribeReq{}, int(gpro.ERK_PROTYPE_ERK_ROUTERSUBSCRIBE_REQ))
	pb.registMessage(&gpro.Erk_RouterSubscribeNtf{}, int(gpro.ERK_PROTYPE_ERK_ROUTERSUBSCRIBE_NTF))
	pb.registMessage(&gpro.Erk_RouterOnlineReq{}, int(gpro.ERK_PROTYPE_ERK_ROUTERONLINE_REQ))
	pb.registMessage(&gpro.GsFr_GameRegionRegistNtf{}, int(gpro.ERK_PROTYPE_GSFR_GAMEREGIONREGIST_NTF))

	pb.registMessage(&gpro.Svr_LiveTickNtf{}, int(gpro.ERK_PROTYPE_SVR_LIVETICK_NTF))
	pb.registMessage(&gpro.Svr_ServiceBindServiceReq{}, int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_REQ))
	pb.registMessage(&gpro.Svr_ServiceBindServiceAck{}, int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK))
	pb.registMessage(&gpro.Svr_RouterOnlineNtf{}, int(gpro.ERK_PROTYPE_SVR_ROUTERONLINE_NTF))
	pb.registMessage(&gpro.Svr_GateSlotUpdateNtf{}, int(gpro.ERK_PROTYPE_SVR_GATESLOTUPDATE_NTF))

	//-----------------------------------------------------------------------------------------------------
	//user proto
	pb.registMessage(&gpro.User_LoginReq{}, int(gpro.USER_PROTYPE_USER_LOGIN_REQ))
	pb.registMessage(&gpro.User_LoginAck{}, int(gpro.USER_PROTYPE_USER_LOGIN_ACK))
	pb.registMessage(&gpro.User_ReLoginReq{}, int(gpro.USER_PROTYPE_USER_RELOGIN_REQ))
	pb.registMessage(&gpro.User_ReLoginAck{}, int(gpro.USER_PROTYPE_USER_RELOGIN_ACK))
	pb.registMessage(&gpro.User_LogoutNtf{}, int(gpro.USER_PROTYPE_USER_LOGOUT_NTF))
	pb.registMessage(&gpro.User_GateLostNtf{}, int(gpro.USER_PROTYPE_USER_GATELOST_NTF))
	pb.registMessage(&gpro.User_ActiveNtf{}, int(gpro.USER_PROTYPE_USER_ACTIVE_NTF))
	pb.registMessage(&gpro.User_SaveRoleLocNtf{}, int(gpro.USER_PROTYPE_USER_SAVEROLELOC_NTF))

	pb.registMessage(&gpro.User_RoleListReq{}, int(gpro.USER_PROTYPE_USER_ROLELIST_REQ))
	pb.registMessage(&gpro.User_RoleListAck{}, int(gpro.USER_PROTYPE_USER_ROLELIST_ACK))
	pb.registMessage(&gpro.User_RoleSelectReq{}, int(gpro.USER_PROTYPE_USER_ROLESELECT_REQ))
	pb.registMessage(&gpro.User_RoleSelectAck{}, int(gpro.USER_PROTYPE_USER_ROLESELECT_ACK))
	pb.registMessage(&gpro.User_RoleCreateReq{}, int(gpro.USER_PROTYPE_USER_ROLECREATE_REQ))
	pb.registMessage(&gpro.User_RoleCreateAck{}, int(gpro.USER_PROTYPE_USER_ROLECREATE_ACK))

	pb.registMessage(&gpro.User_RoleDetailANtf{}, int(gpro.USER_PROTYPE_USER_ROLEDETAILA_NTF))
	pb.registMessage(&gpro.User_RoleDetailBNtf{}, int(gpro.USER_PROTYPE_USER_ROLEDETAILB_NTF))
	pb.registMessage(&gpro.User_RoleDetailEndNtf{}, int(gpro.USER_PROTYPE_USER_ROLEDETAILEND_NTF))

	//building
	pb.registMessage(&gpro.Build_AddItemReq{}, int(gpro.BUILD_PROTYPE_BUILD_ADDITEM_REQ))
	pb.registMessage(&gpro.Build_AddItemAck{}, int(gpro.BUILD_PROTYPE_BUILD_ADDITEM_ACK))

	pb.registMessage(&gpro.Build_DelItemReq{}, int(gpro.BUILD_PROTYPE_BUILD_DELITEM_REQ))
	pb.registMessage(&gpro.Build_DelItemAck{}, int(gpro.BUILD_PROTYPE_BUILD_DELITEM_ACK))

	//pets
	pb.registMessage(&gpro.Pet_AdoptOneReq{}, int(gpro.PET_PROTYPE_PET_ADOPTONE_REQ))
	pb.registMessage(&gpro.Pet_AdoptOneAck{}, int(gpro.PET_PROTYPE_PET_ADOPTONE_ACK))
	pb.registMessage(&gpro.Pet_ReleaseOneReq{}, int(gpro.PET_PROTYPE_PET_RELEASEONE_REQ))
	pb.registMessage(&gpro.Pet_ReleaseOneAck{}, int(gpro.PET_PROTYPE_PET_RELEASEONE_ACK))

	//chat
	pb.registMessage(&gpro.Chat_GlobalMsgNtf{}, int(gpro.CHAT_PROTYPE_CHAT_GLOBALMSG_NTF))
	pb.registMessage(&gpro.Chat_UserChannelsActive{}, int(gpro.CHAT_PROTYPE_CHAT_USERCHANNELS_ACTIVE))
	pb.registMessage(&gpro.Chat_CustomChannelIdReq{}, int(gpro.CHAT_PROTYPE_CHAT_CUSTOMCHANNELID_REQ))
	pb.registMessage(&gpro.Chat_CustomChannelIdAck{}, int(gpro.CHAT_PROTYPE_CHAT_CUSTOMCHANNELID_ACK))
	pb.registMessage(&gpro.Chat_UserMsgSay{}, int(gpro.CHAT_PROTYPE_CHAT_USERMSG_SAY))
	pb.registMessage(&gpro.Chat_ChatMsgNtf{}, int(gpro.CHAT_PROTYPE_CHAT_CHATMSG_NTF))

	//mail
	pb.registMessage(&gpro.Mail_SystemMailReq{}, int(gpro.MAIL_PROTYPE_MAIL_SYSTEMMAIL_REQ))
	pb.registMessage(&gpro.Mail_SystemMailAck{}, int(gpro.MAIL_PROTYPE_MAIL_SYSTEMMAIL_ACK))
	pb.registMessage(&gpro.Mail_SystemMailNtf{}, int(gpro.MAIL_PROTYPE_MAIL_SYSTEMMAIL_NTF))

	pb.registMessage(&gpro.Mail_NewMailReq{}, int(gpro.MAIL_PROTYPE_MAIL_NEWMAIL_REQ))
	pb.registMessage(&gpro.Mail_NewMailAck{}, int(gpro.MAIL_PROTYPE_MAIL_NEWMAIL_ACK))
	pb.registMessage(&gpro.Mail_NewMailNtf{}, int(gpro.MAIL_PROTYPE_MAIL_NEWMAIL_NTF))
	pb.registMessage(&gpro.Mail_MailGetReq{}, int(gpro.MAIL_PROTYPE_MAIL_MAILGET_REQ))
	pb.registMessage(&gpro.Mail_MailGetAck{}, int(gpro.MAIL_PROTYPE_MAIL_MAILGET_ACK))
	pb.registMessage(&gpro.Mail_UserOnlineActive{}, int(gpro.MAIL_PROTYPE_MAIL_USERONLINE_ACTIVE))
	pb.registMessage(&gpro.Mail_ReadMailReq{}, int(gpro.MAIL_PROTYPE_MAIL_READMAIL_REQ))
	pb.registMessage(&gpro.Mail_ReadMailAck{}, int(gpro.MAIL_PROTYPE_MAIL_READMAIL_ACK))
	pb.registMessage(&gpro.Mail_DeleteMailReq{}, int(gpro.MAIL_PROTYPE_MAIL_DELETEMAIL_REQ))
	pb.registMessage(&gpro.Mail_DeleteMailAck{}, int(gpro.MAIL_PROTYPE_MAIL_DELETEMAIL_ACK))
	pb.registMessage(&gpro.Mail_UnBindAttachsReq{}, int(gpro.MAIL_PROTYPE_MAIL_UNBINDATTACHS_REQ))
	pb.registMessage(&gpro.Mail_UnBindAttachsAck{}, int(gpro.MAIL_PROTYPE_MAIL_UNBINDATTACHS_ACK))

	//friend
	pb.registMessage(&gpro.Frd_FriendListReq{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDLIST_REQ))
	pb.registMessage(&gpro.Frd_FriendListAck{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDLIST_ACK))
	pb.registMessage(&gpro.Frd_FriendInviteReq{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDINVITE_REQ))
	pb.registMessage(&gpro.Frd_FriendInviteAck{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDINVITE_ACK))
	pb.registMessage(&gpro.Frd_InviteConfirmReq{}, int(gpro.FRIEND_PROTYPE_FRD_INVITECONFIRM_REQ))
	pb.registMessage(&gpro.Frd_InviteConfirmAck{}, int(gpro.FRIEND_PROTYPE_FRD_INVITECONFIRM_ACK))
	pb.registMessage(&gpro.Frd_FriendDeleteReq{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDDELETE_REQ))
	pb.registMessage(&gpro.Frd_FriendDeleteAck{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDDELETE_ACK))

	pb.registMessage(&gpro.Frd_FriendChangeOtherNtf{}, int(gpro.FRIEND_PROTYPE_FRD_FRIENDCHANGEOTHER_NTF))

	//item warehouse
	pb.registMessage(&gpro.Item_UserBindItemsReq{}, int(gpro.ITEMS_PROTYPE_ITEM_USERBINDITEMS_REQ))
	pb.registMessage(&gpro.Item_UserBindItemsAck{}, int(gpro.ITEMS_PROTYPE_ITEM_USERBINDITEMS_ACK))

	//task
	pb.registMessage(&gpro.Task_WaitListReq{}, int(gpro.TASK_PROTYPE_TASK_WAITLIST_REQ))
	pb.registMessage(&gpro.Task_WaitListAck{}, int(gpro.TASK_PROTYPE_TASK_WAITLIST_ACK))
	pb.registMessage(&gpro.Task_MyTaskListReq{}, int(gpro.TASK_PROTYPE_TASK_MYTASKLIST_REQ))
	pb.registMessage(&gpro.Task_MyTaskListAck{}, int(gpro.TASK_PROTYPE_TASK_MYTASKLIST_ACK))
	pb.registMessage(&gpro.Task_GetTaskReq{}, int(gpro.TASK_PROTYPE_TASK_GETTASK_REQ))
	pb.registMessage(&gpro.Task_GetTaskAck{}, int(gpro.TASK_PROTYPE_TASK_GETTASK_ACK))
	pb.registMessage(&gpro.Task_SubmitTaskReq{}, int(gpro.TASK_PROTYPE_TASK_SUBMITTASK_REQ))
	pb.registMessage(&gpro.Task_SubmitTaskAck{}, int(gpro.TASK_PROTYPE_TASK_SUBMITTASK_ACK))
	//pb.registMessage(&gpro.Task_ObtainRewardReq{}, int(gpro.TASK_PROTYPE_TASK_OBTAINREWARD_REQ))
	pb.registMessage(&gpro.Task_ObtainRewardNtf{}, int(gpro.TASK_PROTYPE_TASK_OBTAINREWARD_NTF))
	pb.registMessage(&gpro.Task_GiveupTaskReq{}, int(gpro.TASK_PROTYPE_TASK_GIVEUPTASK_REQ))
	pb.registMessage(&gpro.Task_GiveupTaskAck{}, int(gpro.TASK_PROTYPE_TASK_GIVEUPTASK_ACK))

	//matchmaking
	pb.registMessage(&gpro.MMS_MatchMakingReq{}, int(gpro.MMS_PROTYPE_MMS_MATCHMAKING_REQ))
	pb.registMessage(&gpro.MMS_MatchMakingAck{}, int(gpro.MMS_PROTYPE_MMS_MATCHMAKING_ACK))

	//game system
	pb.registMessage(&gpro.Game_UserAliveNtf{}, int(gpro.GMS_PROTYPE_GMS_USERALIVE_NTF))
	pb.registMessage(&gpro.Game_SaveUserLocNtf{}, int(gpro.GMS_PROTYPE_GMS_SAVEUSERLOC_NTF))
	pb.registMessage(&gpro.Game_EnterGameReq{}, int(gpro.GMS_PROTYPE_GMS_ENTERGAME_REQ))
	pb.registMessage(&gpro.Game_EnterGameAck{}, int(gpro.GMS_PROTYPE_GMS_ENTERGAME_ACK))
	pb.registMessage(&gpro.Game_UsersVisiableNtf{}, int(gpro.GMS_PROTYPE_GMS_USERSVISIBLE_NTF))
	pb.registMessage(&gpro.Game_ServiceDisableNtf{}, int(gpro.GMS_PROTYPE_GMS_SERVICEDISABLE_NTF))
	pb.registMessage(&gpro.Game_UserInfoSync{}, int(gpro.GMS_PROTYPE_GMS_USERINFO_SYNC))

	pb.registMessage(&gpro.Game_UserStateSync{}, int(gpro.GMS_PROTYPE_GMS_USERSTATE_SYN))
}
