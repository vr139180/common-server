#include <cmsLib/prolib/ProtocolFactory.h>

#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void ProtocolFactory::init_factory()
{
	//eureka_internal.proto
	regist_protocal<Erk_EurekaBind_req>(ERK_PROTYPE::ERK_EUREKABIND_REQ);
	regist_protocal<Erk_EurekaBind_ack>(ERK_PROTYPE::ERK_EUREKABIND_ACK);
	regist_protocal<Erk_EurekaUpdate_ntf>(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF);
	regist_protocal<Erk_Eureka_sync>(ERK_PROTYPE::ERK_EUREKA_SYNC);

	regist_protocal<Erk_ServiceRegist_req>(ERK_PROTYPE::ERK_SERVICEREGIST_REQ);
	regist_protocal<Erk_ServiceRegist_ack>(ERK_PROTYPE::ERK_SERVICEREGIST_ACK);
	regist_protocal<Erk_ServiceRegist_Confirm>(ERK_PROTYPE::ERK_SERVICEREGIST_CONFIRM);

	regist_protocal<Erk_ServiceBind_req>(ERK_PROTYPE::ERK_SERVICEBIND_REQ);
	regist_protocal<Erk_ServiceBind_ack>(ERK_PROTYPE::ERK_SERVICEBIND_ACK);

	regist_protocal<Erk_ServiceSubscribe_req>(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ);
	regist_protocal<Erk_ServiceSubscribe_ack>(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_ACK);

	regist_protocal<Erk_ServiceShutdown_ntf>(ERK_PROTYPE::ERK_SERVICESHUTDOWN_NTF);

	regist_protocal<Svr_LiveTick_ntf>(ERK_PROTYPE::SVR_LIVETICK_NTF);
	regist_protocal<Svr_ServiceBindService_req>(ERK_PROTYPE::SVR_SERVICEBINDSERVICE_REQ);
	regist_protocal<Svr_ServiceBindService_ack>(ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK);
	regist_protocal<Svr_GateBindHome_req>(ERK_PROTYPE::SVR_GATEBINDHOME_REQ);
	regist_protocal<Svr_GateBindHome_ack>(ERK_PROTYPE::SVR_GATEBINDHOME_ACK);
	regist_protocal<Svr_GateBindHome_confirm>(ERK_PROTYPE::SVR_GATEBINDHOME_CONFIRM);

	//res_internal.proto
	regist_protocal<Res_UserSlot_req>(ERK_PROTYPE::RES_USERSLOT_REQ);
	regist_protocal<Res_UserSlot_req>(ERK_PROTYPE::RES_USERSLOT_ACK);
	regist_protocal<Res_ProxyUserSlot_req>(ERK_PROTYPE::RES_PROXYUSERSLOT_REQ);
	regist_protocal<Res_ProxyUserSlot_ack>(ERK_PROTYPE::RES_PROXYUSERSLOT_ACK);
	regist_protocal<Res_SyncGateSlot_ntf>(ERK_PROTYPE::RES_SYNCGATESLOT_NTF);

	//gatehome_internal.proto
	regist_protocal<GHS_UserInit_req>(GATEHOME_PROTYPE::GHS_USERINIT_REQ);
	regist_protocal<GHS_UserLogout_ntf>(GATEHOME_PROTYPE::GHS_USERLOGOUT_NTF);
	regist_protocal<GHS_RoleDetail_ask>(GATEHOME_PROTYPE::GHS_ROLEDETAIL_ASK);

	//自定义协议
	regist_protocal<User_Login_req>(USER_PROTYPE::USER_LOGIN_REQ);
	regist_protocal<User_Login_ack>(USER_PROTYPE::USER_LOGIN_ACK);
	regist_protocal<User_ProxyLogin_req>(USER_PROTYPE::USER_PROXYLOGIN_REQ);
	regist_protocal<User_ProxyLogin_ack>(USER_PROTYPE::USER_PROXYLOGIN_ACK);
	regist_protocal<User_Logout_ntf>(USER_PROTYPE::USER_LOGOUT_NTF);

	regist_protocal<User_RoleList_ack>(USER_PROTYPE::USER_ROLELIST_ACK);
	regist_protocal<User_RoleSelect_req>(USER_PROTYPE::USER_ROLESELECT_REQ);
	regist_protocal<User_RoleSelect_ack>(USER_PROTYPE::USER_ROLESELECT_ACK);
	regist_protocal<User_RoleCreate_req>(USER_PROTYPE::USER_ROLECREATE_REQ);
	regist_protocal<User_RoleCreate_ack>(USER_PROTYPE::USER_ROLECREATE_ACK);

	regist_protocal<User_RoleDetailA_ntf>(USER_PROTYPE::USER_ROLEDETAILA_NTF);
	regist_protocal<User_RoleDetailB_ntf>(USER_PROTYPE::USER_ROLEDETAILB_NTF);
	regist_protocal<User_RoleDetailEnd_ntf>(USER_PROTYPE::USER_ROLEDETAILEND_NTF);

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
}
