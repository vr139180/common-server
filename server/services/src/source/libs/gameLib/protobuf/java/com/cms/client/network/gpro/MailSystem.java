// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mail_system.proto

package com.cms.client.network.gpro;

public final class MailSystem {
  private MailSystem() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_MailUserBox_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_MailUserBox_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_NewMail_ntf_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_NewMail_ntf_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_MailGet_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_MailGet_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_MailGet_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_MailGet_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_UserOnline_active_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_UserOnline_active_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_ReadMail_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_ReadMail_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_ReadMail_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_ReadMail_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_DeleteMail_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_DeleteMail_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_DeleteMail_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_DeleteMail_ack_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_UnBindAttachs_req_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_UnBindAttachs_req_fieldAccessorTable;
  static final com.google.protobuf.Descriptors.Descriptor
    internal_static_PRO_Mail_UnBindAttachs_ack_descriptor;
  static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_PRO_Mail_UnBindAttachs_ack_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\021mail_system.proto\022\003PRO\032\025global_interna" +
      "l.proto\"Z\n\013MailUserBox\022\014\n\004ver_\030\001 \001(\005\022\020\n\010" +
      "role_iid\030\002 \001(\003\022\024\n\014sysmail_sync\030\003 \001(\003\022\025\n\r" +
      "lastlist_mail\030\004 \001(\003\"B\n\020Mail_NewMail_ntf\022" +
      "#\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210\001\001B\t\n\007" +
      "_utoken\"n\n\020Mail_MailGet_req\022#\n\006utoken\030\001 " +
      "\001(\0132\016.PRO.UserTokenH\000\210\001\001\022\017\n\007mailiid\030\002 \001(" +
      "\003\022\014\n\004next\030\003 \001(\010\022\013\n\003num\030\004 \001(\005B\t\n\007_utoken\"" +
      "u\n\020Mail_MailGet_ack\022#\n\006utoken\030\001 \001(\0132\016.PR" +
      "O.UserTokenH\000\210\001\001\022\"\n\005mails\030\002 \003(\0132\023.PRO.Ma" +
      "ilNormalItem\022\r\n\005totle\030\003 \001(\005B\t\n\007_utoken\"s" +
      "\n\026Mail_UserOnline_active\022#\n\006utoken\030\001 \001(\013" +
      "2\016.PRO.UserTokenH\000\210\001\001\022\024\n\014last_mailiid\030\002 " +
      "\001(\003\022\023\n\013firstactive\030\003 \001(\010B\t\n\007_utoken\"U\n\021M" +
      "ail_ReadMail_req\022#\n\006utoken\030\001 \001(\0132\016.PRO.U" +
      "serTokenH\000\210\001\001\022\020\n\010mail_iid\030\002 \001(\003B\t\n\007_utok" +
      "en\"e\n\021Mail_ReadMail_ack\022#\n\006utoken\030\001 \001(\0132" +
      "\016.PRO.UserTokenH\000\210\001\001\022\016\n\006result\030\002 \001(\005\022\020\n\010" +
      "mail_iid\030\003 \001(\003B\t\n\007_utoken\"W\n\023Mail_Delete" +
      "Mail_req\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserToken" +
      "H\000\210\001\001\022\020\n\010mail_iid\030\002 \001(\003B\t\n\007_utoken\"g\n\023Ma" +
      "il_DeleteMail_ack\022#\n\006utoken\030\001 \001(\0132\016.PRO." +
      "UserTokenH\000\210\001\001\022\016\n\006result\030\002 \001(\005\022\020\n\010mail_i" +
      "id\030\003 \001(\003B\t\n\007_utoken\"Z\n\026Mail_UnBindAttach" +
      "s_req\022#\n\006utoken\030\001 \001(\0132\016.PRO.UserTokenH\000\210" +
      "\001\001\022\020\n\010mail_iid\030\002 \001(\003B\t\n\007_utoken\"j\n\026Mail_" +
      "UnBindAttachs_ack\022#\n\006utoken\030\001 \001(\0132\016.PRO." +
      "UserTokenH\000\210\001\001\022\016\n\006result\030\002 \001(\005\022\020\n\010mail_i" +
      "id\030\003 \001(\003B\t\n\007_utoken*X\n\010MailType\022\021\n\rMailT" +
      "ype_Init\020\000\022\023\n\017MailType_System\020\001\022\022\n\016MailT" +
      "ype_Union\020\002\022\020\n\014MailType_Max\020\003BT\n\033com.cms" +
      ".client.network.gproP\001Z\025gamelib/protobuf" +
      "/gpro\252\002\033com.cms.client.network.gprob\006pro" +
      "to3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          com.cms.client.network.gpro.GlobalInternal.getDescriptor(),
        });
    internal_static_PRO_MailUserBox_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_PRO_MailUserBox_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_MailUserBox_descriptor,
        new java.lang.String[] { "Ver", "RoleIid", "SysmailSync", "LastlistMail", });
    internal_static_PRO_Mail_NewMail_ntf_descriptor =
      getDescriptor().getMessageTypes().get(1);
    internal_static_PRO_Mail_NewMail_ntf_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_NewMail_ntf_descriptor,
        new java.lang.String[] { "Utoken", "Utoken", });
    internal_static_PRO_Mail_MailGet_req_descriptor =
      getDescriptor().getMessageTypes().get(2);
    internal_static_PRO_Mail_MailGet_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_MailGet_req_descriptor,
        new java.lang.String[] { "Utoken", "Mailiid", "Next", "Num", "Utoken", });
    internal_static_PRO_Mail_MailGet_ack_descriptor =
      getDescriptor().getMessageTypes().get(3);
    internal_static_PRO_Mail_MailGet_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_MailGet_ack_descriptor,
        new java.lang.String[] { "Utoken", "Mails", "Totle", "Utoken", });
    internal_static_PRO_Mail_UserOnline_active_descriptor =
      getDescriptor().getMessageTypes().get(4);
    internal_static_PRO_Mail_UserOnline_active_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_UserOnline_active_descriptor,
        new java.lang.String[] { "Utoken", "LastMailiid", "Firstactive", "Utoken", });
    internal_static_PRO_Mail_ReadMail_req_descriptor =
      getDescriptor().getMessageTypes().get(5);
    internal_static_PRO_Mail_ReadMail_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_ReadMail_req_descriptor,
        new java.lang.String[] { "Utoken", "MailIid", "Utoken", });
    internal_static_PRO_Mail_ReadMail_ack_descriptor =
      getDescriptor().getMessageTypes().get(6);
    internal_static_PRO_Mail_ReadMail_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_ReadMail_ack_descriptor,
        new java.lang.String[] { "Utoken", "Result", "MailIid", "Utoken", });
    internal_static_PRO_Mail_DeleteMail_req_descriptor =
      getDescriptor().getMessageTypes().get(7);
    internal_static_PRO_Mail_DeleteMail_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_DeleteMail_req_descriptor,
        new java.lang.String[] { "Utoken", "MailIid", "Utoken", });
    internal_static_PRO_Mail_DeleteMail_ack_descriptor =
      getDescriptor().getMessageTypes().get(8);
    internal_static_PRO_Mail_DeleteMail_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_DeleteMail_ack_descriptor,
        new java.lang.String[] { "Utoken", "Result", "MailIid", "Utoken", });
    internal_static_PRO_Mail_UnBindAttachs_req_descriptor =
      getDescriptor().getMessageTypes().get(9);
    internal_static_PRO_Mail_UnBindAttachs_req_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_UnBindAttachs_req_descriptor,
        new java.lang.String[] { "Utoken", "MailIid", "Utoken", });
    internal_static_PRO_Mail_UnBindAttachs_ack_descriptor =
      getDescriptor().getMessageTypes().get(10);
    internal_static_PRO_Mail_UnBindAttachs_ack_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_PRO_Mail_UnBindAttachs_ack_descriptor,
        new java.lang.String[] { "Utoken", "Result", "MailIid", "Utoken", });
    com.cms.client.network.gpro.GlobalInternal.getDescriptor();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
