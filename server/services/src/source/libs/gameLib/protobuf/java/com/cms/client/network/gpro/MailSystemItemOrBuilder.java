// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: global_internal.proto

package com.cms.client.network.gpro;

public interface MailSystemItemOrBuilder extends
    // @@protoc_insertion_point(interface_extends:PRO.MailSystemItem)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>int64 iid = 1;</code>
   * @return The iid.
   */
  long getIid();

  /**
   * <code>int64 sender_iid = 2;</code>
   * @return The senderIid.
   */
  long getSenderIid();

  /**
   * <code>string title = 3;</code>
   * @return The title.
   */
  java.lang.String getTitle();
  /**
   * <code>string title = 3;</code>
   * @return The bytes for title.
   */
  com.google.protobuf.ByteString
      getTitleBytes();

  /**
   * <code>string contents = 4;</code>
   * @return The contents.
   */
  java.lang.String getContents();
  /**
   * <code>string contents = 4;</code>
   * @return The bytes for contents.
   */
  com.google.protobuf.ByteString
      getContentsBytes();

  /**
   * <pre>
   *0:无附件 1:有附件
   * </pre>
   *
   * <code>int32 attachment = 5;</code>
   * @return The attachment.
   */
  int getAttachment();

  /**
   * <pre>
   *附件json字符串
   * </pre>
   *
   * <code>string attachinfo = 6;</code>
   * @return The attachinfo.
   */
  java.lang.String getAttachinfo();
  /**
   * <pre>
   *附件json字符串
   * </pre>
   *
   * <code>string attachinfo = 6;</code>
   * @return The bytes for attachinfo.
   */
  com.google.protobuf.ByteString
      getAttachinfoBytes();

  /**
   * <pre>
   *创建时间second
   * </pre>
   *
   * <code>int32 createtime = 7;</code>
   * @return The createtime.
   */
  int getCreatetime();
}
