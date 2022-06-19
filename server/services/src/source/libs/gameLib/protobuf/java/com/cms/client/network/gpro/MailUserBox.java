// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mail_system.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.MailUserBox}
 */
public final class MailUserBox extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.MailUserBox)
    MailUserBoxOrBuilder {
private static final long serialVersionUID = 0L;
  // Use MailUserBox.newBuilder() to construct.
  private MailUserBox(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private MailUserBox() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new MailUserBox();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private MailUserBox(
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

            ver_ = input.readInt32();
            break;
          }
          case 16: {

            roleIid_ = input.readInt64();
            break;
          }
          case 24: {

            sysmailSync_ = input.readInt64();
            break;
          }
          case 32: {

            lastlistMail_ = input.readInt64();
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
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_MailUserBox_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.MailSystem.internal_static_PRO_MailUserBox_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.MailUserBox.class, com.cms.client.network.gpro.MailUserBox.Builder.class);
  }

  public static final int VER__FIELD_NUMBER = 1;
  private int ver_;
  /**
   * <code>int32 ver_ = 1;</code>
   * @return The ver.
   */
  @java.lang.Override
  public int getVer() {
    return ver_;
  }

  public static final int ROLE_IID_FIELD_NUMBER = 2;
  private long roleIid_;
  /**
   * <code>int64 role_iid = 2;</code>
   * @return The roleIid.
   */
  @java.lang.Override
  public long getRoleIid() {
    return roleIid_;
  }

  public static final int SYSMAIL_SYNC_FIELD_NUMBER = 3;
  private long sysmailSync_;
  /**
   * <pre>
   *最后一次获取的系统邮件
   * </pre>
   *
   * <code>int64 sysmail_sync = 3;</code>
   * @return The sysmailSync.
   */
  @java.lang.Override
  public long getSysmailSync() {
    return sysmailSync_;
  }

  public static final int LASTLIST_MAIL_FIELD_NUMBER = 4;
  private long lastlistMail_;
  /**
   * <pre>
   *最后一次获取的邮件iid
   * </pre>
   *
   * <code>int64 lastlist_mail = 4;</code>
   * @return The lastlistMail.
   */
  @java.lang.Override
  public long getLastlistMail() {
    return lastlistMail_;
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
    if (ver_ != 0) {
      output.writeInt32(1, ver_);
    }
    if (roleIid_ != 0L) {
      output.writeInt64(2, roleIid_);
    }
    if (sysmailSync_ != 0L) {
      output.writeInt64(3, sysmailSync_);
    }
    if (lastlistMail_ != 0L) {
      output.writeInt64(4, lastlistMail_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (ver_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(1, ver_);
    }
    if (roleIid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, roleIid_);
    }
    if (sysmailSync_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(3, sysmailSync_);
    }
    if (lastlistMail_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(4, lastlistMail_);
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
    if (!(obj instanceof com.cms.client.network.gpro.MailUserBox)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.MailUserBox other = (com.cms.client.network.gpro.MailUserBox) obj;

    if (getVer()
        != other.getVer()) return false;
    if (getRoleIid()
        != other.getRoleIid()) return false;
    if (getSysmailSync()
        != other.getSysmailSync()) return false;
    if (getLastlistMail()
        != other.getLastlistMail()) return false;
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
    hash = (37 * hash) + VER__FIELD_NUMBER;
    hash = (53 * hash) + getVer();
    hash = (37 * hash) + ROLE_IID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getRoleIid());
    hash = (37 * hash) + SYSMAIL_SYNC_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getSysmailSync());
    hash = (37 * hash) + LASTLIST_MAIL_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getLastlistMail());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MailUserBox parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MailUserBox parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MailUserBox parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.MailUserBox prototype) {
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
   * Protobuf type {@code PRO.MailUserBox}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.MailUserBox)
      com.cms.client.network.gpro.MailUserBoxOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_MailUserBox_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_MailUserBox_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.MailUserBox.class, com.cms.client.network.gpro.MailUserBox.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.MailUserBox.newBuilder()
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
      ver_ = 0;

      roleIid_ = 0L;

      sysmailSync_ = 0L;

      lastlistMail_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.MailSystem.internal_static_PRO_MailUserBox_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MailUserBox getDefaultInstanceForType() {
      return com.cms.client.network.gpro.MailUserBox.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MailUserBox build() {
      com.cms.client.network.gpro.MailUserBox result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MailUserBox buildPartial() {
      com.cms.client.network.gpro.MailUserBox result = new com.cms.client.network.gpro.MailUserBox(this);
      result.ver_ = ver_;
      result.roleIid_ = roleIid_;
      result.sysmailSync_ = sysmailSync_;
      result.lastlistMail_ = lastlistMail_;
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
      if (other instanceof com.cms.client.network.gpro.MailUserBox) {
        return mergeFrom((com.cms.client.network.gpro.MailUserBox)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.MailUserBox other) {
      if (other == com.cms.client.network.gpro.MailUserBox.getDefaultInstance()) return this;
      if (other.getVer() != 0) {
        setVer(other.getVer());
      }
      if (other.getRoleIid() != 0L) {
        setRoleIid(other.getRoleIid());
      }
      if (other.getSysmailSync() != 0L) {
        setSysmailSync(other.getSysmailSync());
      }
      if (other.getLastlistMail() != 0L) {
        setLastlistMail(other.getLastlistMail());
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
      com.cms.client.network.gpro.MailUserBox parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.MailUserBox) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private int ver_ ;
    /**
     * <code>int32 ver_ = 1;</code>
     * @return The ver.
     */
    @java.lang.Override
    public int getVer() {
      return ver_;
    }
    /**
     * <code>int32 ver_ = 1;</code>
     * @param value The ver to set.
     * @return This builder for chaining.
     */
    public Builder setVer(int value) {
      
      ver_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int32 ver_ = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearVer() {
      
      ver_ = 0;
      onChanged();
      return this;
    }

    private long roleIid_ ;
    /**
     * <code>int64 role_iid = 2;</code>
     * @return The roleIid.
     */
    @java.lang.Override
    public long getRoleIid() {
      return roleIid_;
    }
    /**
     * <code>int64 role_iid = 2;</code>
     * @param value The roleIid to set.
     * @return This builder for chaining.
     */
    public Builder setRoleIid(long value) {
      
      roleIid_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 role_iid = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearRoleIid() {
      
      roleIid_ = 0L;
      onChanged();
      return this;
    }

    private long sysmailSync_ ;
    /**
     * <pre>
     *最后一次获取的系统邮件
     * </pre>
     *
     * <code>int64 sysmail_sync = 3;</code>
     * @return The sysmailSync.
     */
    @java.lang.Override
    public long getSysmailSync() {
      return sysmailSync_;
    }
    /**
     * <pre>
     *最后一次获取的系统邮件
     * </pre>
     *
     * <code>int64 sysmail_sync = 3;</code>
     * @param value The sysmailSync to set.
     * @return This builder for chaining.
     */
    public Builder setSysmailSync(long value) {
      
      sysmailSync_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *最后一次获取的系统邮件
     * </pre>
     *
     * <code>int64 sysmail_sync = 3;</code>
     * @return This builder for chaining.
     */
    public Builder clearSysmailSync() {
      
      sysmailSync_ = 0L;
      onChanged();
      return this;
    }

    private long lastlistMail_ ;
    /**
     * <pre>
     *最后一次获取的邮件iid
     * </pre>
     *
     * <code>int64 lastlist_mail = 4;</code>
     * @return The lastlistMail.
     */
    @java.lang.Override
    public long getLastlistMail() {
      return lastlistMail_;
    }
    /**
     * <pre>
     *最后一次获取的邮件iid
     * </pre>
     *
     * <code>int64 lastlist_mail = 4;</code>
     * @param value The lastlistMail to set.
     * @return This builder for chaining.
     */
    public Builder setLastlistMail(long value) {
      
      lastlistMail_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *最后一次获取的邮件iid
     * </pre>
     *
     * <code>int64 lastlist_mail = 4;</code>
     * @return This builder for chaining.
     */
    public Builder clearLastlistMail() {
      
      lastlistMail_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.MailUserBox)
  }

  // @@protoc_insertion_point(class_scope:PRO.MailUserBox)
  private static final com.cms.client.network.gpro.MailUserBox DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.MailUserBox();
  }

  public static com.cms.client.network.gpro.MailUserBox getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<MailUserBox>
      PARSER = new com.google.protobuf.AbstractParser<MailUserBox>() {
    @java.lang.Override
    public MailUserBox parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new MailUserBox(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<MailUserBox> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<MailUserBox> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.MailUserBox getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

