// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: res_internal.proto

package com.cms.client.network.gpro;

/**
 * <pre>
 *res服务器和gate,确认资源协议
 * </pre>
 *
 * Protobuf type {@code PRO.Res_ProxyUserSlot_req}
 */
public final class Res_ProxyUserSlot_req extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Res_ProxyUserSlot_req)
    Res_ProxyUserSlot_reqOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Res_ProxyUserSlot_req.newBuilder() to construct.
  private Res_ProxyUserSlot_req(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Res_ProxyUserSlot_req() {
    proxytoken_ = "";
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Res_ProxyUserSlot_req();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Res_ProxyUserSlot_req(
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
          case 82: {
            java.lang.String s = input.readStringRequireUtf8();

            proxytoken_ = s;
            break;
          }
          case 88: {

            homeiid_ = input.readInt64();
            break;
          }
          case 96: {

            gateiid_ = input.readInt64();
            break;
          }
          case 104: {

            hgtype_ = input.readInt32();
            break;
          }
          case 160: {

            lsno_ = input.readInt64();
            break;
          }
          case 168: {

            sequence_ = input.readInt64();
            break;
          }
          case 176: {

            userIid_ = input.readInt64();
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
    return com.cms.client.network.gpro.ResInternal.internal_static_PRO_Res_ProxyUserSlot_req_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.ResInternal.internal_static_PRO_Res_ProxyUserSlot_req_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Res_ProxyUserSlot_req.class, com.cms.client.network.gpro.Res_ProxyUserSlot_req.Builder.class);
  }

  public static final int PROXYTOKEN_FIELD_NUMBER = 10;
  private volatile java.lang.Object proxytoken_;
  /**
   * <pre>
   *proxytoken
   * </pre>
   *
   * <code>string proxytoken = 10;</code>
   * @return The proxytoken.
   */
  @java.lang.Override
  public java.lang.String getProxytoken() {
    java.lang.Object ref = proxytoken_;
    if (ref instanceof java.lang.String) {
      return (java.lang.String) ref;
    } else {
      com.google.protobuf.ByteString bs = 
          (com.google.protobuf.ByteString) ref;
      java.lang.String s = bs.toStringUtf8();
      proxytoken_ = s;
      return s;
    }
  }
  /**
   * <pre>
   *proxytoken
   * </pre>
   *
   * <code>string proxytoken = 10;</code>
   * @return The bytes for proxytoken.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString
      getProxytokenBytes() {
    java.lang.Object ref = proxytoken_;
    if (ref instanceof java.lang.String) {
      com.google.protobuf.ByteString b = 
          com.google.protobuf.ByteString.copyFromUtf8(
              (java.lang.String) ref);
      proxytoken_ = b;
      return b;
    } else {
      return (com.google.protobuf.ByteString) ref;
    }
  }

  public static final int HOMEIID_FIELD_NUMBER = 11;
  private long homeiid_;
  /**
   * <pre>
   *homeiid gateiid用来在home,gate节点判断资源组合是否发生了变化
   * </pre>
   *
   * <code>int64 homeiid = 11;</code>
   * @return The homeiid.
   */
  @java.lang.Override
  public long getHomeiid() {
    return homeiid_;
  }

  public static final int GATEIID_FIELD_NUMBER = 12;
  private long gateiid_;
  /**
   * <code>int64 gateiid = 12;</code>
   * @return The gateiid.
   */
  @java.lang.Override
  public long getGateiid() {
    return gateiid_;
  }

  public static final int HGTYPE_FIELD_NUMBER = 13;
  private int hgtype_;
  /**
   * <code>int32 hgtype = 13;</code>
   * @return The hgtype.
   */
  @java.lang.Override
  public int getHgtype() {
    return hgtype_;
  }

  public static final int LSNO_FIELD_NUMBER = 20;
  private long lsno_;
  /**
   * <pre>
   *userslotreq附带信息
   * </pre>
   *
   * <code>int64 lsno = 20;</code>
   * @return The lsno.
   */
  @java.lang.Override
  public long getLsno() {
    return lsno_;
  }

  public static final int SEQUENCE_FIELD_NUMBER = 21;
  private long sequence_;
  /**
   * <code>int64 sequence = 21;</code>
   * @return The sequence.
   */
  @java.lang.Override
  public long getSequence() {
    return sequence_;
  }

  public static final int USER_IID_FIELD_NUMBER = 22;
  private long userIid_;
  /**
   * <code>int64 user_iid = 22;</code>
   * @return The userIid.
   */
  @java.lang.Override
  public long getUserIid() {
    return userIid_;
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
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(proxytoken_)) {
      com.google.protobuf.GeneratedMessageV3.writeString(output, 10, proxytoken_);
    }
    if (homeiid_ != 0L) {
      output.writeInt64(11, homeiid_);
    }
    if (gateiid_ != 0L) {
      output.writeInt64(12, gateiid_);
    }
    if (hgtype_ != 0) {
      output.writeInt32(13, hgtype_);
    }
    if (lsno_ != 0L) {
      output.writeInt64(20, lsno_);
    }
    if (sequence_ != 0L) {
      output.writeInt64(21, sequence_);
    }
    if (userIid_ != 0L) {
      output.writeInt64(22, userIid_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(proxytoken_)) {
      size += com.google.protobuf.GeneratedMessageV3.computeStringSize(10, proxytoken_);
    }
    if (homeiid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(11, homeiid_);
    }
    if (gateiid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(12, gateiid_);
    }
    if (hgtype_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(13, hgtype_);
    }
    if (lsno_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(20, lsno_);
    }
    if (sequence_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(21, sequence_);
    }
    if (userIid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(22, userIid_);
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
    if (!(obj instanceof com.cms.client.network.gpro.Res_ProxyUserSlot_req)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Res_ProxyUserSlot_req other = (com.cms.client.network.gpro.Res_ProxyUserSlot_req) obj;

    if (!getProxytoken()
        .equals(other.getProxytoken())) return false;
    if (getHomeiid()
        != other.getHomeiid()) return false;
    if (getGateiid()
        != other.getGateiid()) return false;
    if (getHgtype()
        != other.getHgtype()) return false;
    if (getLsno()
        != other.getLsno()) return false;
    if (getSequence()
        != other.getSequence()) return false;
    if (getUserIid()
        != other.getUserIid()) return false;
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
    hash = (37 * hash) + PROXYTOKEN_FIELD_NUMBER;
    hash = (53 * hash) + getProxytoken().hashCode();
    hash = (37 * hash) + HOMEIID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getHomeiid());
    hash = (37 * hash) + GATEIID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getGateiid());
    hash = (37 * hash) + HGTYPE_FIELD_NUMBER;
    hash = (53 * hash) + getHgtype();
    hash = (37 * hash) + LSNO_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getLsno());
    hash = (37 * hash) + SEQUENCE_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getSequence());
    hash = (37 * hash) + USER_IID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getUserIid());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.Res_ProxyUserSlot_req prototype) {
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
   * <pre>
   *res服务器和gate,确认资源协议
   * </pre>
   *
   * Protobuf type {@code PRO.Res_ProxyUserSlot_req}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Res_ProxyUserSlot_req)
      com.cms.client.network.gpro.Res_ProxyUserSlot_reqOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.ResInternal.internal_static_PRO_Res_ProxyUserSlot_req_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.ResInternal.internal_static_PRO_Res_ProxyUserSlot_req_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Res_ProxyUserSlot_req.class, com.cms.client.network.gpro.Res_ProxyUserSlot_req.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Res_ProxyUserSlot_req.newBuilder()
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
      proxytoken_ = "";

      homeiid_ = 0L;

      gateiid_ = 0L;

      hgtype_ = 0;

      lsno_ = 0L;

      sequence_ = 0L;

      userIid_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.ResInternal.internal_static_PRO_Res_ProxyUserSlot_req_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Res_ProxyUserSlot_req getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Res_ProxyUserSlot_req.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Res_ProxyUserSlot_req build() {
      com.cms.client.network.gpro.Res_ProxyUserSlot_req result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Res_ProxyUserSlot_req buildPartial() {
      com.cms.client.network.gpro.Res_ProxyUserSlot_req result = new com.cms.client.network.gpro.Res_ProxyUserSlot_req(this);
      result.proxytoken_ = proxytoken_;
      result.homeiid_ = homeiid_;
      result.gateiid_ = gateiid_;
      result.hgtype_ = hgtype_;
      result.lsno_ = lsno_;
      result.sequence_ = sequence_;
      result.userIid_ = userIid_;
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
      if (other instanceof com.cms.client.network.gpro.Res_ProxyUserSlot_req) {
        return mergeFrom((com.cms.client.network.gpro.Res_ProxyUserSlot_req)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Res_ProxyUserSlot_req other) {
      if (other == com.cms.client.network.gpro.Res_ProxyUserSlot_req.getDefaultInstance()) return this;
      if (!other.getProxytoken().isEmpty()) {
        proxytoken_ = other.proxytoken_;
        onChanged();
      }
      if (other.getHomeiid() != 0L) {
        setHomeiid(other.getHomeiid());
      }
      if (other.getGateiid() != 0L) {
        setGateiid(other.getGateiid());
      }
      if (other.getHgtype() != 0) {
        setHgtype(other.getHgtype());
      }
      if (other.getLsno() != 0L) {
        setLsno(other.getLsno());
      }
      if (other.getSequence() != 0L) {
        setSequence(other.getSequence());
      }
      if (other.getUserIid() != 0L) {
        setUserIid(other.getUserIid());
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
      com.cms.client.network.gpro.Res_ProxyUserSlot_req parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Res_ProxyUserSlot_req) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private java.lang.Object proxytoken_ = "";
    /**
     * <pre>
     *proxytoken
     * </pre>
     *
     * <code>string proxytoken = 10;</code>
     * @return The proxytoken.
     */
    public java.lang.String getProxytoken() {
      java.lang.Object ref = proxytoken_;
      if (!(ref instanceof java.lang.String)) {
        com.google.protobuf.ByteString bs =
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        proxytoken_ = s;
        return s;
      } else {
        return (java.lang.String) ref;
      }
    }
    /**
     * <pre>
     *proxytoken
     * </pre>
     *
     * <code>string proxytoken = 10;</code>
     * @return The bytes for proxytoken.
     */
    public com.google.protobuf.ByteString
        getProxytokenBytes() {
      java.lang.Object ref = proxytoken_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        proxytoken_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    /**
     * <pre>
     *proxytoken
     * </pre>
     *
     * <code>string proxytoken = 10;</code>
     * @param value The proxytoken to set.
     * @return This builder for chaining.
     */
    public Builder setProxytoken(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      proxytoken_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *proxytoken
     * </pre>
     *
     * <code>string proxytoken = 10;</code>
     * @return This builder for chaining.
     */
    public Builder clearProxytoken() {
      
      proxytoken_ = getDefaultInstance().getProxytoken();
      onChanged();
      return this;
    }
    /**
     * <pre>
     *proxytoken
     * </pre>
     *
     * <code>string proxytoken = 10;</code>
     * @param value The bytes for proxytoken to set.
     * @return This builder for chaining.
     */
    public Builder setProxytokenBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
      
      proxytoken_ = value;
      onChanged();
      return this;
    }

    private long homeiid_ ;
    /**
     * <pre>
     *homeiid gateiid用来在home,gate节点判断资源组合是否发生了变化
     * </pre>
     *
     * <code>int64 homeiid = 11;</code>
     * @return The homeiid.
     */
    @java.lang.Override
    public long getHomeiid() {
      return homeiid_;
    }
    /**
     * <pre>
     *homeiid gateiid用来在home,gate节点判断资源组合是否发生了变化
     * </pre>
     *
     * <code>int64 homeiid = 11;</code>
     * @param value The homeiid to set.
     * @return This builder for chaining.
     */
    public Builder setHomeiid(long value) {
      
      homeiid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *homeiid gateiid用来在home,gate节点判断资源组合是否发生了变化
     * </pre>
     *
     * <code>int64 homeiid = 11;</code>
     * @return This builder for chaining.
     */
    public Builder clearHomeiid() {
      
      homeiid_ = 0L;
      onChanged();
      return this;
    }

    private long gateiid_ ;
    /**
     * <code>int64 gateiid = 12;</code>
     * @return The gateiid.
     */
    @java.lang.Override
    public long getGateiid() {
      return gateiid_;
    }
    /**
     * <code>int64 gateiid = 12;</code>
     * @param value The gateiid to set.
     * @return This builder for chaining.
     */
    public Builder setGateiid(long value) {
      
      gateiid_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 gateiid = 12;</code>
     * @return This builder for chaining.
     */
    public Builder clearGateiid() {
      
      gateiid_ = 0L;
      onChanged();
      return this;
    }

    private int hgtype_ ;
    /**
     * <code>int32 hgtype = 13;</code>
     * @return The hgtype.
     */
    @java.lang.Override
    public int getHgtype() {
      return hgtype_;
    }
    /**
     * <code>int32 hgtype = 13;</code>
     * @param value The hgtype to set.
     * @return This builder for chaining.
     */
    public Builder setHgtype(int value) {
      
      hgtype_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int32 hgtype = 13;</code>
     * @return This builder for chaining.
     */
    public Builder clearHgtype() {
      
      hgtype_ = 0;
      onChanged();
      return this;
    }

    private long lsno_ ;
    /**
     * <pre>
     *userslotreq附带信息
     * </pre>
     *
     * <code>int64 lsno = 20;</code>
     * @return The lsno.
     */
    @java.lang.Override
    public long getLsno() {
      return lsno_;
    }
    /**
     * <pre>
     *userslotreq附带信息
     * </pre>
     *
     * <code>int64 lsno = 20;</code>
     * @param value The lsno to set.
     * @return This builder for chaining.
     */
    public Builder setLsno(long value) {
      
      lsno_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *userslotreq附带信息
     * </pre>
     *
     * <code>int64 lsno = 20;</code>
     * @return This builder for chaining.
     */
    public Builder clearLsno() {
      
      lsno_ = 0L;
      onChanged();
      return this;
    }

    private long sequence_ ;
    /**
     * <code>int64 sequence = 21;</code>
     * @return The sequence.
     */
    @java.lang.Override
    public long getSequence() {
      return sequence_;
    }
    /**
     * <code>int64 sequence = 21;</code>
     * @param value The sequence to set.
     * @return This builder for chaining.
     */
    public Builder setSequence(long value) {
      
      sequence_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 sequence = 21;</code>
     * @return This builder for chaining.
     */
    public Builder clearSequence() {
      
      sequence_ = 0L;
      onChanged();
      return this;
    }

    private long userIid_ ;
    /**
     * <code>int64 user_iid = 22;</code>
     * @return The userIid.
     */
    @java.lang.Override
    public long getUserIid() {
      return userIid_;
    }
    /**
     * <code>int64 user_iid = 22;</code>
     * @param value The userIid to set.
     * @return This builder for chaining.
     */
    public Builder setUserIid(long value) {
      
      userIid_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 user_iid = 22;</code>
     * @return This builder for chaining.
     */
    public Builder clearUserIid() {
      
      userIid_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.Res_ProxyUserSlot_req)
  }

  // @@protoc_insertion_point(class_scope:PRO.Res_ProxyUserSlot_req)
  private static final com.cms.client.network.gpro.Res_ProxyUserSlot_req DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Res_ProxyUserSlot_req();
  }

  public static com.cms.client.network.gpro.Res_ProxyUserSlot_req getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Res_ProxyUserSlot_req>
      PARSER = new com.google.protobuf.AbstractParser<Res_ProxyUserSlot_req>() {
    @java.lang.Override
    public Res_ProxyUserSlot_req parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Res_ProxyUserSlot_req(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Res_ProxyUserSlot_req> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Res_ProxyUserSlot_req> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Res_ProxyUserSlot_req getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

