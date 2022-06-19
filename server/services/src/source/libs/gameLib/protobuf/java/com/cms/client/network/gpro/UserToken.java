// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: global_internal.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.UserToken}
 */
public final class UserToken extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.UserToken)
    UserTokenOrBuilder {
private static final long serialVersionUID = 0L;
  // Use UserToken.newBuilder() to construct.
  private UserToken(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private UserToken() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new UserToken();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private UserToken(
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

            giduid_ = input.readInt64();
            break;
          }
          case 16: {

            slottoken_ = input.readInt64();
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
    return com.cms.client.network.gpro.GlobalInternal.internal_static_PRO_UserToken_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.GlobalInternal.internal_static_PRO_UserToken_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.UserToken.class, com.cms.client.network.gpro.UserToken.Builder.class);
  }

  public static final int GIDUID_FIELD_NUMBER = 1;
  private long giduid_;
  /**
   * <pre>
   *20bit gateid+43bit userid
   * </pre>
   *
   * <code>int64 giduid = 1;</code>
   * @return The giduid.
   */
  @java.lang.Override
  public long getGiduid() {
    return giduid_;
  }

  public static final int SLOTTOKEN_FIELD_NUMBER = 2;
  private long slottoken_;
  /**
   * <code>int64 slottoken = 2;</code>
   * @return The slottoken.
   */
  @java.lang.Override
  public long getSlottoken() {
    return slottoken_;
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
    if (giduid_ != 0L) {
      output.writeInt64(1, giduid_);
    }
    if (slottoken_ != 0L) {
      output.writeInt64(2, slottoken_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (giduid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(1, giduid_);
    }
    if (slottoken_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, slottoken_);
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
    if (!(obj instanceof com.cms.client.network.gpro.UserToken)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.UserToken other = (com.cms.client.network.gpro.UserToken) obj;

    if (getGiduid()
        != other.getGiduid()) return false;
    if (getSlottoken()
        != other.getSlottoken()) return false;
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
    hash = (37 * hash) + GIDUID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getGiduid());
    hash = (37 * hash) + SLOTTOKEN_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getSlottoken());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.UserToken parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.UserToken parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.UserToken parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.UserToken parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.UserToken prototype) {
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
   * Protobuf type {@code PRO.UserToken}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.UserToken)
      com.cms.client.network.gpro.UserTokenOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.GlobalInternal.internal_static_PRO_UserToken_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.GlobalInternal.internal_static_PRO_UserToken_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.UserToken.class, com.cms.client.network.gpro.UserToken.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.UserToken.newBuilder()
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
      giduid_ = 0L;

      slottoken_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.GlobalInternal.internal_static_PRO_UserToken_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.UserToken getDefaultInstanceForType() {
      return com.cms.client.network.gpro.UserToken.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.UserToken build() {
      com.cms.client.network.gpro.UserToken result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.UserToken buildPartial() {
      com.cms.client.network.gpro.UserToken result = new com.cms.client.network.gpro.UserToken(this);
      result.giduid_ = giduid_;
      result.slottoken_ = slottoken_;
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
      if (other instanceof com.cms.client.network.gpro.UserToken) {
        return mergeFrom((com.cms.client.network.gpro.UserToken)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.UserToken other) {
      if (other == com.cms.client.network.gpro.UserToken.getDefaultInstance()) return this;
      if (other.getGiduid() != 0L) {
        setGiduid(other.getGiduid());
      }
      if (other.getSlottoken() != 0L) {
        setSlottoken(other.getSlottoken());
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
      com.cms.client.network.gpro.UserToken parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.UserToken) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private long giduid_ ;
    /**
     * <pre>
     *20bit gateid+43bit userid
     * </pre>
     *
     * <code>int64 giduid = 1;</code>
     * @return The giduid.
     */
    @java.lang.Override
    public long getGiduid() {
      return giduid_;
    }
    /**
     * <pre>
     *20bit gateid+43bit userid
     * </pre>
     *
     * <code>int64 giduid = 1;</code>
     * @param value The giduid to set.
     * @return This builder for chaining.
     */
    public Builder setGiduid(long value) {
      
      giduid_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     *20bit gateid+43bit userid
     * </pre>
     *
     * <code>int64 giduid = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearGiduid() {
      
      giduid_ = 0L;
      onChanged();
      return this;
    }

    private long slottoken_ ;
    /**
     * <code>int64 slottoken = 2;</code>
     * @return The slottoken.
     */
    @java.lang.Override
    public long getSlottoken() {
      return slottoken_;
    }
    /**
     * <code>int64 slottoken = 2;</code>
     * @param value The slottoken to set.
     * @return This builder for chaining.
     */
    public Builder setSlottoken(long value) {
      
      slottoken_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 slottoken = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearSlottoken() {
      
      slottoken_ = 0L;
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


    // @@protoc_insertion_point(builder_scope:PRO.UserToken)
  }

  // @@protoc_insertion_point(class_scope:PRO.UserToken)
  private static final com.cms.client.network.gpro.UserToken DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.UserToken();
  }

  public static com.cms.client.network.gpro.UserToken getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<UserToken>
      PARSER = new com.google.protobuf.AbstractParser<UserToken>() {
    @java.lang.Override
    public UserToken parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new UserToken(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<UserToken> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<UserToken> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.UserToken getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

