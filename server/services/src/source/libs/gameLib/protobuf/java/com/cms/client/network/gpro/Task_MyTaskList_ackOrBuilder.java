// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_system.proto

package com.cms.client.network.gpro;

public interface Task_MyTaskList_ackOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Task_MyTaskList_ack)
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
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   * @return Whether the groups field is set.
   */
  boolean hasGroups();
  /**
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   * @return The groups.
   */
  com.cms.client.network.gpro.DBUserTaskGroups getGroups();
  /**
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   */
  com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder getGroupsOrBuilder();

  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   * @return Whether the tasks field is set.
   */
  boolean hasTasks();
  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   * @return The tasks.
   */
  com.cms.client.network.gpro.DBUserTasks getTasks();
  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   */
  com.cms.client.network.gpro.DBUserTasksOrBuilder getTasksOrBuilder();
}