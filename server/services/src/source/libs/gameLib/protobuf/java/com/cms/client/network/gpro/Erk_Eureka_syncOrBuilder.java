// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

public interface Erk_Eureka_syncOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Erk_Eureka_sync)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   *请求方iid
   * </pre>
   *
   * <code>int64 myiid = 1;</code>
   * @return The myiid.
   */
  long getMyiid();

  /**
   * <pre>
   *存在的eureka服务
   * </pre>
   *
   * <code>repeated int64 exists = 2;</code>
   * @return A list containing the exists.
   */
  java.util.List<java.lang.Long> getExistsList();
  /**
   * <pre>
   *存在的eureka服务
   * </pre>
   *
   * <code>repeated int64 exists = 2;</code>
   * @return The count of exists.
   */
  int getExistsCount();
  /**
   * <pre>
   *存在的eureka服务
   * </pre>
   *
   * <code>repeated int64 exists = 2;</code>
   * @param index The index of the element to return.
   * @return The exists at the given index.
   */
  long getExists(int index);
}
