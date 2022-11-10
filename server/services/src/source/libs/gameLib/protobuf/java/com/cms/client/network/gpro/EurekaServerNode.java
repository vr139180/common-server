// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

/**
 * <pre>
 *eureka server node info
 * </pre>
 *
 * Protobuf type {@code PRO.EurekaServerNode}
 */
public final class EurekaServerNode extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.EurekaServerNode)
    EurekaServerNodeOrBuilder {
private static final long serialVersionUID = 0L;
  // Use EurekaServerNode.newBuilder() to construct.
  private EurekaServerNode(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private EurekaServerNode() {
    ip_ = "";
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new EurekaServerNode();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private EurekaServerNode(
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

            iid_ = input.readInt64();
            break;
          }
          case 16: {

            token_ = input.readInt64();
            break;
          }
          case 26: {
            java.lang.String s = input.readStringRequireUtf8();

            ip_ = s;
            break;
          }
          case 32: {

            port_ = input.readInt32();
            break;
          }
          case 40: {

            ismaster_ = input.readBool();
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
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_EurekaServerNode_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_EurekaServerNode_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.EurekaServerNode.class, com.cms.client.network.gpro.EurekaServerNode.Builder.class);
  }

  public static final int IID_FIELD_NUMBER = 1;
  private long iid_;
  /**
   * <pre>
   *服务器sn
   * </pre>
   *
   * <code>int64 iid = 1;</code>
   * @return The iid.
   */
  @java.lang.Override
  public long getIid() {
    return iid_;
  }

  public static final int TOKEN_FIELD_NUMBER = 2;
  private long token_;
  /**
   * <pre>
   *对方token，用于验证
   * </pre>
   *
   * <code>int64 token = 2;</code>
   * @return The token.
   */
  @java.lang.Override
  public long getToken() {
    return token_;
  }

  public static final int IP_FIELD_NUMBER = 3;
  private volatile java.lang.Object ip_;
  /**
   * <pre>
   *动态ip
   * </pre>
   *
   * <code>string ip = 3;</code>
   * @return The ip.
   */
  @java.lang.Override
  public java.lang.String getIp() {
    java.lang.Object ref = ip_;
    if (ref instanceof java.lang.String) {
      return (java.lang.String) ref;
    } else {
      com.google.protobuf.ByteString bs = 
          (com.google.protobuf.ByteString) ref;
      java.lang.String s = bs.toStringUtf8();
      ip_ = s;
      return s;
    }
  }
  /**
   * <pre>
   *动态ip
   * </pre>
   *
   * <code>string ip = 3;</code>
   * @return The bytes for ip.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString
      getIpBytes() {
    java.lang.Object ref = ip_;
    if (ref instanceof java.lang.String) {
      com.google.protobuf.ByteString b = 
          com.google.protobuf.ByteString.copyFromUtf8(
              (java.lang.String) ref);
      ip_ = b;
      return b;
    } else {
      return (com.google.protobuf.ByteString) ref;
    }
  }

  public static final int PORT_FIELD_NUMBER = 4;
  private int port_;
  /**
   * <pre>
   *服务器port
   * </pre>
   *
   * <code>int32 port = 4;</code>
   * @return The port.
   */
  @java.lang.Override
  public int getPort() {
    return port_;
  }

  public static final int ISMASTER_FIELD_NUMBER = 5;
  private boolean ismaster_;
  /**
   * <pre>
   *是否master节点
   * </pre>
   *
   * <code>bool ismaster = 5;</code>
   * @return The ismaster.
   */
  @java.lang.Override
  public boolean getIsmaster() {
    return ismaster_;
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
    if (iid_ != 0L) {
      output.writeInt64(1, iid_);
    }
    if (token_ != 0L) {
      output.writeInt64(2, token_);
    }
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(ip_)) {
      com.google.protobuf.GeneratedMessageV3.writeString(output, 3, ip_);
    }
    if (port_ != 0) {
      output.writeInt32(4, port_);
    }
    if (ismaster_ != false) {
      output.writeBool(5, ismaster_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (iid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(1, iid_);
    }
    if (token_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, token_);
    }
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(ip_)) {
      size += com.google.protobuf.GeneratedMessageV3.computeStringSize(3, ip_);
    }
    if (port_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(4, port_);
    }
    if (ismaster_ != false) {
      size += com.google.protobuf.CodedOutputStream
        .computeBoolSize(5, ismaster_);
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
    if (!(obj instanceof com.cms.client.network.gpro.EurekaServerNode)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.EurekaServerNode other = (com.cms.client.network.gpro.EurekaServerNode) obj;

    if (getIid()
        != other.getIid()) return false;
    if (getToken()
        != other.getToken()) return false;
    if (!getIp()
        .equals(other.getIp())) return false;
    if (getPort()
        != other.getPort()) return false;
    if (getIsmaster()
        != other.getIsmaster()) return false;
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
    hash = (37 * hash) + IID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getIid());
    hash = (37 * hash) + TOKEN_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getToken());
    hash = (37 * hash) + IP_FIELD_NUMBER;
    hash = (53 * hash) + getIp().hashCode();
    hash = (37 * hash) + PORT_FIELD_NUMBER;
    hash = (53 * hash) + getPort();
    hash = (37 * hash) + ISMASTER_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashBoolean(
        getIsmaster());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.EurekaServerNode parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.EurekaServerNode prototype) {
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
   *eureka server node info
   * </pre>
   *
   * Protobuf type {@code PRO.EurekaServerNode}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.EurekaServerNode)
      com.cms.client.network.gpro.EurekaServerNodeOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_EurekaServerNode_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_EurekaServerNode_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.EurekaServerNode.class, com.cms.client.network.gpro.EurekaServerNode.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.EurekaServerNode.newBuilder()
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
      iid_ = 0L;

      token_ = 0L;

      ip_ = "";

      port_ = 0;

      ismaster_ = false;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_EurekaServerNode_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.EurekaServerNode getDefaultInstanceForType() {
      return com.cms.client.network.gpro.EurekaServerNode.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.EurekaServerNode build() {
      com.cms.client.network.gpro.EurekaServerNode result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.EurekaServerNode buildPartial() {
      com.cms.client.network.gpro.EurekaServerNode result = new com.cms.client.network.gpro.EurekaServerNode(this);
      result.iid_ = iid_;
      result.token_ = token_;
      result.ip_ = ip_;
      result.port_ = port_;
      result.ismaster_ = ismaster_;
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
      if (other instanceof com.cms.client.network.gpro.EurekaServerNode) {
        return mergeFrom((com.cms.client.network.gpro.EurekaServerNode)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.EurekaServerNode other) {
      if (other == com.cms.client.network.gpro.EurekaServerNode.getDefaultInstance()) return this;
      if (other.getIid() != 0L) {
        setIid(other.getIid());
      }
      if (other.getToken() != 0L) {
        setToken(other.getToken());
      }
      if (!other.getIp().isEmpty()) {
        ip_ = other.ip_;
        onChanged();
      }
      if (other.getPort() != 0) {
        setPort(other.getPort());
      }
      if (other.getIsmaster() != false) {
        setIsmaster(other.getIsmaster());
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
      com.cms.client.network.gpro.EurekaServerNode parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.EurekaServerNode) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private long iid_ ;
    /**
     * <pre>
     *服务器sn
     * </pre>
     *
     * <code>int64 iid = 1;</code>
     * @return The iid.
     */
    @java.lang.Override
    public long getIid() {
      return iid_;
    }
    /**
     * <pre>
     *服务器sn
     * </pre>
     *
     * <code>int64 iid = 1;</code>
     * @param value The iid to set.
     * @return This builder for chaining.
     */
    public Builder setIid(long value) {
      
      iid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *服务器sn
     * </pre>
     *
     * <code>int64 iid = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearIid() {
      
      iid_ = 0L;
      onChanged();
      return this;
    }

    private long token_ ;
    /**
     * <pre>
     *对方token，用于验证
     * </pre>
     *
     * <code>int64 token = 2;</code>
     * @return The token.
     */
    @java.lang.Override
    public long getToken() {
      return token_;
    }
    /**
     * <pre>
     *对方token，用于验证
     * </pre>
     *
     * <code>int64 token = 2;</code>
     * @param value The token to set.
     * @return This builder for chaining.
     */
    public Builder setToken(long value) {
      
      token_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *对方token，用于验证
     * </pre>
     *
     * <code>int64 token = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearToken() {
      
      token_ = 0L;
      onChanged();
      return this;
    }

    private java.lang.Object ip_ = "";
    /**
     * <pre>
     *动态ip
     * </pre>
     *
     * <code>string ip = 3;</code>
     * @return The ip.
     */
    public java.lang.String getIp() {
      java.lang.Object ref = ip_;
      if (!(ref instanceof java.lang.String)) {
        com.google.protobuf.ByteString bs =
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        ip_ = s;
        return s;
      } else {
        return (java.lang.String) ref;
      }
    }
    /**
     * <pre>
     *动态ip
     * </pre>
     *
     * <code>string ip = 3;</code>
     * @return The bytes for ip.
     */
    public com.google.protobuf.ByteString
        getIpBytes() {
      java.lang.Object ref = ip_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        ip_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    /**
     * <pre>
     *动态ip
     * </pre>
     *
     * <code>string ip = 3;</code>
     * @param value The ip to set.
     * @return This builder for chaining.
     */
    public Builder setIp(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      ip_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *动态ip
     * </pre>
     *
     * <code>string ip = 3;</code>
     * @return This builder for chaining.
     */
    public Builder clearIp() {
      
      ip_ = getDefaultInstance().getIp();
      onChanged();
      return this;
    }
    /**
     * <pre>
     *动态ip
     * </pre>
     *
     * <code>string ip = 3;</code>
     * @param value The bytes for ip to set.
     * @return This builder for chaining.
     */
    public Builder setIpBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
      
      ip_ = value;
      onChanged();
      return this;
    }

    private int port_ ;
    /**
     * <pre>
     *服务器port
     * </pre>
     *
     * <code>int32 port = 4;</code>
     * @return The port.
     */
    @java.lang.Override
    public int getPort() {
      return port_;
    }
    /**
     * <pre>
     *服务器port
     * </pre>
     *
     * <code>int32 port = 4;</code>
     * @param value The port to set.
     * @return This builder for chaining.
     */
    public Builder setPort(int value) {
      
      port_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *服务器port
     * </pre>
     *
     * <code>int32 port = 4;</code>
     * @return This builder for chaining.
     */
    public Builder clearPort() {
      
      port_ = 0;
      onChanged();
      return this;
    }

    private boolean ismaster_ ;
    /**
     * <pre>
     *是否master节点
     * </pre>
     *
     * <code>bool ismaster = 5;</code>
     * @return The ismaster.
     */
    @java.lang.Override
    public boolean getIsmaster() {
      return ismaster_;
    }
    /**
     * <pre>
     *是否master节点
     * </pre>
     *
     * <code>bool ismaster = 5;</code>
     * @param value The ismaster to set.
     * @return This builder for chaining.
     */
    public Builder setIsmaster(boolean value) {
      
      ismaster_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *是否master节点
     * </pre>
     *
     * <code>bool ismaster = 5;</code>
     * @return This builder for chaining.
     */
    public Builder clearIsmaster() {
      
      ismaster_ = false;
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


    // @@protoc_insertion_point(builder_scope:PRO.EurekaServerNode)
  }

  // @@protoc_insertion_point(class_scope:PRO.EurekaServerNode)
  private static final com.cms.client.network.gpro.EurekaServerNode DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.EurekaServerNode();
  }

  public static com.cms.client.network.gpro.EurekaServerNode getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<EurekaServerNode>
      PARSER = new com.google.protobuf.AbstractParser<EurekaServerNode>() {
    @java.lang.Override
    public EurekaServerNode parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new EurekaServerNode(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<EurekaServerNode> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<EurekaServerNode> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.EurekaServerNode getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

