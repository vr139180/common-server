﻿// Copyright 2021 common-server Authors
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

#include <cmsLib/prolib/ProtocolFactory.h>

#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void ProtocolFactory::init_factory()
{
	//eureka_internal.proto
	regist_protocal<Erk_EurekaRegist_req>(ERK_PROTYPE::ERK_EUREKAREGIST_REQ);
	regist_protocal<Erk_EurekaRegist_ack>(ERK_PROTYPE::ERK_EUREKAREGIST_ACK);
	regist_protocal<Erk_EurekaBind_req>(ERK_PROTYPE::ERK_EUREKABIND_REQ);
	regist_protocal<Erk_EurekaBind_ack>(ERK_PROTYPE::ERK_EUREKABIND_ACK);
	regist_protocal<Erk_EurekaUpdate_ntf>(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF);
	regist_protocal<Erk_MasterChange_ntf>(ERK_PROTYPE::ERK_MASTERCHANGE_NTF);

	regist_protocal<Erk_ServiceRegist_req>(ERK_PROTYPE::ERK_SERVICEREGIST_REQ);
	regist_protocal<Erk_ServiceRegist_ack>(ERK_PROTYPE::ERK_SERVICEREGIST_ACK);
	regist_protocal<Erk_ServiceSync_ntf>(ERK_PROTYPE::ERK_SERVICESYNC_NTF);
	regist_protocal<Erk_ServiceBind_req>(ERK_PROTYPE::ERK_SERVICEBIND_REQ);
	regist_protocal<Erk_ServiceBind_ack>(ERK_PROTYPE::ERK_SERVICEBIND_ACK);
	regist_protocal<Erk_ServiceSubscribe_req>(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ);
	regist_protocal<Erk_ServiceSubscribe_ntf>(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_NTF);
	regist_protocal<Erk_RouterSubscribe_req>(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_REQ);
	regist_protocal<Erk_RouterSubscribe_ntf>(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_NTF);
	regist_protocal<Erk_RouterOnline_req>(ERK_PROTYPE::ERK_ROUTERONLINE_REQ);
	regist_protocal<GsFr_GameRegionRegist_ntf>(ERK_PROTYPE::GSFR_GAMEREGIONREGIST_NTF);

	regist_protocal<Svr_LiveTick_ntf>(ERK_PROTYPE::SVR_LIVETICK_NTF);
	regist_protocal<Svr_ServiceBindService_req>(ERK_PROTYPE::SVR_SERVICEBINDSERVICE_REQ);
	regist_protocal<Svr_ServiceBindService_ack>(ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK);
	regist_protocal<Svr_RouterOnline_ntf>(ERK_PROTYPE::SVR_ROUTERONLINE_NTF);
	regist_protocal<Svr_GateSlotUpdate_ntf>(ERK_PROTYPE::SVR_GATESLOTUPDATE_NTF);

	//robot_test_internal.proto
	regist_protocal<Robot_Config_Req>(ROBOTTEST_PROTYPE::ROBOTOTEST_ROBOT_CONFIG_REQ);
	regist_protocal<Robot_Config_Ack>(ROBOTTEST_PROTYPE::ROBOTOTEST_ROBOT_CONFIG_ACK);
	regist_protocal<Robot_Start_Ack>(ROBOTTEST_PROTYPE::ROBOTOTEST_ROBOT_START_ACK);
	regist_protocal<Robot_Stop_Ack>(ROBOTTEST_PROTYPE::ROBOTOTEST_ROBOT_STOP_ACK);
	regist_protocal<Robot_UploadLogs_ntf>(ROBOTTEST_PROTYPE::ROBOTOTEST_UPLOADLOGSROBOT_NTF);
	regist_protocal<Robot_ClearLogs_ntf>(ROBOTTEST_PROTYPE::ROBOTOTEST_CLEARLOGSROBOT_NTF);
	regist_protocal<Robot_UserPrefixChg_ntf>(ROBOTTEST_PROTYPE::ROBOTOTEST_USERPREFIXROBOT_NTF);

	regist_protocal<Robot_Record_req>(ROBOTTEST_PROTYPE::ROBOTOTEST_RECORD_REG);
	regist_protocal<Robot_StartRecord_req>(ROBOTTEST_PROTYPE::ROBOTOTEST_STARTRECORD_REG);
	regist_protocal<Robot_EndRecord_req>(ROBOTTEST_PROTYPE::ROBOTOTEST_ENDRECORD_REG);


	//自定义协议
	regist_protocal<User_Login_req>(USER_PROTYPE::USER_LOGIN_REQ);
	regist_protocal<User_Login_ack>(USER_PROTYPE::USER_LOGIN_ACK);
	regist_protocal<User_ReLogin_req>(USER_PROTYPE::USER_RELOGIN_REQ);
	regist_protocal<User_ReLogin_ack>(USER_PROTYPE::USER_RELOGIN_ACK);
	regist_protocal<User_Logout_ntf>(USER_PROTYPE::USER_LOGOUT_NTF);
	regist_protocal<User_GateLost_ntf>(USER_PROTYPE::USER_GATELOST_NTF);
	regist_protocal<User_Active_ntf>(USER_PROTYPE::USER_ACTIVE_NTF);
	regist_protocal<User_SaveRoleLoc_ntf>(USER_PROTYPE::USER_SAVEROLELOC_NTF);

	regist_protocal<User_RoleList_req>(USER_PROTYPE::USER_ROLELIST_REQ);
	regist_protocal<User_RoleList_ack>(USER_PROTYPE::USER_ROLELIST_ACK);
	regist_protocal<User_RoleSelect_req>(USER_PROTYPE::USER_ROLESELECT_REQ);
	regist_protocal<User_RoleSelect_ack>(USER_PROTYPE::USER_ROLESELECT_ACK);
	regist_protocal<User_RoleCreate_req>(USER_PROTYPE::USER_ROLECREATE_REQ);
	regist_protocal<User_RoleCreate_ack>(USER_PROTYPE::USER_ROLECREATE_ACK);

	regist_protocal<User_RoleDetailA_ntf>(USER_PROTYPE::USER_ROLEDETAILA_NTF);
	regist_protocal<User_RoleDetailB_ntf>(USER_PROTYPE::USER_ROLEDETAILB_NTF);
	regist_protocal<User_RoleDetailEnd_ntf>(USER_PROTYPE::USER_ROLEDETAILEND_NTF);

	regist_protocal<User_MySimpleInfo_req>(USER_PROTYPE::USER_MYSIMPLEINFO_REQ);
	regist_protocal<User_MySimpleInfo_ack>(USER_PROTYPE::USER_MYSIMPLEINFO_ACK);

	//building
	regist_protocal<Build_AddItem_req>(BUILD_PROTYPE::BUILD_ADDITEM_REQ);
	regist_protocal<Build_AddItem_ack>(BUILD_PROTYPE::BUILD_ADDITEM_ACK);

	regist_protocal<Build_DelItem_req>(BUILD_PROTYPE::BUILD_DELITEM_REQ);
	regist_protocal<Build_DelItem_ack>(BUILD_PROTYPE::BUILD_DELITEM_ACK);

	//pets
	regist_protocal<Pet_AdoptOne_req>(PET_PROTYPE::PET_ADOPTONE_REQ);
	regist_protocal<Pet_AdoptOne_ack>(PET_PROTYPE::PET_ADOPTONE_ACK);
	regist_protocal<Pet_ReleaseOne_req>(PET_PROTYPE::PET_RELEASEONE_REQ);
	regist_protocal<Pet_ReleaseOne_ack>(PET_PROTYPE::PET_RELEASEONE_ACK);

	//chat
	regist_protocal<Chat_GlobalMsg_ntf>(CHAT_PROTYPE::CHAT_GLOBALMSG_NTF);
	regist_protocal<Chat_UserChannels_active>(CHAT_PROTYPE::CHAT_USERCHANNELS_ACTIVE);
	regist_protocal<Chat_CustomChannelId_req>(CHAT_PROTYPE::CHAT_CUSTOMCHANNELID_REQ);
	regist_protocal<Chat_CustomChannelId_ack>(CHAT_PROTYPE::CHAT_CUSTOMCHANNELID_ACK);
	regist_protocal<Chat_UserMsg_say>(CHAT_PROTYPE::CHAT_USERMSG_SAY);
	regist_protocal<Chat_ChatMsg_ntf>(CHAT_PROTYPE::CHAT_CHATMSG_NTF);

	//mail
	regist_protocal<Mail_SystemMail_req>(MAIL_PROTYPE::MAIL_SYSTEMMAIL_REQ);
	regist_protocal<Mail_SystemMail_ack>(MAIL_PROTYPE::MAIL_SYSTEMMAIL_ACK);
	regist_protocal<Mail_SystemMail_ntf>(MAIL_PROTYPE::MAIL_SYSTEMMAIL_NTF);

	regist_protocal<Mail_NewMail_req>(MAIL_PROTYPE::MAIL_NEWMAIL_REQ);
	regist_protocal<Mail_NewMail_ack>(MAIL_PROTYPE::MAIL_NEWMAIL_ACK);
	regist_protocal<Mail_NewMail_ntf>(MAIL_PROTYPE::MAIL_NEWMAIL_NTF);
	regist_protocal<Mail_MailGet_req>(MAIL_PROTYPE::MAIL_MAILGET_REQ);
	regist_protocal<Mail_MailGet_ack>(MAIL_PROTYPE::MAIL_MAILGET_ACK);
	regist_protocal<Mail_UserOnline_active>(MAIL_PROTYPE::MAIL_USERONLINE_ACTIVE);
	regist_protocal<Mail_ReadMail_req>(MAIL_PROTYPE::MAIL_READMAIL_REQ);
	regist_protocal<Mail_ReadMail_ack>(MAIL_PROTYPE::MAIL_READMAIL_ACK);
	regist_protocal<Mail_DeleteMail_req>(MAIL_PROTYPE::MAIL_DELETEMAIL_REQ);
	regist_protocal<Mail_DeleteMail_ack>(MAIL_PROTYPE::MAIL_DELETEMAIL_ACK);
	regist_protocal<Mail_UnBindAttachs_req>(MAIL_PROTYPE::MAIL_UNBINDATTACHS_REQ);
	regist_protocal<Mail_UnBindAttachs_ack>(MAIL_PROTYPE::MAIL_UNBINDATTACHS_ACK);

	//friend
	regist_protocal<Frd_FriendList_req>(FRIEND_PROTYPE::FRD_FRIENDLIST_REQ);
	regist_protocal<Frd_FriendList_ack>(FRIEND_PROTYPE::FRD_FRIENDLIST_ACK);
	regist_protocal<Frd_FriendInvite_req>(FRIEND_PROTYPE::FRD_FRIENDINVITE_REQ);
	regist_protocal<Frd_FriendInvite_ack>(FRIEND_PROTYPE::FRD_FRIENDINVITE_ACK);
	regist_protocal<Frd_InviteConfirm_req>(FRIEND_PROTYPE::FRD_INVITECONFIRM_REQ);
	regist_protocal<Frd_InviteConfirm_ack>(FRIEND_PROTYPE::FRD_INVITECONFIRM_ACK);
	regist_protocal<Frd_FriendDelete_req>(FRIEND_PROTYPE::FRD_FRIENDDELETE_REQ);
	regist_protocal<Frd_FriendDelete_ack>(FRIEND_PROTYPE::FRD_FRIENDDELETE_ACK);

	regist_protocal<Frd_FriendChangeOther_ntf>(FRIEND_PROTYPE::FRD_FRIENDCHANGEOTHER_NTF);

	//item warehouse
	regist_protocal<Item_UserBindItems_req>(ITEMS_PROTYPE::ITEM_USERBINDITEMS_REQ);
	regist_protocal<Item_UserBindItems_ack>(ITEMS_PROTYPE::ITEM_USERBINDITEMS_ACK);

	//task system
	regist_protocal<Task_WaitList_req>(TASK_PROTYPE::TASK_WAITLIST_REQ);
	regist_protocal<Task_WaitList_ack>(TASK_PROTYPE::TASK_WAITLIST_ACK);
	regist_protocal<Task_MyTaskList_req>(TASK_PROTYPE::TASK_MYTASKLIST_REQ);
	regist_protocal<Task_MyTaskList_ack>(TASK_PROTYPE::TASK_MYTASKLIST_ACK);
	regist_protocal<Task_GetTask_req>(TASK_PROTYPE::TASK_GETTASK_REQ);
	regist_protocal<Task_GetTask_ack>(TASK_PROTYPE::TASK_GETTASK_ACK);
	regist_protocal<Task_SubmitTask_req>(TASK_PROTYPE::TASK_SUBMITTASK_REQ);
	regist_protocal<Task_SubmitTask_ack>(TASK_PROTYPE::TASK_SUBMITTASK_ACK);
	//regist_protocal<Task_ObtainReward_req>(TASK_PROTYPE::TASK_OBTAINREWARD_REQ);
	regist_protocal<Task_ObtainReward_ntf>(TASK_PROTYPE::TASK_OBTAINREWARD_NTF);
	regist_protocal<Task_GiveupTask_req>(TASK_PROTYPE::TASK_GIVEUPTASK_REQ);
	regist_protocal<Task_GiveupTask_ack>(TASK_PROTYPE::TASK_GIVEUPTASK_ACK);

	//matchmaking system
	regist_protocal<MMS_MatchMaking_req>(MMS_PROTYPE::MMS_MATCHMAKING_REQ);
	regist_protocal<MMS_MatchMaking_ack>(MMS_PROTYPE::MMS_MATCHMAKING_ACK);

	//game system
	regist_protocal<Game_UserAlive_ntf>(GMS_PROTYPE::GMS_USERALIVE_NTF);
	regist_protocal<Game_SaveUserLoc_ntf>(GMS_PROTYPE::GMS_SAVEUSERLOC_NTF);
	regist_protocal<Game_EnterGame_req>(GMS_PROTYPE::GMS_ENTERGAME_REQ);
	regist_protocal<Game_EnterGame_ack>(GMS_PROTYPE::GMS_ENTERGAME_ACK);
	regist_protocal<Game_UsersVisiable_ntf>(GMS_PROTYPE::GMS_USERSVISIBLE_NTF);
	regist_protocal<Game_ServiceDisable_ntf>(GMS_PROTYPE::GMS_SERVICEDISABLE_NTF);
	regist_protocal<Game_UserInfo_sync>(GMS_PROTYPE::GMS_USERINFO_SYNC);

	regist_protocal<Game_UserState_sync>(GMS_PROTYPE::GMS_USERSTATE_SYN);
}
