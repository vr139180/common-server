// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db_internal.proto

package com.cms.client.network.gpro;

public interface DBUserHomeOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.DBUserHome)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>uint32 ver_ = 1;</code>
   * @return The ver.
   */
  int getVer();

  /**
   * <code>int64 role_iid = 2;</code>
   * @return The roleIid.
   */
  long getRoleIid();

  /**
   * <code>string home_name = 3;</code>
   * @return The homeName.
   */
  java.lang.String getHomeName();
  /**
   * <code>string home_name = 3;</code>
   * @return The bytes for homeName.
   */
  com.google.protobuf.ByteString
      getHomeNameBytes();

  /**
   * <code>int64 ground_resid = 4;</code>
   * @return The groundResid.
   */
  long getGroundResid();

  /**
   * <code>string look_at = 5;</code>
   * @return The lookAt.
   */
  java.lang.String getLookAt();
  /**
   * <code>string look_at = 5;</code>
   * @return The bytes for lookAt.
   */
  com.google.protobuf.ByteString
      getLookAtBytes();

  /**
   * <code>string geo_pos = 6;</code>
   * @return The geoPos.
   */
  java.lang.String getGeoPos();
  /**
   * <code>string geo_pos = 6;</code>
   * @return The bytes for geoPos.
   */
  com.google.protobuf.ByteString
      getGeoPosBytes();

  /**
   * <code>int32 reside_time = 7;</code>
   * @return The resideTime.
   */
  int getResideTime();

  /**
   * <code>int32 last_residedate = 8;</code>
   * @return The lastResidedate.
   */
  int getLastResidedate();

  /**
   * <code>int32 levels = 9;</code>
   * @return The levels.
   */
  int getLevels();
}
