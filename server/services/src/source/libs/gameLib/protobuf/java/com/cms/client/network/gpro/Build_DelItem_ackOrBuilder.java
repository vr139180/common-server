// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_building.proto

package com.cms.client.network.gpro;

public interface Build_DelItem_ackOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.Build_DelItem_ack)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>int64 building_iid = 1;</code>
   * @return The buildingIid.
   */
  long getBuildingIid();

  /**
   * <pre>
   *0 success 1 fail
   * </pre>
   *
   * <code>int32 result = 2;</code>
   * @return The result.
   */
  int getResult();
}
