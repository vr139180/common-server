// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: eureka_internal.proto

package com.cms.client.network.gpro;

/**
 * <pre>
 *服务下线通知
 * </pre>
 *
 * Protobuf type {@code PRO.Erk_ServiceShutdown_ntf}
 */
public final class Erk_ServiceShutdown_ntf extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Erk_ServiceShutdown_ntf)
    Erk_ServiceShutdown_ntfOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Erk_ServiceShutdown_ntf.newBuilder() to construct.
  private Erk_ServiceShutdown_ntf(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Erk_ServiceShutdown_ntf() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Erk_ServiceShutdown_ntf();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Erk_ServiceShutdown_ntf(
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

            service_ = input.readInt64();
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
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceShutdown_ntf_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceShutdown_ntf_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.class, com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.Builder.class);
  }

  public static final int SERVICE_FIELD_NUMBER = 1;
  private long service_;
  /**
   * <pre>
   *下线的服务
   * </pre>
   *
   * <code>int64 service = 1;</code>
   * @return The service.
   */
  @java.lang.Override
  public long getService() {
    return service_;
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
    if (service_ != 0L) {
      output.writeInt64(1, service_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (service_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(1, service_);
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
    if (!(obj instanceof com.cms.client.network.gpro.Erk_ServiceShutdown_ntf)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Erk_ServiceShutdown_ntf other = (com.cms.client.network.gpro.Erk_ServiceShutdown_ntf) obj;

    if (getService()
        != other.getService()) return false;
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
    hash = (37 * hash) + SERVICE_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getService());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.Erk_ServiceShutdown_ntf prototype) {
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
   *服务下线通知
   * </pre>
   *
   * Protobuf type {@code PRO.Erk_ServiceShutdown_ntf}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Erk_ServiceShutdown_ntf)
      com.cms.client.network.gpro.Erk_ServiceShutdown_ntfOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceShutdown_ntf_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceShutdown_ntf_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.class, com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.newBuilder()
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
      service_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.EurekaInternal.internal_static_PRO_Erk_ServiceShutdown_ntf_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceShutdown_ntf getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceShutdown_ntf build() {
      com.cms.client.network.gpro.Erk_ServiceShutdown_ntf result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Erk_ServiceShutdown_ntf buildPartial() {
      com.cms.client.network.gpro.Erk_ServiceShutdown_ntf result = new com.cms.client.network.gpro.Erk_ServiceShutdown_ntf(this);
      result.service_ = service_;
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
      if (other instanceof com.cms.client.network.gpro.Erk_ServiceShutdown_ntf) {
        return mergeFrom((com.cms.client.network.gpro.Erk_ServiceShutdown_ntf)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Erk_ServiceShutdown_ntf other) {
      if (other == com.cms.client.network.gpro.Erk_ServiceShutdown_ntf.getDefaultInstance()) return this;
      if (other.getService() != 0L) {
        setService(other.getService());
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
      com.cms.client.network.gpro.Erk_ServiceShutdown_ntf parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Erk_ServiceShutdown_ntf) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private long service_ ;
    /**
     * <pre>
     *下线的服务
     * </pre>
     *
     * <code>int64 service = 1;</code>
     * @return The service.
     */
    @java.lang.Override
    public long getService() {
      return service_;
    }
    /**
     * <pre>
     *下线的服务
     * </pre>
     *
     * <code>int64 service = 1;</code>
     * @param value The service to set.
     * @return This builder for chaining.
     */
    public Builder setService(long value) {
      
      service_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *下线的服务
     * </pre>
     *
     * <code>int64 service = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearService() {
      
      service_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.Erk_ServiceShutdown_ntf)
  }

  // @@protoc_insertion_point(class_scope:PRO.Erk_ServiceShutdown_ntf)
  private static final com.cms.client.network.gpro.Erk_ServiceShutdown_ntf DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Erk_ServiceShutdown_ntf();
  }

  public static com.cms.client.network.gpro.Erk_ServiceShutdown_ntf getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Erk_ServiceShutdown_ntf>
      PARSER = new com.google.protobuf.AbstractParser<Erk_ServiceShutdown_ntf>() {
    @java.lang.Override
    public Erk_ServiceShutdown_ntf parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Erk_ServiceShutdown_ntf(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Erk_ServiceShutdown_ntf> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Erk_ServiceShutdown_ntf> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Erk_ServiceShutdown_ntf getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

