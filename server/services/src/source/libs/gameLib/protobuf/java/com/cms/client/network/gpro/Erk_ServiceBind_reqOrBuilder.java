// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

public interface Erk_ServiceBind_reqOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Erk_ServiceBind_req)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   *服务器类型 NETSVR_TYPE_ENUM定义
   * </pre>
   *
   * <code>int32 svr_type = 1;</code>
   * @return The svrType.
   */
  int getSvrType();

  /**
   * <pre>
   *service编号，由eureka保证唯一
   * </pre>
   *
   * <code>int64 iid = 2;</code>
   * @return The iid.
   */
  long getIid();

  /**
   * <code>int64 token = 3;</code>
   * @return The token.
   */
  long getToken();
}
