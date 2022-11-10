// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

public interface ServerNodeOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.ServerNode)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   *服务器sn
   * </pre>
   *
   * <code>int64 iid = 1;</code>
   * @return The iid.
   */
  long getIid();

  /**
   * <pre>
   *对方token，用于验证
   * </pre>
   *
   * <code>int64 token = 2;</code>
   * @return The token.
   */
  long getToken();

  /**
   * <pre>
   *动态ip
   * </pre>
   *
   * <code>string ip = 3;</code>
   * @return The ip.
   */
  java.lang.String getIp();
  /**
   * <pre>
   *动态ip
   * </pre>
   *
   * <code>string ip = 3;</code>
   * @return The bytes for ip.
   */
  com.google.protobuf.ByteString
      getIpBytes();

  /**
   * <pre>
   *服务器port
   * </pre>
   *
   * <code>int32 port = 4;</code>
   * @return The port.
   */
  int getPort();

  /**
   * <pre>
   *扩展信息
   * </pre>
   *
   * <code>map&lt;string, string&gt; exts = 5;</code>
   */
  int getExtsCount();
  /**
   * <pre>
   *扩展信息
   * </pre>
   *
   * <code>map&lt;string, string&gt; exts = 5;</code>
   */
  boolean containsExts(
      java.lang.String key);
  /**
   * Use {@link #getExtsMap()} instead.
   */
  @java.lang.Deprecated
  java.util.Map<java.lang.String, java.lang.String>
  getExts();
  /**
   * <pre>
   *扩展信息
   * </pre>
   *
   * <code>map&lt;string, string&gt; exts = 5;</code>
   */
  java.util.Map<java.lang.String, java.lang.String>
  getExtsMap();
  /**
   * <pre>
   *扩展信息
   * </pre>
   *
   * <code>map&lt;string, string&gt; exts = 5;</code>
   */

  java.lang.String getExtsOrDefault(
      java.lang.String key,
      java.lang.String defaultValue);
  /**
   * <pre>
   *扩展信息
   * </pre>
   *
   * <code>map&lt;string, string&gt; exts = 5;</code>
   */

  java.lang.String getExtsOrThrow(
      java.lang.String key);

  /**
   * <pre>
   *是否router服务
   * </pre>
   *
   * <code>bool isrouter = 6;</code>
   * @return The isrouter.
   */
  boolean getIsrouter();

  /**
   * <pre>
   *是否上线
   * </pre>
   *
   * <code>bool isonline = 7;</code>
   * @return The isonline.
   */
  boolean getIsonline();
}
