// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: friend_system.proto

package com.cms.client.network.gpro;

public interface Frd_FriendInvite_reqOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Frd_FriendInvite_req)
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
   * <pre>
   *被邀请人
   * </pre>
   *
   * <code>int64 invite_iid = 2;</code>
   * @return The inviteIid.
   */
  long getInviteIid();
}
