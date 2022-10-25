// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mail_system.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.Mail_UserOnline_active}
 */
public final class Mail_UserOnline_active extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Mail_UserOnline_active)
    Mail_UserOnline_activeOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Mail_UserOnline_active.newBuilder() to construct.
  private Mail_UserOnline_active(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Mail_UserOnline_active() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Mail_UserOnline_active();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Mail_UserOnline_active(
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

            lastMailiid_ = input.readInt64();
            break;
          }
          case 16: {

            firstactive_ = input.readBool();
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
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UserOnline_active_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UserOnline_active_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Mail_UserOnline_active.class, com.cms.client.network.gpro.Mail_UserOnline_active.Builder.class);
  }

  public static final int LAST_MAILIID_FIELD_NUMBER = 1;
  private long lastMailiid_;
  /**
   * <pre>
   *客户端本地保存的最大mailiid
   * </pre>
   *
   * <code>int64 last_mailiid = 1;</code>
   * @return The lastMailiid.
   */
  @java.lang.Override
  public long getLastMailiid() {
    return lastMailiid_;
  }

  public static final int FIRSTACTIVE_FIELD_NUMBER = 2;
  private boolean firstactive_;
  /**
   * <pre>
   *登陆成功后第一发送设置为true
   * </pre>
   *
   * <code>bool firstactive = 2;</code>
   * @return The firstactive.
   */
  @java.lang.Override
  public boolean getFirstactive() {
    return firstactive_;
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
    if (lastMailiid_ != 0L) {
      output.writeInt64(1, lastMailiid_);
    }
    if (firstactive_ != false) {
      output.writeBool(2, firstactive_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (lastMailiid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(1, lastMailiid_);
    }
    if (firstactive_ != false) {
      size += com.google.protobuf.CodedOutputStream
        .computeBoolSize(2, firstactive_);
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
    if (!(obj instanceof com.cms.client.network.gpro.Mail_UserOnline_active)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Mail_UserOnline_active other = (com.cms.client.network.gpro.Mail_UserOnline_active) obj;

    if (getLastMailiid()
        != other.getLastMailiid()) return false;
    if (getFirstactive()
        != other.getFirstactive()) return false;
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
    hash = (37 * hash) + LAST_MAILIID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getLastMailiid());
    hash = (37 * hash) + FIRSTACTIVE_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashBoolean(
        getFirstactive());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Mail_UserOnline_active parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.Mail_UserOnline_active prototype) {
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
   * Protobuf type {@code PRO.Mail_UserOnline_active}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Mail_UserOnline_active)
      com.cms.client.network.gpro.Mail_UserOnline_activeOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UserOnline_active_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UserOnline_active_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Mail_UserOnline_active.class, com.cms.client.network.gpro.Mail_UserOnline_active.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Mail_UserOnline_active.newBuilder()
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
      lastMailiid_ = 0L;

      firstactive_ = false;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_Mail_UserOnline_active_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UserOnline_active getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Mail_UserOnline_active.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UserOnline_active build() {
      com.cms.client.network.gpro.Mail_UserOnline_active result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Mail_UserOnline_active buildPartial() {
      com.cms.client.network.gpro.Mail_UserOnline_active result = new com.cms.client.network.gpro.Mail_UserOnline_active(this);
      result.lastMailiid_ = lastMailiid_;
      result.firstactive_ = firstactive_;
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
      if (other instanceof com.cms.client.network.gpro.Mail_UserOnline_active) {
        return mergeFrom((com.cms.client.network.gpro.Mail_UserOnline_active)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Mail_UserOnline_active other) {
      if (other == com.cms.client.network.gpro.Mail_UserOnline_active.getDefaultInstance()) return this;
      if (other.getLastMailiid() != 0L) {
        setLastMailiid(other.getLastMailiid());
      }
      if (other.getFirstactive() != false) {
        setFirstactive(other.getFirstactive());
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
      com.cms.client.network.gpro.Mail_UserOnline_active parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Mail_UserOnline_active) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private long lastMailiid_ ;
    /**
     * <pre>
     *客户端本地保存的最大mailiid
     * </pre>
     *
     * <code>int64 last_mailiid = 1;</code>
     * @return The lastMailiid.
     */
    @java.lang.Override
    public long getLastMailiid() {
      return lastMailiid_;
    }
    /**
     * <pre>
     *客户端本地保存的最大mailiid
     * </pre>
     *
     * <code>int64 last_mailiid = 1;</code>
     * @param value The lastMailiid to set.
     * @return This builder for chaining.
     */
    public Builder setLastMailiid(long value) {
      
      lastMailiid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *客户端本地保存的最大mailiid
     * </pre>
     *
     * <code>int64 last_mailiid = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearLastMailiid() {
      
      lastMailiid_ = 0L;
      onChanged();
      return this;
    }

    private boolean firstactive_ ;
    /**
     * <pre>
     *登陆成功后第一发送设置为true
     * </pre>
     *
     * <code>bool firstactive = 2;</code>
     * @return The firstactive.
     */
    @java.lang.Override
    public boolean getFirstactive() {
      return firstactive_;
    }
    /**
     * <pre>
     *登陆成功后第一发送设置为true
     * </pre>
     *
     * <code>bool firstactive = 2;</code>
     * @param value The firstactive to set.
     * @return This builder for chaining.
     */
    public Builder setFirstactive(boolean value) {
      
      firstactive_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *登陆成功后第一发送设置为true
     * </pre>
     *
     * <code>bool firstactive = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearFirstactive() {
      
      firstactive_ = false;
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


    // @@protoc_insertion_point(builder_scope:PRO.Mail_UserOnline_active)
  }

  // @@protoc_insertion_point(class_scope:PRO.Mail_UserOnline_active)
  private static final com.cms.client.network.gpro.Mail_UserOnline_active DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Mail_UserOnline_active();
  }

  public static com.cms.client.network.gpro.Mail_UserOnline_active getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Mail_UserOnline_active>
      PARSER = new com.google.protobuf.AbstractParser<Mail_UserOnline_active>() {
    @java.lang.Override
    public Mail_UserOnline_active parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Mail_UserOnline_active(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Mail_UserOnline_active> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Mail_UserOnline_active> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Mail_UserOnline_active getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

