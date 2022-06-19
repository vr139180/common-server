/**
* protocol module
*
* auto generate file, don't change it.
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2022
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using com.cms.client.network;

namespace com.cms.client.network.proto
{

    public partial class AppProtoFactory : IProtoFactory
    {
        public void init_protolib()
        {
            //--------------------user_login.pro--------------------------
            regist_one((UInt16)gpro.USER_PROTYPE.UserLoginReq, new gpro.User_Login_req());
            regist_one((UInt16)gpro.USER_PROTYPE.UserLoginAck, new gpro.User_Login_ack());
            regist_one((UInt16)gpro.USER_PROTYPE.UserProxyloginReq, new gpro.User_ProxyLogin_req());
            regist_one((UInt16)gpro.USER_PROTYPE.UserProxyloginAck, new gpro.User_ProxyLogin_ack());
            regist_one((UInt16)gpro.USER_PROTYPE.UserLogoutNtf, new gpro.User_Logout_ntf());

            regist_one((UInt16)gpro.USER_PROTYPE.UserRolelistAck, new gpro.User_RoleList_ack());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRoleselectReq, new gpro.User_RoleSelect_req());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRoleselectAck, new gpro.User_RoleSelect_ack());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRolecreateReq, new gpro.User_RoleCreate_req());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRolecreateAck, new gpro.User_RoleCreate_ack());

            regist_one((UInt16)gpro.USER_PROTYPE.UserRoledetailaNtf, new gpro.User_RoleDetailA_ntf());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRoledetailbNtf, new gpro.User_RoleDetailB_ntf());
            regist_one((UInt16)gpro.USER_PROTYPE.UserRoledetailendNtf, new gpro.User_RoleDetailEnd_ntf());

            //---------------------user_build.pro------------------------
            regist_one((UInt16)gpro.BUILD_PROTYPE.BuildAdditemReq, new gpro.Build_AddItem_req());
            regist_one((UInt16)gpro.BUILD_PROTYPE.BuildAdditemAck, new gpro.Build_AddItem_ack());
            regist_one((UInt16)gpro.BUILD_PROTYPE.BuildDelitemReq, new gpro.Build_DelItem_req());
            regist_one((UInt16)gpro.BUILD_PROTYPE.BuildDelitemAck, new gpro.Build_DelItem_ack());

            //---------------------user_pet.pro------------------------
            regist_one((UInt16)gpro.PET_PROTYPE.PetAdoptoneReq, new gpro.Pet_AdoptOne_req());
            regist_one((UInt16)gpro.PET_PROTYPE.PetAdoptoneAck, new gpro.Pet_AdoptOne_ack());
            regist_one((UInt16)gpro.PET_PROTYPE.PetReleaseoneReq, new gpro.Pet_ReleaseOne_req());
            regist_one((UInt16)gpro.PET_PROTYPE.PetReleaseoneAck, new gpro.Pet_ReleaseOne_ack());

            //---------------------user_chat.pro------------------------
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatGlobalmsgNtf, new gpro.Chat_GlobalMsg_ntf());
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatUserchannelsActive, new gpro.Chat_UserChannels_active());
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatCustomchannelidReq, new gpro.Chat_CustomChannelId_req());
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatCustomchannelidAck, new gpro.Chat_CustomChannelId_ack());
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatUsermsgSay, new gpro.Chat_UserMsg_say());
            regist_one((UInt16)gpro.CHAT_PROTYPE.ChatChatmsgNtf, new gpro.Chat_ChatMsg_ntf());

            //---------------------mail_system.pro------------------------
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailNewmailNtf, new gpro.Mail_NewMail_ntf());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailMailgetReq, new gpro.Mail_MailGet_req());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailMailgetAck, new gpro.Mail_MailGet_ack());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailUseronlineActive, new gpro.Mail_UserOnline_active());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailReadmailReq, new gpro.Mail_ReadMail_req());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailReadmailAck, new gpro.Mail_ReadMail_ack());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailDeletemailReq, new gpro.Mail_DeleteMail_req());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailDeletemailAck, new gpro.Mail_DeleteMail_ack());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailUnbindattachsReq, new gpro.Mail_UnBindAttachs_req());
            regist_one((UInt16)gpro.MAIL_PROTYPE.MailUnbindattachsAck, new gpro.Mail_UnBindAttachs_ack());

            //---------------------friend_system.pro------------------------
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFriendlistReq, new gpro.Frd_FriendList_req());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFriendlistAck, new gpro.Frd_FriendList_ack());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFriendinviteReq, new gpro.Frd_FriendInvite_req());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFriendinviteAck, new gpro.Frd_FriendInvite_ack());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdInviteconfirmReq, new gpro.Frd_InviteConfirm_req());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdInviteconfirmAck, new gpro.Frd_InviteConfirm_ack());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFrienddeleteReq, new gpro.Frd_FriendDelete_req());
            regist_one((UInt16)gpro.FRIEND_PROTYPE.FrdFrienddeleteAck, new gpro.Frd_FriendDelete_ack());
        }

    }

}

