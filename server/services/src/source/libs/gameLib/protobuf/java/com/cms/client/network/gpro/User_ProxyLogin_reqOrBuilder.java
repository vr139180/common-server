// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_login.proto

package com.cms.client.network.gpro;

public interface User_ProxyLogin_reqOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.User_ProxyLogin_req)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   *用户iid
   * </pre>
   *
   * <code>int64 user_iid = 1;</code>
   * @return The userIid.
   */
  long getUserIid();

  /**
   * <pre>
   *proxytoken
   * </pre>
   *
   * <code>int64 proxytoken = 2;</code>
   * @return The proxytoken.
   */
  long getProxytoken();

  /**
   * <code>int32 slot = 3;</code>
   * @return The slot.
   */
  int getSlot();
}
