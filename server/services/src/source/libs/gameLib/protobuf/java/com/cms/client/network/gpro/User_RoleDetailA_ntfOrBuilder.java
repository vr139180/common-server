// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_login.proto

package com.cms.client.network.gpro;

public interface User_RoleDetailA_ntfOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.User_RoleDetailA_ntf)
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
   * <code>.PRO.DBUserHome home = 2;</code>
   * @return Whether the home field is set.
   */
  boolean hasHome();
  /**
   * <code>.PRO.DBUserHome home = 2;</code>
   * @return The home.
   */
  com.cms.client.network.gpro.DBUserHome getHome();
  /**
   * <code>.PRO.DBUserHome home = 2;</code>
   */
  com.cms.client.network.gpro.DBUserHomeOrBuilder getHomeOrBuilder();

  /**
   * <code>.PRO.DBUserHomeStructure homeitems = 3;</code>
   * @return Whether the homeitems field is set.
   */
  boolean hasHomeitems();
  /**
   * <code>.PRO.DBUserHomeStructure homeitems = 3;</code>
   * @return The homeitems.
   */
  com.cms.client.network.gpro.DBUserHomeStructure getHomeitems();
  /**
   * <code>.PRO.DBUserHomeStructure homeitems = 3;</code>
   */
  com.cms.client.network.gpro.DBUserHomeStructureOrBuilder getHomeitemsOrBuilder();
}
