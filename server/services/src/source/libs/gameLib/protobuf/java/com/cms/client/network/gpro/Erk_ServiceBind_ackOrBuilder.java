// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

public interface Erk_ServiceBind_ackOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Erk_ServiceBind_ack)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   *0:成功 1:失败 2:服务不存在
   * </pre>
   *
   * <code>int32 result = 1;</code>
   * @return The result.
   */
  int getResult();

  /**
   * <code>optional int64 eurekaiid = 2;</code>
   * @return Whether the eurekaiid field is set.
   */
  boolean hasEurekaiid();
  /**
   * <code>optional int64 eurekaiid = 2;</code>
   * @return The eurekaiid.
   */
  long getEurekaiid();

  /**
   * <code>optional int64 eurekatoken = 3;</code>
   * @return Whether the eurekatoken field is set.
   */
  boolean hasEurekatoken();
  /**
   * <code>optional int64 eurekatoken = 3;</code>
   * @return The eurekatoken.
   */
  long getEurekatoken();
}
