// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mail_system.proto

package com.cms.client.network.gpro;

public interface Mail_UnBindAttachs_reqOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Mail_UnBindAttachs_req)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   * @return Whether the utoken field is set.
   */
  boolean hasUtoken();
  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   * @return The utoken.
   */
  com.cms.client.network.gpro.UserToken getUtoken();
  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   */
  com.cms.client.network.gpro.UserTokenOrBuilder getUtokenOrBuilder();

  /**
   * <code>int64 mail_iid = 2;</code>
   * @return The mailIid.
   */
  long getMailIid();
}
