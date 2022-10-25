// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mail_system.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.Mail_UnBindAttachs_ack}
 */
public final class Mail_UnBindAttachs_ack extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Mail_UnBindAttachs_ack)
    Mail_UnBindAttachs_ackOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Mail_UnBindAttachs_ack.newBuilder() to construct.
  private Mail_UnBindAttachs_ack(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Mail_UnBindAttachs_ack() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Mail_UnBindAttachs_ack();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Mail_UnBindAttachs_ack(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          case 8: {

            result_ = input.readInt32();
            break;
          }
          case 16: {

            mailIid_ = input.readInt64();
            break;
          }
          default: {
            if (!parseUnknownField(
                input, unknownFields, extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e).setUnfinishedMessage(this);
    } finally {
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UnBindAttachs_ack_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UnBindAttachs_ack_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Mail_UnBindAttachs_ack.class, com.cms.client.network.gpro.Mail_UnBindAttachs_ack.Builder.class);
  }

  public static final int RESULT_FIELD_NUMBER = 1;
  private int result_;
  /**
   * <pre>
   *0:success 1:not exist 2:already unbinded
   * </pre>
   *
   * <code>int32 result = 1;</code>
   * @return The result.
   */
  @java.lang.Override
  public int getResult() {
    return result_;
  }

  public static final int MAIL_IID_FIELD_NUMBER = 2;
  private long mailIid_;
  /**
   * <code>int64 mail_iid = 2;</code>
   * @return The mailIid.
   */
  @java.lang.Override
  public long getMailIid() {
    return mailIid_;
  }

  private byte memoizedIsInitialized = -1;
  @java.lang.Override
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  @java.lang.Override
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (result_ != 0) {
      output.writeInt32(1, result_);
    }
    if (mailIid_ != 0L) {
      output.writeInt64(2, mailIid_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (result_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(1, result_);
    }
    if (mailIid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, mailIid_);
    }
    size += unknownFields.getSerializedSize();
    memoizedSize = size;
    return size;
  }

  @java.lang.Override
  public boolean equals(final java.lang.Object obj) {
    if (obj == this) {
     return true;
    }
    if (!(obj instanceof com.cms.client.network.gpro.Mail_UnBindAttachs_ack)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Mail_UnBindAttachs_ack other = (com.cms.client.network.gpro.Mail_UnBindAttachs_ack) obj;

    if (getResult()
        != other.getResult()) return false;
    if (getMailIid()
        != other.getMailIid()) return false;
    if (!unknownFields.equals(other.unknownFields)) return false;
    return true;
  }

  @java.lang.Override
  public int hashCode() {
    if (memoizedHashCode != 0) {
      return memoizedHashCode;
    }
    int hash = 41;
    hash = (19 * hash) + getDescriptor().hashCode();
    hash = (37 * hash) + RESULT_FIELD_NUMBER;
    hash = (53 * hash) + getResult();
    hash = (37 * hash) + MAIL_IID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getMailIid());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }

  @java.lang.Override
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder() {
    return DEFAULT_INSTANCE.toBuilder();
  }
  public static Builder newBuilder(com.cms.client.network.gpro.Mail_UnBindAttachs_ack prototype) {
    return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
  }
  @java.lang.Override
  public Builder toBuilder() {
    return this == DEFAULT_INSTANCE
        ? new Builder() : new Builder().mergeFrom(this);
  }

  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * Protobuf type {@code PRO.Mail_UnBindAttachs_ack}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Mail_UnBindAttachs_ack)
      com.cms.client.network.gpro.Mail_UnBindAttachs_ackOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UnBindAttachs_ack_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UnBindAttachs_ack_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Mail_UnBindAttachs_ack.class, com.cms.client.network.gpro.Mail_UnBindAttachs_ack.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Mail_UnBindAttachs_ack.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessageV3
              .alwaysUseFieldBuilders) {
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      result_ = 0;

      mailIid_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UnBindAttachs_ack_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UnBindAttachs_ack getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Mail_UnBindAttachs_ack.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UnBindAttachs_ack build() {
      com.cms.client.network.gpro.Mail_UnBindAttachs_ack result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UnBindAttachs_ack buildPartial() {
      com.cms.client.network.gpro.Mail_UnBindAttachs_ack result = new com.cms.client.network.gpro.Mail_UnBindAttachs_ack(this);
      result.result_ = result_;
      result.mailIid_ = mailIid_;
      onBuilt();
      return result;
    }

    @java.lang.Override
    public Builder clone() {
      return super.clone();
    }
    @java.lang.Override
    public Builder setField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.setField(field, value);
    }
    @java.lang.Override
    public Builder clearField(
        com.google.protobuf.Descriptors.FieldDescriptor field) {
      return super.clearField(field);
    }
    @java.lang.Override
    public Builder clearOneof(
        com.google.protobuf.Descriptors.OneofDescriptor oneof) {
      return super.clearOneof(oneof);
    }
    @java.lang.Override
    public Builder setRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        int index, java.lang.Object value) {
      return super.setRepeatedField(field, index, value);
    }
    @java.lang.Override
    public Builder addRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.addRepeatedField(field, value);
    }
    @java.lang.Override
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof com.cms.client.network.gpro.Mail_UnBindAttachs_ack) {
        return mergeFrom((com.cms.client.network.gpro.Mail_UnBindAttachs_ack)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Mail_UnBindAttachs_ack other) {
      if (other == com.cms.client.network.gpro.Mail_UnBindAttachs_ack.getDefaultInstance()) return this;
      if (other.getResult() != 0) {
        setResult(other.getResult());
      }
      if (other.getMailIid() != 0L) {
        setMailIid(other.getMailIid());
      }
      this.mergeUnknownFields(other.unknownFields);
      onChanged();
      return this;
    }

    @java.lang.Override
    public final boolean isInitialized() {
      return true;
    }

    @java.lang.Override
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      com.cms.client.network.gpro.Mail_UnBindAttachs_ack parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Mail_UnBindAttachs_ack) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private int result_ ;
    /**
     * <pre>
     *0:success 1:not exist 2:already unbinded
     * </pre>
     *
     * <code>int32 result = 1;</code>
     * @return The result.
     */
    @java.lang.Override
    public int getResult() {
      return result_;
    }
    /**
     * <pre>
     *0:success 1:not exist 2:already unbinded
     * </pre>
     *
     * <code>int32 result = 1;</code>
     * @param value The result to set.
     * @return This builder for chaining.
     */
    public Builder setResult(int value) {
      
      result_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *0:success 1:not exist 2:already unbinded
     * </pre>
     *
     * <code>int32 result = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearResult() {
      
      result_ = 0;
      onChanged();
      return this;
    }

    private long mailIid_ ;
    /**
     * <code>int64 mail_iid = 2;</code>
     * @return The mailIid.
     */
    @java.lang.Override
    public long getMailIid() {
      return mailIid_;
    }
    /**
     * <code>int64 mail_iid = 2;</code>
     * @param value The mailIid to set.
     * @return This builder for chaining.
     */
    public Builder setMailIid(long value) {
      
      mailIid_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 mail_iid = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearMailIid() {
      
      mailIid_ = 0L;
      onChanged();
      return this;
    }
    @java.lang.Override
    public final Builder setUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.setUnknownFields(unknownFields);
    }

    @java.lang.Override
    public final Builder mergeUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.mergeUnknownFields(unknownFields);
    }


    // @@protoc_insertion_point(builder_scope:PRO.Mail_UnBindAttachs_ack)
  }

  // @@protoc_insertion_point(class_scope:PRO.Mail_UnBindAttachs_ack)
  private static final com.cms.client.network.gpro.Mail_UnBindAttachs_ack DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Mail_UnBindAttachs_ack();
  }

  public static com.cms.client.network.gpro.Mail_UnBindAttachs_ack getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Mail_UnBindAttachs_ack>
      PARSER = new com.google.protobuf.AbstractParser<Mail_UnBindAttachs_ack>() {
    @java.lang.Override
    public Mail_UnBindAttachs_ack parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Mail_UnBindAttachs_ack(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Mail_UnBindAttachs_ack> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Mail_UnBindAttachs_ack> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Mail_UnBindAttachs_ack getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

