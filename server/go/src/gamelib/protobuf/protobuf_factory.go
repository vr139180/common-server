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
	pb.registMessage(&gpro.Erk_EurekaUpdateNtf{}, int(gpro.ERK_PROTYPE_ERK_EUREKAUPDATE_NTF))
	pb.registMessage(&gpro.Erk_EurekaSync{}, int(gpro.ERK_PROTYPE_ERK_EUREKA_SYNC))
	// 处理服务注册
	pb.registMessage(&gpro.Erk_ServiceRegistReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_REQ))
	pb.registMessage(&gpro.Erk_ServiceRegistAck{}, int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_ACK))
	pb.registMessage(&gpro.Erk_ServiceRegist_Confirm{}, int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_CONFIRM))
	pb.registMessage(&gpro.Erk_ServiceBindReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_REQ))
	pb.registMessage(&gpro.Erk_ServiceBindAck{}, int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_ACK))
	pb.registMessage(&gpro.Erk_ServiceSubscribeReq{}, int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_REQ))
	pb.registMessage(&gpro.Erk_ServiceSubscribeAck{}, int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_ACK))
	pb.registMessage(&gpro.Erk_ServiceShutdownNtf{}, int(gpro.ERK_PROTYPE_ERK_SERVICESHUTDOWN_NTF))

	pb.registMessage(&gpro.Svr_LiveTickNtf{}, int(gpro.ERK_PROTYPE_SVR_LIVETICK_NTF))
	pb.registMessage(&gpro.Svr_ServiceBindServiceReq{}, int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_REQ))
	pb.registMessage(&gpro.Svr_ServiceBindServiceAck{}, int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK))
	pb.registMessage(&gpro.Svr_GateBindHomeReq{}, int(gpro.ERK_PROTYPE_SVR_GATEBINDHOME_REQ))
	pb.registMessage(&gpro.Svr_GateBindHomeAck{}, int(gpro.ERK_PROTYPE_SVR_GATEBINDHOME_ACK))
	pb.registMessage(&gpro.Svr_GateBindHomeConfirm{}, int(gpro.ERK_PROTYPE_SVR_GATEBINDHOME_CONFIRM))

	//res_internal.proto
	pb.registMessage(&gpro.Res_UserSlotReq{}, int(gpro.ERK_PROTYPE_RES_USERSLOT_REQ))
	pb.registMessage(&gpro.Res_UserSlotAck{}, int(gpro.ERK_PROTYPE_RES_USERSLOT_ACK))
	pb.registMessage(&gpro.Res_ProxyUserSlotReq{}, int(gpro.ERK_PROTYPE_RES_PROXYUSERSLOT_REQ))
	pb.registMessage(&gpro.Res_ProxyUserSlotAck{}, int(gpro.ERK_PROTYPE_RES_PROXYUSERSLOT_ACK))
	pb.registMessage(&gpro.Res_SyncGateSlotNtf{}, int(gpro.ERK_PROTYPE_RES_SYNCGATESLOT_NTF))

	//gatehome_internal.proto
	pb.registMessage(&gpro.GHS_UserInitReq{}, int(gpro.GATEHOME_PROTYPE_GHS_USERINIT_REQ))
	pb.registMessage(&gpro.GHS_UserLogoutNtf{}, int(gpro.GATEHOME_PROTYPE_GHS_USERLOGOUT_NTF))
	pb.registMessage(&gpro.GHS_RoleDetailAsk{}, int(gpro.GATEHOME_PROTYPE_GHS_ROLEDETAIL_ASK))

	//user proto
	pb.registMessage(&gpro.User_LoginReq{}, int(gpro.USER_PROTYPE_USER_LOGIN_REQ))
	pb.registMessage(&gpro.User_LoginAck{}, int(gpro.USER_PROTYPE_USER_LOGIN_ACK))
	pb.registMessage(&gpro.User_ProxyLoginReq{}, int(gpro.USER_PROTYPE_USER_PROXYLOGIN_REQ))
	pb.registMessage(&gpro.User_ProxyLoginAck{}, int(gpro.USER_PROTYPE_USER_PROXYLOGIN_ACK))
	pb.registMessage(&gpro.User_LogoutNtf{}, int(gpro.USER_PROTYPE_USER_LOGOUT_NTF))

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
}
