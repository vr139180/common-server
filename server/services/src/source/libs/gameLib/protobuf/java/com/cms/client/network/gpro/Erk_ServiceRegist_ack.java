// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.Erk_ServiceRegist_ack}
 */
public final class Erk_ServiceRegist_ack extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Erk_ServiceRegist_ack)
    Erk_ServiceRegist_ackOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Erk_ServiceRegist_ack.newBuilder() to construct.
  private Erk_ServiceRegist_ack(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Erk_ServiceRegist_ack() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Erk_ServiceRegist_ack();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Erk_ServiceRegist_ack(
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

            result_ = input.readInt32();
            break;
          }
          case 16: {
            bitField0_ |= 0x00000001;
            iid_ = input.readInt64();
            break;
          }
          case 24: {
            bitField0_ |= 0x00000002;
            token_ = input.readInt64();
            break;
          }
          case 32: {
            bitField0_ |= 0x00000004;
            eurekaiid_ = input.readInt64();
            break;
          }
          case 40: {
            bitField0_ |= 0x00000008;
            eurekatoken_ = input.readInt64();
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
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceRegist_ack_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceRegist_ack_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Erk_ServiceRegist_ack.class, com.cms.client.network.gpro.Erk_ServiceRegist_ack.Builder.class);
  }

  private int bitField0_;
  public static final int RESULT_FIELD_NUMBER = 1;
  private int result_;
  /**
   * <pre>
   *0:成功 1:失败
   * </pre>
   *
   * <code>int32 result = 1;</code>
   * @return The result.
   */
  @java.lang.Override
  public int getResult() {
    return result_;
  }

  public static final int IID_FIELD_NUMBER = 2;
  private long iid_;
  /**
   * <pre>
   *service编号，由eureka保证唯一
   * </pre>
   *
   * <code>optional int64 iid = 2;</code>
   * @return Whether the iid field is set.
   */
  @java.lang.Override
  public boolean hasIid() {
    return ((bitField0_ & 0x00000001) != 0);
  }
  /**
   * <pre>
   *service编号，由eureka保证唯一
   * </pre>
   *
   * <code>optional int64 iid = 2;</code>
   * @return The iid.
   */
  @java.lang.Override
  public long getIid() {
    return iid_;
  }

  public static final int TOKEN_FIELD_NUMBER = 3;
  private long token_;
  /**
   * <code>optional int64 token = 3;</code>
   * @return Whether the token field is set.
   */
  @java.lang.Override
  public boolean hasToken() {
    return ((bitField0_ & 0x00000002) != 0);
  }
  /**
   * <code>optional int64 token = 3;</code>
   * @return The token.
   */
  @java.lang.Override
  public long getToken() {
    return token_;
  }

  public static final int EUREKAIID_FIELD_NUMBER = 4;
  private long eurekaiid_;
  /**
   * <pre>
   *eureka iid
   * </pre>
   *
   * <code>optional int64 eurekaiid = 4;</code>
   * @return Whether the eurekaiid field is set.
   */
  @java.lang.Override
  public boolean hasEurekaiid() {
    return ((bitField0_ & 0x00000004) != 0);
  }
  /**
   * <pre>
   *eureka iid
   * </pre>
   *
   * <code>optional int64 eurekaiid = 4;</code>
   * @return The eurekaiid.
   */
  @java.lang.Override
  public long getEurekaiid() {
    return eurekaiid_;
  }

  public static final int EUREKATOKEN_FIELD_NUMBER = 5;
  private long eurekatoken_;
  /**
   * <code>optional int64 eurekatoken = 5;</code>
   * @return Whether the eurekatoken field is set.
   */
  @java.lang.Override
  public boolean hasEurekatoken() {
    return ((bitField0_ & 0x00000008) != 0);
  }
  /**
   * <code>optional int64 eurekatoken = 5;</code>
   * @return The eurekatoken.
   */
  @java.lang.Override
  public long getEurekatoken() {
    return eurekatoken_;
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
    if (((bitField0_ & 0x00000001) != 0)) {
      output.writeInt64(2, iid_);
    }
    if (((bitField0_ & 0x00000002) != 0)) {
      output.writeInt64(3, token_);
    }
    if (((bitField0_ & 0x00000004) != 0)) {
      output.writeInt64(4, eurekaiid_);
    }
    if (((bitField0_ & 0x00000008) != 0)) {
      output.writeInt64(5, eurekatoken_);
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
    if (((bitField0_ & 0x00000001) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, iid_);
    }
    if (((bitField0_ & 0x00000002) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(3, token_);
    }
    if (((bitField0_ & 0x00000004) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(4, eurekaiid_);
    }
    if (((bitField0_ & 0x00000008) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(5, eurekatoken_);
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
    if (!(obj instanceof com.cms.client.network.gpro.Erk_ServiceRegist_ack)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Erk_ServiceRegist_ack other = (com.cms.client.network.gpro.Erk_ServiceRegist_ack) obj;

    if (getResult()
        != other.getResult()) return false;
    if (hasIid() != other.hasIid()) return false;
    if (hasIid()) {
      if (getIid()
          != other.getIid()) return false;
    }
    if (hasToken() != other.hasToken()) return false;
    if (hasToken()) {
      if (getToken()
          != other.getToken()) return false;
    }
    if (hasEurekaiid() != other.hasEurekaiid()) return false;
    if (hasEurekaiid()) {
      if (getEurekaiid()
          != other.getEurekaiid()) return false;
    }
    if (hasEurekatoken() != other.hasEurekatoken()) return false;
    if (hasEurekatoken()) {
      if (getEurekatoken()
          != other.getEurekatoken()) return false;
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
    hash = (37 * hash) + RESULT_FIELD_NUMBER;
    hash = (53 * hash) + getResult();
    if (hasIid()) {
      hash = (37 * hash) + IID_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getIid());
    }
    if (hasToken()) {
      hash = (37 * hash) + TOKEN_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getToken());
    }
    if (hasEurekaiid()) {
      hash = (37 * hash) + EUREKAIID_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getEurekaiid());
    }
    if (hasEurekatoken()) {
      hash = (37 * hash) + EUREKATOKEN_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getEurekatoken());
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.Erk_ServiceRegist_ack prototype) {
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
   * Protobuf type {@code PRO.Erk_ServiceRegist_ack}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Erk_ServiceRegist_ack)
      com.cms.client.network.gpro.Erk_ServiceRegist_ackOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceRegist_ack_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceRegist_ack_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Erk_ServiceRegist_ack.class, com.cms.client.network.gpro.Erk_ServiceRegist_ack.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Erk_ServiceRegist_ack.newBuilder()
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

      iid_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000001);
      token_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000002);
      eurekaiid_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000004);
      eurekatoken_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000008);
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceRegist_ack_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceRegist_ack getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Erk_ServiceRegist_ack.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceRegist_ack build() {
      com.cms.client.network.gpro.Erk_ServiceRegist_ack result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceRegist_ack buildPartial() {
      com.cms.client.network.gpro.Erk_ServiceRegist_ack result = new com.cms.client.network.gpro.Erk_ServiceRegist_ack(this);
      int from_bitField0_ = bitField0_;
      int to_bitField0_ = 0;
      result.result_ = result_;
      if (((from_bitField0_ & 0x00000001) != 0)) {
        result.iid_ = iid_;
        to_bitField0_ |= 0x00000001;
      }
      if (((from_bitField0_ & 0x00000002) != 0)) {
        result.token_ = token_;
        to_bitField0_ |= 0x00000002;
      }
      if (((from_bitField0_ & 0x00000004) != 0)) {
        result.eurekaiid_ = eurekaiid_;
        to_bitField0_ |= 0x00000004;
      }
      if (((from_bitField0_ & 0x00000008) != 0)) {
        result.eurekatoken_ = eurekatoken_;
        to_bitField0_ |= 0x00000008;
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
      if (other instanceof com.cms.client.network.gpro.Erk_ServiceRegist_ack) {
        return mergeFrom((com.cms.client.network.gpro.Erk_ServiceRegist_ack)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Erk_ServiceRegist_ack other) {
      if (other == com.cms.client.network.gpro.Erk_ServiceRegist_ack.getDefaultInstance()) return this;
      if (other.getResult() != 0) {
        setResult(other.getResult());
      }
      if (other.hasIid()) {
        setIid(other.getIid());
      }
      if (other.hasToken()) {
        setToken(other.getToken());
      }
      if (other.hasEurekaiid()) {
        setEurekaiid(other.getEurekaiid());
      }
      if (other.hasEurekatoken()) {
        setEurekatoken(other.getEurekatoken());
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
      com.cms.client.network.gpro.Erk_ServiceRegist_ack parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Erk_ServiceRegist_ack) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private int result_ ;
    /**
     * <pre>
     *0:成功 1:失败
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
     *0:成功 1:失败
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
     *0:成功 1:失败
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

    private long iid_ ;
    /**
     * <pre>
     *service编号，由eureka保证唯一
     * </pre>
     *
     * <code>optional int64 iid = 2;</code>
     * @return Whether the iid field is set.
     */
    @java.lang.Override
    public boolean hasIid() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <pre>
     *service编号，由eureka保证唯一
     * </pre>
     *
     * <code>optional int64 iid = 2;</code>
     * @return The iid.
     */
    @java.lang.Override
    public long getIid() {
      return iid_;
    }
    /**
     * <pre>
     *service编号，由eureka保证唯一
     * </pre>
     *
     * <code>optional int64 iid = 2;</code>
     * @param value The iid to set.
     * @return This builder for chaining.
     */
    public Builder setIid(long value) {
      bitField0_ |= 0x00000001;
      iid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *service编号，由eureka保证唯一
     * </pre>
     *
     * <code>optional int64 iid = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearIid() {
      bitField0_ = (bitField0_ & ~0x00000001);
      iid_ = 0L;
      onChanged();
      return this;
    }

    private long token_ ;
    /**
     * <code>optional int64 token = 3;</code>
     * @return Whether the token field is set.
     */
    @java.lang.Override
    public boolean hasToken() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <code>optional int64 token = 3;</code>
     * @return The token.
     */
    @java.lang.Override
    public long getToken() {
      return token_;
    }
    /**
     * <code>optional int64 token = 3;</code>
     * @param value The token to set.
     * @return This builder for chaining.
     */
    public Builder setToken(long value) {
      bitField0_ |= 0x00000002;
      token_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>optional int64 token = 3;</code>
     * @return This builder for chaining.
     */
    public Builder clearToken() {
      bitField0_ = (bitField0_ & ~0x00000002);
      token_ = 0L;
      onChanged();
      return this;
    }

    private long eurekaiid_ ;
    /**
     * <pre>
     *eureka iid
     * </pre>
     *
     * <code>optional int64 eurekaiid = 4;</code>
     * @return Whether the eurekaiid field is set.
     */
    @java.lang.Override
    public boolean hasEurekaiid() {
      return ((bitField0_ & 0x00000004) != 0);
    }
    /**
     * <pre>
     *eureka iid
     * </pre>
     *
     * <code>optional int64 eurekaiid = 4;</code>
     * @return The eurekaiid.
     */
    @java.lang.Override
    public long getEurekaiid() {
      return eurekaiid_;
    }
    /**
     * <pre>
     *eureka iid
     * </pre>
     *
     * <code>optional int64 eurekaiid = 4;</code>
     * @param value The eurekaiid to set.
     * @return This builder for chaining.
     */
    public Builder setEurekaiid(long value) {
      bitField0_ |= 0x00000004;
      eurekaiid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *eureka iid
     * </pre>
     *
     * <code>optional int64 eurekaiid = 4;</code>
     * @return This builder for chaining.
     */
    public Builder clearEurekaiid() {
      bitField0_ = (bitField0_ & ~0x00000004);
      eurekaiid_ = 0L;
      onChanged();
      return this;
    }

    private long eurekatoken_ ;
    /**
     * <code>optional int64 eurekatoken = 5;</code>
     * @return Whether the eurekatoken field is set.
     */
    @java.lang.Override
    public boolean hasEurekatoken() {
      return ((bitField0_ & 0x00000008) != 0);
    }
    /**
     * <code>optional int64 eurekatoken = 5;</code>
     * @return The eurekatoken.
     */
    @java.lang.Override
    public long getEurekatoken() {
      return eurekatoken_;
    }
    /**
     * <code>optional int64 eurekatoken = 5;</code>
     * @param value The eurekatoken to set.
     * @return This builder for chaining.
     */
    public Builder setEurekatoken(long value) {
      bitField0_ |= 0x00000008;
      eurekatoken_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>optional int64 eurekatoken = 5;</code>
     * @return This builder for chaining.
     */
    public Builder clearEurekatoken() {
      bitField0_ = (bitField0_ & ~0x00000008);
      eurekatoken_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.Erk_ServiceRegist_ack)
  }

  // @@protoc_insertion_point(class_scope:PRO.Erk_ServiceRegist_ack)
  private static final com.cms.client.network.gpro.Erk_ServiceRegist_ack DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Erk_ServiceRegist_ack();
  }

  public static com.cms.client.network.gpro.Erk_ServiceRegist_ack getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Erk_ServiceRegist_ack>
      PARSER = new com.google.protobuf.AbstractParser<Erk_ServiceRegist_ack>() {
    @java.lang.Override
    public Erk_ServiceRegist_ack parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Erk_ServiceRegist_ack(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Erk_ServiceRegist_ack> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Erk_ServiceRegist_ack> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Erk_ServiceRegist_ack getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

