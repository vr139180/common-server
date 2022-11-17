// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_login.proto

package com.cms.client.network.gpro;

public final class UserLogin {
  private UserLogin() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_Login_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_Login_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_Login_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_Login_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_ReLogin_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_ReLogin_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_ReLogin_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_ReLogin_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_Logout_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_Logout_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_GateLost_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_GateLost_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_Active_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_Active_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleList_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleList_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleList_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleList_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleCreate_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleCreate_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleCreate_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleCreate_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleSelect_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleSelect_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleSelect_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleSelect_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleDetailA_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleDetailA_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleDetailB_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleDetailB_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_User_RoleDetailEnd_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_User_RoleDetailEnd_ntf_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\020user_login.proto\022\003PRO\032\021db_internal.pro" +
      "to\"\230\001\n\016User_Login_req\022\014\n\004type\030\001 \001(\005\022\024\n\007a" +
      "ccount\030\002 \001(\tH\000\210\001\001\022\020\n\003pwd\030\003 \001(\tH\001\210\001\001\022\022\n\005t" +
      "oken\030\004 \001(\003H\002\210\001\001\022\023\n\006userid\030\005 \001(\003H\003\210\001\001B\n\n\010" +
      "_accountB\006\n\004_pwdB\010\n\006_tokenB\t\n\007_userid\"z\n" +
      "\016User_Login_ack\022\014\n\004type\030\001 \001(\005\022\016\n\006result\030" +
      "\002 \001(\005\022\025\n\010user_iid\030\003 \001(\003H\000\210\001\001\022\027\n\nlogintok" +
      "en\030\004 \001(\003H\001\210\001\001B\013\n\t_user_iidB\r\n\013_logintoke" +
      "n\"8\n\020User_ReLogin_req\022\020\n\010user_iid\030\001 \001(\003\022" +
      "\022\n\nlogintoken\030\002 \001(\003\"n\n\020User_ReLogin_ack\022" +
      "\016\n\006result\030\001 \001(\005\022\025\n\010user_iid\030\002 \001(\003H\000\210\001\001\022\027" +
      "\n\nlogintoken\030\003 \001(\003H\001\210\001\001B\013\n\t_user_iidB\r\n\013" +
      "_logintoken\"\021\n\017User_Logout_ntf\"\023\n\021User_G" +
      "ateLost_ntf\"\021\n\017User_Active_ntf\"\023\n\021User_R" +
      "oleList_req\"4\n\021User_RoleList_ack\022\037\n\005role" +
      "s\030\001 \001(\0132\020.PRO.DBUserRoles\"\'\n\023User_RoleCr" +
      "eate_req\022\020\n\010nickname\030\001 \001(\t\"I\n\023User_RoleC" +
      "reate_ack\022\016\n\006result\030\001 \001(\005\022\025\n\010role_iid\030\002 " +
      "\001(\003H\000\210\001\001B\013\n\t_role_iid\"\'\n\023User_RoleSelect" +
      "_req\022\020\n\010role_iid\030\001 \001(\003\"7\n\023User_RoleSelec" +
      "t_ack\022\016\n\006result\030\001 \001(\005\022\020\n\010role_iid\030\002 \001(\003\"" +
      "b\n\024User_RoleDetailA_ntf\022\035\n\004home\030\001 \001(\0132\017." +
      "PRO.DBUserHome\022+\n\thomeitems\030\002 \001(\0132\030.PRO." +
      "DBUserHomeStructure\"5\n\024User_RoleDetailB_" +
      "ntf\022\035\n\004pets\030\001 \001(\0132\017.PRO.DBUserPets\"\030\n\026Us" +
      "er_RoleDetailEnd_ntfBT\n\033com.cms.client.n" +
      "etwork.gproP\001Z\025gamelib/protobuf/gpro\252\002\033c" +
      "om.cms.client.network.gprob\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          com.cms.client.network.gpro.DbInternal.getDescriptor(),
        });
    internal_static_PRO_User_Login_req_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_PRO_User_Login_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_Login_req_descriptor,
        new java.lang.String[] { "Type", "Account", "Pwd", "Token", "Userid", "Account", "Pwd", "Token", "Userid", });
    internal_static_PRO_User_Login_ack_descriptor =
      getDescriptor().getMessageTypes().get(1);
    internal_static_PRO_User_Login_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_Login_ack_descriptor,
        new java.lang.String[] { "Type", "Result", "UserIid", "Logintoken", "UserIid", "Logintoken", });
    internal_static_PRO_User_ReLogin_req_descriptor =
      getDescriptor().getMessageTypes().get(2);
    internal_static_PRO_User_ReLogin_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_ReLogin_req_descriptor,
        new java.lang.String[] { "UserIid", "Logintoken", });
    internal_static_PRO_User_ReLogin_ack_descriptor =
      getDescriptor().getMessageTypes().get(3);
    internal_static_PRO_User_ReLogin_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_ReLogin_ack_descriptor,
        new java.lang.String[] { "Result", "UserIid", "Logintoken", "UserIid", "Logintoken", });
    internal_static_PRO_User_Logout_ntf_descriptor =
      getDescriptor().getMessageTypes().get(4);
    internal_static_PRO_User_Logout_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_Logout_ntf_descriptor,
        new java.lang.String[] { });
    internal_static_PRO_User_GateLost_ntf_descriptor =
      getDescriptor().getMessageTypes().get(5);
    internal_static_PRO_User_GateLost_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_GateLost_ntf_descriptor,
        new java.lang.String[] { });
    internal_static_PRO_User_Active_ntf_descriptor =
      getDescriptor().getMessageTypes().get(6);
    internal_static_PRO_User_Active_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_Active_ntf_descriptor,
        new java.lang.String[] { });
    internal_static_PRO_User_RoleList_req_descriptor =
      getDescriptor().getMessageTypes().get(7);
    internal_static_PRO_User_RoleList_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleList_req_descriptor,
        new java.lang.String[] { });
    internal_static_PRO_User_RoleList_ack_descriptor =
      getDescriptor().getMessageTypes().get(8);
    internal_static_PRO_User_RoleList_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleList_ack_descriptor,
        new java.lang.String[] { "Roles", });
    internal_static_PRO_User_RoleCreate_req_descriptor =
      getDescriptor().getMessageTypes().get(9);
    internal_static_PRO_User_RoleCreate_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleCreate_req_descriptor,
        new java.lang.String[] { "Nickname", });
    internal_static_PRO_User_RoleCreate_ack_descriptor =
      getDescriptor().getMessageTypes().get(10);
    internal_static_PRO_User_RoleCreate_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleCreate_ack_descriptor,
        new java.lang.String[] { "Result", "RoleIid", "RoleIid", });
    internal_static_PRO_User_RoleSelect_req_descriptor =
      getDescriptor().getMessageTypes().get(11);
    internal_static_PRO_User_RoleSelect_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleSelect_req_descriptor,
        new java.lang.String[] { "RoleIid", });
    internal_static_PRO_User_RoleSelect_ack_descriptor =
      getDescriptor().getMessageTypes().get(12);
    internal_static_PRO_User_RoleSelect_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleSelect_ack_descriptor,
        new java.lang.String[] { "Result", "RoleIid", });
    internal_static_PRO_User_RoleDetailA_ntf_descriptor =
      getDescriptor().getMessageTypes().get(13);
    internal_static_PRO_User_RoleDetailA_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleDetailA_ntf_descriptor,
        new java.lang.String[] { "Home", "Homeitems", });
    internal_static_PRO_User_RoleDetailB_ntf_descriptor =
      getDescriptor().getMessageTypes().get(14);
    internal_static_PRO_User_RoleDetailB_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleDetailB_ntf_descriptor,
        new java.lang.String[] { "Pets", });
    internal_static_PRO_User_RoleDetailEnd_ntf_descriptor =
      getDescriptor().getMessageTypes().get(15);
    internal_static_PRO_User_RoleDetailEnd_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_User_RoleDetailEnd_ntf_descriptor,
        new java.lang.String[] { });
    com.cms.client.network.gpro.DbInternal.getDescriptor();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
