// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: proto_iid.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace com.cms.client.network.gpro {

  /// <summary>Holder for reflection information generated from proto_iid.proto</summary>
  public static partial class ProtoIidReflection {

    #region Descriptor
    /// <summary>File descriptor for proto_iid.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static ProtoIidReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Cg9wcm90b19paWQucHJvdG8SA1BSTyr8AgoMVVNFUl9QUk9UWVBFEhIKDlVO",
            "SVZFUlNBTF8xMDAwEAASEQoMRVJLX1VTRVJfTlVNEOgHEhMKDlVTRVJfTE9H",
            "SU5fUkVREOkHEhMKDlVTRVJfTE9HSU5fQUNLEOoHEhgKE1VTRVJfUFJPWFlM",
            "T0dJTl9SRVEQ6wcSGAoTVVNFUl9QUk9YWUxPR0lOX0FDSxDsBxIUCg9VU0VS",
            "X0xPR09VVF9OVEYQ7QcSFgoRVVNFUl9ST0xFTElTVF9BQ0sQ7wcSGAoTVVNF",
            "Ul9ST0xFU0VMRUNUX1JFURDwBxIYChNVU0VSX1JPTEVTRUxFQ1RfQUNLEPEH",
            "EhgKE1VTRVJfUk9MRUNSRUFURV9SRVEQ8gcSGAoTVVNFUl9ST0xFQ1JFQVRF",
            "X0FDSxDzBxIZChRVU0VSX1JPTEVERVRBSUxBX05URhD0BxIZChRVU0VSX1JP",
            "TEVERVRBSUxCX05URhD1BxIbChZVU0VSX1JPTEVERVRBSUxFTkRfTlRGEPYH",
            "KoMBCg1CVUlMRF9QUk9UWVBFEhIKDlVOSVZFUlNBTF8xMjUwEAASFgoRQlVJ",
            "TERfQURESVRFTV9SRVEQ4wkSFgoRQlVJTERfQURESVRFTV9BQ0sQ5AkSFgoR",
            "QlVJTERfREVMSVRFTV9SRVEQ5QkSFgoRQlVJTERfREVMSVRFTV9BQ0sQ5gkq",
            "gQEKC1BFVF9QUk9UWVBFEhIKDlVOSVZFUlNBTF8xNTAwEAASFQoQUEVUX0FE",
            "T1BUT05FX1JFURDdCxIVChBQRVRfQURPUFRPTkVfQUNLEN4LEhcKElBFVF9S",
            "RUxFQVNFT05FX1JFURDfCxIXChJQRVRfUkVMRUFTRU9ORV9BQ0sQ4AsqhAIK",
            "DENIQVRfUFJPVFlQRRISCg5VTklWRVJTQUxfMTc1MBAAEhMKDkNIQVRfTVNH",
            "X0JFR0lOENYNEh0KGENIQVRfVVNFUkNIQU5ORUxTX0FDVElWRRDXDRIdChhD",
            "SEFUX0NVU1RPTUNIQU5ORUxJRF9SRVEQ2A0SHQoYQ0hBVF9DVVNUT01DSEFO",
            "TkVMSURfQUNLENkNEhUKEENIQVRfVVNFUk1TR19TQVkQ2g0SFQoQQ0hBVF9D",
            "SEFUTVNHX05URhDbDRIXChJDSEFUX0dMT0JBTE1TR19OVEYQnQ8SEQoMQ0hB",
            "VF9NU0dfRU5EEJ4PEhQKD0NIQVRfTVNHQUxMX0VORBDPDyrcAwoMTUFJTF9Q",
            "Uk9UWVBFEhIKDlVOSVZFUlNBTF8yMDAwEAASEwoOTUFJTF9NU0dfQkVHSU4Q",
            "0A8SFQoQTUFJTF9ORVdNQUlMX05URhDRDxIVChBNQUlMX01BSUxHRVRfUkVR",
            "ENIPEhUKEE1BSUxfTUFJTEdFVF9BQ0sQ0w8SGwoWTUFJTF9VU0VST05MSU5F",
            "X0FDVElWRRDUDxIWChFNQUlMX1JFQURNQUlMX1JFURDVDxIWChFNQUlMX1JF",
            "QURNQUlMX0FDSxDWDxIYChNNQUlMX0RFTEVURU1BSUxfUkVRENcPEhgKE01B",
            "SUxfREVMRVRFTUFJTF9BQ0sQ2A8SGwoWTUFJTF9VTkJJTkRBVFRBQ0hTX1JF",
            "URDZDxIbChZNQUlMX1VOQklOREFUVEFDSFNfQUNLENoPEhgKE01BSUxfU1lT",
            "VEVNTUFJTF9SRVEQmRESGAoTTUFJTF9TWVNURU1NQUlMX0FDSxCaERIYChNN",
            "QUlMX1NZU1RFTU1BSUxfTlRGEJsREhUKEE1BSUxfTkVXTUFJTF9SRVEQnBES",
            "FQoQTUFJTF9ORVdNQUlMX0FDSxCdERIRCgxNQUlMX01TR19FTkQQwBESFAoP",
            "TUFJTF9NU0dBTExfRU5EEMkRKt4CCg5GUklFTkRfUFJPVFlQRRISCg5VTklW",
            "RVJTQUxfMjI1MBAAEhUKEEZSSUVORF9NU0dfQkVHSU4QyhESFwoSRlJEX0ZS",
            "SUVORExJU1RfUkVREMsREhcKEkZSRF9GUklFTkRMSVNUX0FDSxDMERIZChRG",
            "UkRfRlJJRU5ESU5WSVRFX1JFURDNERIZChRGUkRfRlJJRU5ESU5WSVRFX0FD",
            "SxDOERIaChVGUkRfSU5WSVRFQ09ORklSTV9SRVEQzxESGgoVRlJEX0lOVklU",
            "RUNPTkZJUk1fQUNLENAREhkKFEZSRF9GUklFTkRERUxFVEVfUkVRENEREhkK",
            "FEZSRF9GUklFTkRERUxFVEVfQUNLENIREhMKDkZSSUVORF9NU0dfRU5EEJIT",
            "Eh4KGUZSRF9GUklFTkRDSEFOR0VPVEhFUl9OVEYQkxMSFgoRRlJJRU5EX01T",
            "R0FMTF9FTkQQwxMqXQoNSVRFTVNfUFJPVFlQRRISCg5VTklWRVJTQUxfMjUw",
            "MBAAEhsKFklURU1fVVNFUkJJTkRJVEVNU19SRVEQxRMSGwoWSVRFTV9VU0VS",
            "QklORElURU1TX0FDSxDGEyr2AgoMVEFTS19QUk9UWVBFEhIKDlVOSVZFUlNB",
            "TF8yNzUwEAASEwoOVEFTS19NU0dfQkVHSU4QvhUSFgoRVEFTS19XQUlUTElT",
            "VF9SRVEQvxUSFgoRVEFTS19XQUlUTElTVF9BQ0sQwBUSGAoTVEFTS19NWVRB",
            "U0tMSVNUX1JFURDBFRIYChNUQVNLX01ZVEFTS0xJU1RfQUNLEMIVEhUKEFRB",
            "U0tfR0VUVEFTS19SRVEQwxUSFQoQVEFTS19HRVRUQVNLX0FDSxDEFRIYChNU",
            "QVNLX1NVQk1JVFRBU0tfUkVREMUVEhgKE1RBU0tfU1VCTUlUVEFTS19BQ0sQ",
            "xhUSGgoVVEFTS19PQlRBSU5SRVdBUkRfTlRGEMgVEhgKE1RBU0tfR0lWRVVQ",
            "VEFTS19SRVEQyRUSGAoTVEFTS19HSVZFVVBUQVNLX0FDSxDKFRIRCgxUQVNL",
            "X01TR19FTkQQhhcSFAoPVEFTS19NU0dBTExfRU5EELcXKpABCgtNTVNfUFJP",
            "VFlQRRISCg5VTklWRVJTQUxfMzAwMBAAEhIKDU1NU19NU0dfQkVHSU4QuBcS",
            "GAoTTU1TX01BVENITUFLSU5HX1JFURC5FxIYChNNTVNfTUFUQ0hNQUtJTkdf",
            "QUNLELoXEhAKC01NU19NU0dfRU5EEKgZEhMKDk1NU19NU0dBTExfRU5EELEZ",
            "QlQKG2NvbS5jbXMuY2xpZW50Lm5ldHdvcmsuZ3Byb1ABWhVnYW1lbGliL3By",
            "b3RvYnVmL2dwcm+qAhtjb20uY21zLmNsaWVudC5uZXR3b3JrLmdwcm9iBnBy",
            "b3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(new[] {typeof(global::com.cms.client.network.gpro.USER_PROTYPE), typeof(global::com.cms.client.network.gpro.BUILD_PROTYPE), typeof(global::com.cms.client.network.gpro.PET_PROTYPE), typeof(global::com.cms.client.network.gpro.CHAT_PROTYPE), typeof(global::com.cms.client.network.gpro.MAIL_PROTYPE), typeof(global::com.cms.client.network.gpro.FRIEND_PROTYPE), typeof(global::com.cms.client.network.gpro.ITEMS_PROTYPE), typeof(global::com.cms.client.network.gpro.TASK_PROTYPE), typeof(global::com.cms.client.network.gpro.MMS_PROTYPE), }, null, null));
    }
    #endregion

  }
  #region Enums
  /// <summary>
  ///用户定义的协议
  /// </summary>
  public enum USER_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_1000")] Universal1000 = 0,
    [pbr::OriginalName("ERK_USER_NUM")] ErkUserNum = 1000,
    /// <summary>
    ///用户协议
    /// </summary>
    [pbr::OriginalName("USER_LOGIN_REQ")] UserLoginReq = 1001,
    [pbr::OriginalName("USER_LOGIN_ACK")] UserLoginAck = 1002,
    [pbr::OriginalName("USER_PROXYLOGIN_REQ")] UserProxyloginReq = 1003,
    [pbr::OriginalName("USER_PROXYLOGIN_ACK")] UserProxyloginAck = 1004,
    [pbr::OriginalName("USER_LOGOUT_NTF")] UserLogoutNtf = 1005,
    [pbr::OriginalName("USER_ROLELIST_ACK")] UserRolelistAck = 1007,
    [pbr::OriginalName("USER_ROLESELECT_REQ")] UserRoleselectReq = 1008,
    [pbr::OriginalName("USER_ROLESELECT_ACK")] UserRoleselectAck = 1009,
    [pbr::OriginalName("USER_ROLECREATE_REQ")] UserRolecreateReq = 1010,
    [pbr::OriginalName("USER_ROLECREATE_ACK")] UserRolecreateAck = 1011,
    [pbr::OriginalName("USER_ROLEDETAILA_NTF")] UserRoledetailaNtf = 1012,
    [pbr::OriginalName("USER_ROLEDETAILB_NTF")] UserRoledetailbNtf = 1013,
    [pbr::OriginalName("USER_ROLEDETAILEND_NTF")] UserRoledetailendNtf = 1014,
  }

  public enum BUILD_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_1250")] Universal1250 = 0,
    [pbr::OriginalName("BUILD_ADDITEM_REQ")] BuildAdditemReq = 1251,
    [pbr::OriginalName("BUILD_ADDITEM_ACK")] BuildAdditemAck = 1252,
    [pbr::OriginalName("BUILD_DELITEM_REQ")] BuildDelitemReq = 1253,
    [pbr::OriginalName("BUILD_DELITEM_ACK")] BuildDelitemAck = 1254,
  }

  public enum PET_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_1500")] Universal1500 = 0,
    [pbr::OriginalName("PET_ADOPTONE_REQ")] PetAdoptoneReq = 1501,
    [pbr::OriginalName("PET_ADOPTONE_ACK")] PetAdoptoneAck = 1502,
    [pbr::OriginalName("PET_RELEASEONE_REQ")] PetReleaseoneReq = 1503,
    [pbr::OriginalName("PET_RELEASEONE_ACK")] PetReleaseoneAck = 1504,
  }

  public enum CHAT_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_1750")] Universal1750 = 0,
    [pbr::OriginalName("CHAT_MSG_BEGIN")] ChatMsgBegin = 1750,
    [pbr::OriginalName("CHAT_USERCHANNELS_ACTIVE")] ChatUserchannelsActive = 1751,
    [pbr::OriginalName("CHAT_CUSTOMCHANNELID_REQ")] ChatCustomchannelidReq = 1752,
    [pbr::OriginalName("CHAT_CUSTOMCHANNELID_ACK")] ChatCustomchannelidAck = 1753,
    [pbr::OriginalName("CHAT_USERMSG_SAY")] ChatUsermsgSay = 1754,
    [pbr::OriginalName("CHAT_CHATMSG_NTF")] ChatChatmsgNtf = 1755,
    [pbr::OriginalName("CHAT_GLOBALMSG_NTF")] ChatGlobalmsgNtf = 1949,
    [pbr::OriginalName("CHAT_MSG_END")] ChatMsgEnd = 1950,
    [pbr::OriginalName("CHAT_MSGALL_END")] ChatMsgallEnd = 1999,
  }

  public enum MAIL_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_2000")] Universal2000 = 0,
    [pbr::OriginalName("MAIL_MSG_BEGIN")] MailMsgBegin = 2000,
    [pbr::OriginalName("MAIL_NEWMAIL_NTF")] MailNewmailNtf = 2001,
    [pbr::OriginalName("MAIL_MAILGET_REQ")] MailMailgetReq = 2002,
    [pbr::OriginalName("MAIL_MAILGET_ACK")] MailMailgetAck = 2003,
    [pbr::OriginalName("MAIL_USERONLINE_ACTIVE")] MailUseronlineActive = 2004,
    [pbr::OriginalName("MAIL_READMAIL_REQ")] MailReadmailReq = 2005,
    [pbr::OriginalName("MAIL_READMAIL_ACK")] MailReadmailAck = 2006,
    [pbr::OriginalName("MAIL_DELETEMAIL_REQ")] MailDeletemailReq = 2007,
    [pbr::OriginalName("MAIL_DELETEMAIL_ACK")] MailDeletemailAck = 2008,
    [pbr::OriginalName("MAIL_UNBINDATTACHS_REQ")] MailUnbindattachsReq = 2009,
    [pbr::OriginalName("MAIL_UNBINDATTACHS_ACK")] MailUnbindattachsAck = 2010,
    [pbr::OriginalName("MAIL_SYSTEMMAIL_REQ")] MailSystemmailReq = 2201,
    [pbr::OriginalName("MAIL_SYSTEMMAIL_ACK")] MailSystemmailAck = 2202,
    [pbr::OriginalName("MAIL_SYSTEMMAIL_NTF")] MailSystemmailNtf = 2203,
    [pbr::OriginalName("MAIL_NEWMAIL_REQ")] MailNewmailReq = 2204,
    [pbr::OriginalName("MAIL_NEWMAIL_ACK")] MailNewmailAck = 2205,
    [pbr::OriginalName("MAIL_MSG_END")] MailMsgEnd = 2240,
    [pbr::OriginalName("MAIL_MSGALL_END")] MailMsgallEnd = 2249,
  }

  public enum FRIEND_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_2250")] Universal2250 = 0,
    [pbr::OriginalName("FRIEND_MSG_BEGIN")] FriendMsgBegin = 2250,
    [pbr::OriginalName("FRD_FRIENDLIST_REQ")] FrdFriendlistReq = 2251,
    [pbr::OriginalName("FRD_FRIENDLIST_ACK")] FrdFriendlistAck = 2252,
    [pbr::OriginalName("FRD_FRIENDINVITE_REQ")] FrdFriendinviteReq = 2253,
    [pbr::OriginalName("FRD_FRIENDINVITE_ACK")] FrdFriendinviteAck = 2254,
    [pbr::OriginalName("FRD_INVITECONFIRM_REQ")] FrdInviteconfirmReq = 2255,
    [pbr::OriginalName("FRD_INVITECONFIRM_ACK")] FrdInviteconfirmAck = 2256,
    [pbr::OriginalName("FRD_FRIENDDELETE_REQ")] FrdFrienddeleteReq = 2257,
    [pbr::OriginalName("FRD_FRIENDDELETE_ACK")] FrdFrienddeleteAck = 2258,
    [pbr::OriginalName("FRIEND_MSG_END")] FriendMsgEnd = 2450,
    [pbr::OriginalName("FRD_FRIENDCHANGEOTHER_NTF")] FrdFriendchangeotherNtf = 2451,
    [pbr::OriginalName("FRIEND_MSGALL_END")] FriendMsgallEnd = 2499,
  }

  public enum ITEMS_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_2500")] Universal2500 = 0,
    [pbr::OriginalName("ITEM_USERBINDITEMS_REQ")] ItemUserbinditemsReq = 2501,
    [pbr::OriginalName("ITEM_USERBINDITEMS_ACK")] ItemUserbinditemsAck = 2502,
  }

  public enum TASK_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_2750")] Universal2750 = 0,
    [pbr::OriginalName("TASK_MSG_BEGIN")] TaskMsgBegin = 2750,
    [pbr::OriginalName("TASK_WAITLIST_REQ")] TaskWaitlistReq = 2751,
    [pbr::OriginalName("TASK_WAITLIST_ACK")] TaskWaitlistAck = 2752,
    [pbr::OriginalName("TASK_MYTASKLIST_REQ")] TaskMytasklistReq = 2753,
    [pbr::OriginalName("TASK_MYTASKLIST_ACK")] TaskMytasklistAck = 2754,
    [pbr::OriginalName("TASK_GETTASK_REQ")] TaskGettaskReq = 2755,
    [pbr::OriginalName("TASK_GETTASK_ACK")] TaskGettaskAck = 2756,
    [pbr::OriginalName("TASK_SUBMITTASK_REQ")] TaskSubmittaskReq = 2757,
    [pbr::OriginalName("TASK_SUBMITTASK_ACK")] TaskSubmittaskAck = 2758,
    [pbr::OriginalName("TASK_OBTAINREWARD_NTF")] TaskObtainrewardNtf = 2760,
    [pbr::OriginalName("TASK_GIVEUPTASK_REQ")] TaskGiveuptaskReq = 2761,
    [pbr::OriginalName("TASK_GIVEUPTASK_ACK")] TaskGiveuptaskAck = 2762,
    [pbr::OriginalName("TASK_MSG_END")] TaskMsgEnd = 2950,
    [pbr::OriginalName("TASK_MSGALL_END")] TaskMsgallEnd = 2999,
  }

  public enum MMS_PROTYPE {
    [pbr::OriginalName("UNIVERSAL_3000")] Universal3000 = 0,
    [pbr::OriginalName("MMS_MSG_BEGIN")] MmsMsgBegin = 3000,
    [pbr::OriginalName("MMS_MATCHMAKING_REQ")] MmsMatchmakingReq = 3001,
    [pbr::OriginalName("MMS_MATCHMAKING_ACK")] MmsMatchmakingAck = 3002,
    [pbr::OriginalName("MMS_MSG_END")] MmsMsgEnd = 3240,
    [pbr::OriginalName("MMS_MSGALL_END")] MmsMsgallEnd = 3249,
  }

  #endregion

}

#endregion Designer generated code
