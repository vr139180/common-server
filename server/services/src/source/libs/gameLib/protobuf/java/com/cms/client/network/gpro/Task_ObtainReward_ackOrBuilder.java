// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_system.proto

package com.cms.client.network.gpro;

public interface Task_ObtainReward_ackOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Task_ObtainReward_ack)
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
   * <code>int32 task_iid = 2;</code>
   * @return The taskIid.
   */
  int getTaskIid();

  /**
   * <pre>
   *0:success 
   * </pre>
   *
   * <code>int32 result = 3;</code>
   * @return The result.
   */
  int getResult();
}
