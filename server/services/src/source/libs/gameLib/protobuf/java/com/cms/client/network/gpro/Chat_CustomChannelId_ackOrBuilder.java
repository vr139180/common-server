// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_chat.proto

package com.cms.client.network.gpro;

public interface Chat_CustomChannelId_ackOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Chat_CustomChannelId_ack)
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
   *0:success 1:failed
   * </pre>
   *
   * <code>int32 result = 2;</code>
   * @return The result.
   */
  int getResult();

  /**
   * <code>optional .PRO.ChatChannelInfo channel = 3;</code>
   * @return Whether the channel field is set.
   */
  boolean hasChannel();
  /**
   * <code>optional .PRO.ChatChannelInfo channel = 3;</code>
   * @return The channel.
   */
  com.cms.client.network.gpro.ChatChannelInfo getChannel();
  /**
   * <code>optional .PRO.ChatChannelInfo channel = 3;</code>
   */
  com.cms.client.network.gpro.ChatChannelInfoOrBuilder getChannelOrBuilder();
}
