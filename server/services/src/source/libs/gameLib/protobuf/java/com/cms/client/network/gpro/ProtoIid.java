// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid.proto

package com.cms.client.network.gpro;

public final class ProtoIid {
  private ProtoIid() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\017proto_iid.proto\022\003PRO*\374\002\n\014USER_PROTYPE\022" +
      "\022\n\016UNIVERSAL_1000\020\000\022\021\n\014ERK_USER_NUM\020\350\007\022\023" +
      "\n\016USER_LOGIN_REQ\020\351\007\022\023\n\016USER_LOGIN_ACK\020\352\007" +
      "\022\030\n\023USER_PROXYLOGIN_REQ\020\353\007\022\030\n\023USER_PROXY" +
      "LOGIN_ACK\020\354\007\022\024\n\017USER_LOGOUT_NTF\020\355\007\022\026\n\021US" +
      "ER_ROLELIST_ACK\020\357\007\022\030\n\023USER_ROLESELECT_RE" +
      "Q\020\360\007\022\030\n\023USER_ROLESELECT_ACK\020\361\007\022\030\n\023USER_R" +
      "OLECREATE_REQ\020\362\007\022\030\n\023USER_ROLECREATE_ACK\020" +
      "\363\007\022\031\n\024USER_ROLEDETAILA_NTF\020\364\007\022\031\n\024USER_RO" +
      "LEDETAILB_NTF\020\365\007\022\033\n\026USER_ROLEDETAILEND_N" +
      "TF\020\366\007*\203\001\n\rBUILD_PROTYPE\022\022\n\016UNIVERSAL_125" +
      "0\020\000\022\026\n\021BUILD_ADDITEM_REQ\020\343\t\022\026\n\021BUILD_ADD" +
      "ITEM_ACK\020\344\t\022\026\n\021BUILD_DELITEM_REQ\020\345\t\022\026\n\021B" +
      "UILD_DELITEM_ACK\020\346\t*\201\001\n\013PET_PROTYPE\022\022\n\016U" +
      "NIVERSAL_1500\020\000\022\025\n\020PET_ADOPTONE_REQ\020\335\013\022\025" +
      "\n\020PET_ADOPTONE_ACK\020\336\013\022\027\n\022PET_RELEASEONE_" +
      "REQ\020\337\013\022\027\n\022PET_RELEASEONE_ACK\020\340\013*\204\002\n\014CHAT" +
      "_PROTYPE\022\022\n\016UNIVERSAL_1750\020\000\022\023\n\016CHAT_MSG" +
      "_BEGIN\020\326\r\022\035\n\030CHAT_USERCHANNELS_ACTIVE\020\327\r" +
      "\022\035\n\030CHAT_CUSTOMCHANNELID_REQ\020\330\r\022\035\n\030CHAT_" +
      "CUSTOMCHANNELID_ACK\020\331\r\022\025\n\020CHAT_USERMSG_S" +
      "AY\020\332\r\022\025\n\020CHAT_CHATMSG_NTF\020\333\r\022\027\n\022CHAT_GLO" +
      "BALMSG_NTF\020\235\017\022\021\n\014CHAT_MSG_END\020\236\017\022\024\n\017CHAT" +
      "_MSGALL_END\020\317\017*\334\003\n\014MAIL_PROTYPE\022\022\n\016UNIVE" +
      "RSAL_2000\020\000\022\023\n\016MAIL_MSG_BEGIN\020\320\017\022\025\n\020MAIL" +
      "_NEWMAIL_NTF\020\321\017\022\025\n\020MAIL_MAILGET_REQ\020\322\017\022\025" +
      "\n\020MAIL_MAILGET_ACK\020\323\017\022\033\n\026MAIL_USERONLINE" +
      "_ACTIVE\020\324\017\022\026\n\021MAIL_READMAIL_REQ\020\325\017\022\026\n\021MA" +
      "IL_READMAIL_ACK\020\326\017\022\030\n\023MAIL_DELETEMAIL_RE" +
      "Q\020\327\017\022\030\n\023MAIL_DELETEMAIL_ACK\020\330\017\022\033\n\026MAIL_U" +
      "NBINDATTACHS_REQ\020\331\017\022\033\n\026MAIL_UNBINDATTACH" +
      "S_ACK\020\332\017\022\030\n\023MAIL_SYSTEMMAIL_REQ\020\231\021\022\030\n\023MA" +
      "IL_SYSTEMMAIL_ACK\020\232\021\022\030\n\023MAIL_SYSTEMMAIL_" +
      "NTF\020\233\021\022\025\n\020MAIL_NEWMAIL_REQ\020\234\021\022\025\n\020MAIL_NE" +
      "WMAIL_ACK\020\235\021\022\021\n\014MAIL_MSG_END\020\300\021\022\024\n\017MAIL_" +
      "MSGALL_END\020\311\021*\336\002\n\016FRIEND_PROTYPE\022\022\n\016UNIV" +
      "ERSAL_2250\020\000\022\025\n\020FRIEND_MSG_BEGIN\020\312\021\022\027\n\022F" +
      "RD_FRIENDLIST_REQ\020\313\021\022\027\n\022FRD_FRIENDLIST_A" +
      "CK\020\314\021\022\031\n\024FRD_FRIENDINVITE_REQ\020\315\021\022\031\n\024FRD_" +
      "FRIENDINVITE_ACK\020\316\021\022\032\n\025FRD_INVITECONFIRM" +
      "_REQ\020\317\021\022\032\n\025FRD_INVITECONFIRM_ACK\020\320\021\022\031\n\024F" +
      "RD_FRIENDDELETE_REQ\020\321\021\022\031\n\024FRD_FRIENDDELE" +
      "TE_ACK\020\322\021\022\023\n\016FRIEND_MSG_END\020\222\023\022\036\n\031FRD_FR" +
      "IENDCHANGEOTHER_NTF\020\223\023\022\026\n\021FRIEND_MSGALL_" +
      "END\020\303\023*]\n\rITEMS_PROTYPE\022\022\n\016UNIVERSAL_250" +
      "0\020\000\022\033\n\026ITEM_USERBINDITEMS_REQ\020\305\023\022\033\n\026ITEM" +
      "_USERBINDITEMS_ACK\020\306\023*:\n\014TASK_PROTYPE\022\022\n" +
      "\016UNIVERSAL_2750\020\000\022\026\n\021TASK_NEWTASKS_NTF\020\277" +
      "\025BT\n\033com.cms.client.network.gproP\001Z\025game" +
      "lib/protobuf/gpro\252\002\033com.cms.client.netwo" +
      "rk.gprob\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
  }

  // @@protoc_insertion_point(outer_class_scope)
}
