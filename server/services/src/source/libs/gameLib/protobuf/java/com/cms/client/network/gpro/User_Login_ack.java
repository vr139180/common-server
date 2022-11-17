// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_login.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.User_Login_ack}
 */
public final class User_Login_ack extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.User_Login_ack)
    User_Login_ackOrBuilder {
private static final long serialVersionUID = 0L;
  // Use User_Login_ack.newBuilder() to construct.
  private User_Login_ack(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private User_Login_ack() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new User_Login_ack();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private User_Login_ack(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
    int mutable_bitField0_ = 0;
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

            type_ = input.readInt32();
            break;
          }
          case 16: {

            result_ = input.readInt32();
            break;
          }
          case 24: {
            bitField0_ |= 0x00000001;
            userIid_ = input.readInt64();
            break;
          }
          case 32: {
            bitField0_ |= 0x00000002;
            logintoken_ = input.readInt64();
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
    return com.cms.client.network.gpro.UserLogin.internal_static_PRO_User_Login_ack_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.UserLogin.internal_static_PRO_User_Login_ack_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.User_Login_ack.class, com.cms.client.network.gpro.User_Login_ack.Builder.class);
  }

  private int bitField0_;
  public static final int TYPE_FIELD_NUMBER = 1;
  private int type_;
  /**
   * <pre>
   *登录类型 1:用户名密码 2:token登录
   * </pre>
   *
   * <code>int32 type = 1;</code>
   * @return The type.
   */
  @java.lang.Override
  public int getType() {
    return type_;
  }

  public static final int RESULT_FIELD_NUMBER = 2;
  private int result_;
  /**
   * <pre>
   *0:成功 1:用户名不存在 2:密码错误 3:token错误 4：没有资源 5:系统错误 6:登陆排队中
   * </pre>
   *
   * <code>int32 result = 2;</code>
   * @return The result.
   */
  @java.lang.Override
  public int getResult() {
    return result_;
  }

  public static final int USER_IID_FIELD_NUMBER = 3;
  private long userIid_;
  /**
   * <pre>
   *成功返回 用户iid
   * </pre>
   *
   * <code>optional int64 user_iid = 3;</code>
   * @return Whether the userIid field is set.
   */
  @java.lang.Override
  public boolean hasUserIid() {
    return ((bitField0_ & 0x00000001) != 0);
  }
  /**
   * <pre>
   *成功返回 用户iid
   * </pre>
   *
   * <code>optional int64 user_iid = 3;</code>
   * @return The userIid.
   */
  @java.lang.Override
  public long getUserIid() {
    return userIid_;
  }

  public static final int LOGINTOKEN_FIELD_NUMBER = 4;
  private long logintoken_;
  /**
   * <pre>
   *token登陆时用到的
   * </pre>
   *
   * <code>optional int64 logintoken = 4;</code>
   * @return Whether the logintoken field is set.
   */
  @java.lang.Override
  public boolean hasLogintoken() {
    return ((bitField0_ & 0x00000002) != 0);
  }
  /**
   * <pre>
   *token登陆时用到的
   * </pre>
   *
   * <code>optional int64 logintoken = 4;</code>
   * @return The logintoken.
   */
  @java.lang.Override
  public long getLogintoken() {
    return logintoken_;
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
    if (type_ != 0) {
      output.writeInt32(1, type_);
    }
    if (result_ != 0) {
      output.writeInt32(2, result_);
    }
    if (((bitField0_ & 0x00000001) != 0)) {
      output.writeInt64(3, userIid_);
    }
    if (((bitField0_ & 0x00000002) != 0)) {
      output.writeInt64(4, logintoken_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (type_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(1, type_);
    }
    if (result_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(2, result_);
    }
    if (((bitField0_ & 0x00000001) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(3, userIid_);
    }
    if (((bitField0_ & 0x00000002) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(4, logintoken_);
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
    if (!(obj instanceof com.cms.client.network.gpro.User_Login_ack)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.User_Login_ack other = (com.cms.client.network.gpro.User_Login_ack) obj;

    if (getType()
        != other.getType()) return false;
    if (getResult()
        != other.getResult()) return false;
    if (hasUserIid() != other.hasUserIid()) return false;
    if (hasUserIid()) {
      if (getUserIid()
          != other.getUserIid()) return false;
    }
    if (hasLogintoken() != other.hasLogintoken()) return false;
    if (hasLogintoken()) {
      if (getLogintoken()
          != other.getLogintoken()) return false;
    }
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
    hash = (37 * hash) + TYPE_FIELD_NUMBER;
    hash = (53 * hash) + getType();
    hash = (37 * hash) + RESULT_FIELD_NUMBER;
    hash = (53 * hash) + getResult();
    if (hasUserIid()) {
      hash = (37 * hash) + USER_IID_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getUserIid());
    }
    if (hasLogintoken()) {
      hash = (37 * hash) + LOGINTOKEN_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getLogintoken());
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.User_Login_ack parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.User_Login_ack prototype) {
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
   * Protobuf type {@code PRO.User_Login_ack}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.User_Login_ack)
      com.cms.client.network.gpro.User_Login_ackOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.UserLogin.internal_static_PRO_User_Login_ack_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.UserLogin.internal_static_PRO_User_Login_ack_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.User_Login_ack.class, com.cms.client.network.gpro.User_Login_ack.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.User_Login_ack.newBuilder()
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
      type_ = 0;

      result_ = 0;

      userIid_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000001);
      logintoken_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000002);
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.UserLogin.internal_static_PRO_User_Login_ack_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.User_Login_ack getDefaultInstanceForType() {
      return com.cms.client.network.gpro.User_Login_ack.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.User_Login_ack build() {
      com.cms.client.network.gpro.User_Login_ack result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.User_Login_ack buildPartial() {
      com.cms.client.network.gpro.User_Login_ack result = new com.cms.client.network.gpro.User_Login_ack(this);
      int from_bitField0_ = bitField0_;
      int to_bitField0_ = 0;
      result.type_ = type_;
      result.result_ = result_;
      if (((from_bitField0_ & 0x00000001) != 0)) {
        result.userIid_ = userIid_;
        to_bitField0_ |= 0x00000001;
      }
      if (((from_bitField0_ & 0x00000002) != 0)) {
        result.logintoken_ = logintoken_;
        to_bitField0_ |= 0x00000002;
      }
      result.bitField0_ = to_bitField0_;
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
      if (other instanceof com.cms.client.network.gpro.User_Login_ack) {
        return mergeFrom((com.cms.client.network.gpro.User_Login_ack)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.User_Login_ack other) {
      if (other == com.cms.client.network.gpro.User_Login_ack.getDefaultInstance()) return this;
      if (other.getType() != 0) {
        setType(other.getType());
      }
      if (other.getResult() != 0) {
        setResult(other.getResult());
      }
      if (other.hasUserIid()) {
        setUserIid(other.getUserIid());
      }
      if (other.hasLogintoken()) {
        setLogintoken(other.getLogintoken());
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
      com.cms.client.network.gpro.User_Login_ack parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.User_Login_ack) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private int type_ ;
    /**
     * <pre>
     *登录类型 1:用户名密码 2:token登录
     * </pre>
     *
     * <code>int32 type = 1;</code>
     * @return The type.
     */
    @java.lang.Override
    public int getType() {
      return type_;
    }
    /**
     * <pre>
     *登录类型 1:用户名密码 2:token登录
     * </pre>
     *
     * <code>int32 type = 1;</code>
     * @param value The type to set.
     * @return This builder for chaining.
     */
    public Builder setType(int value) {
      
      type_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *登录类型 1:用户名密码 2:token登录
     * </pre>
     *
     * <code>int32 type = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearType() {
      
      type_ = 0;
      onChanged();
      return this;
    }

    private int result_ ;
    /**
     * <pre>
     *0:成功 1:用户名不存在 2:密码错误 3:token错误 4：没有资源 5:系统错误 6:登陆排队中
     * </pre>
     *
     * <code>int32 result = 2;</code>
     * @return The result.
     */
    @java.lang.Override
    public int getResult() {
      return result_;
    }
    /**
     * <pre>
     *0:成功 1:用户名不存在 2:密码错误 3:token错误 4：没有资源 5:系统错误 6:登陆排队中
     * </pre>
     *
     * <code>int32 result = 2;</code>
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
     *0:成功 1:用户名不存在 2:密码错误 3:token错误 4：没有资源 5:系统错误 6:登陆排队中
     * </pre>
     *
     * <code>int32 result = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearResult() {
      
      result_ = 0;
      onChanged();
      return this;
    }

    private long userIid_ ;
    /**
     * <pre>
     *成功返回 用户iid
     * </pre>
     *
     * <code>optional int64 user_iid = 3;</code>
     * @return Whether the userIid field is set.
     */
    @java.lang.Override
    public boolean hasUserIid() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <pre>
     *成功返回 用户iid
     * </pre>
     *
     * <code>optional int64 user_iid = 3;</code>
     * @return The userIid.
     */
    @java.lang.Override
    public long getUserIid() {
      return userIid_;
    }
    /**
     * <pre>
     *成功返回 用户iid
     * </pre>
     *
     * <code>optional int64 user_iid = 3;</code>
     * @param value The userIid to set.
     * @return This builder for chaining.
     */
    public Builder setUserIid(long value) {
      bitField0_ |= 0x00000001;
      userIid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *成功返回 用户iid
     * </pre>
     *
     * <code>optional int64 user_iid = 3;</code>
     * @return This builder for chaining.
     */
    public Builder clearUserIid() {
      bitField0_ = (bitField0_ & ~0x00000001);
      userIid_ = 0L;
      onChanged();
      return this;
    }

    private long logintoken_ ;
    /**
     * <pre>
     *token登陆时用到的
     * </pre>
     *
     * <code>optional int64 logintoken = 4;</code>
     * @return Whether the logintoken field is set.
     */
    @java.lang.Override
    public boolean hasLogintoken() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <pre>
     *token登陆时用到的
     * </pre>
     *
     * <code>optional int64 logintoken = 4;</code>
     * @return The logintoken.
     */
    @java.lang.Override
    public long getLogintoken() {
      return logintoken_;
    }
    /**
     * <pre>
     *token登陆时用到的
     * </pre>
     *
     * <code>optional int64 logintoken = 4;</code>
     * @param value The logintoken to set.
     * @return This builder for chaining.
     */
    public Builder setLogintoken(long value) {
      bitField0_ |= 0x00000002;
      logintoken_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *token登陆时用到的
     * </pre>
     *
     * <code>optional int64 logintoken = 4;</code>
     * @return This builder for chaining.
     */
    public Builder clearLogintoken() {
      bitField0_ = (bitField0_ & ~0x00000002);
      logintoken_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.User_Login_ack)
  }

  // @@protoc_insertion_point(class_scope:PRO.User_Login_ack)
  private static final com.cms.client.network.gpro.User_Login_ack DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.User_Login_ack();
  }

  public static com.cms.client.network.gpro.User_Login_ack getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<User_Login_ack>
      PARSER = new com.google.protobuf.AbstractParser<User_Login_ack>() {
    @java.lang.Override
    public User_Login_ack parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new User_Login_ack(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<User_Login_ack> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<User_Login_ack> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.User_Login_ack getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

