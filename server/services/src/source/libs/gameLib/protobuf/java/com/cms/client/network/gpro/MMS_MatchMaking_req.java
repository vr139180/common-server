// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: matchmaking_system.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.MMS_MatchMaking_req}
 */
public final class MMS_MatchMaking_req extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.MMS_MatchMaking_req)
    MMS_MatchMaking_reqOrBuilder {
private static final long serialVersionUID = 0L;
  // Use MMS_MatchMaking_req.newBuilder() to construct.
  private MMS_MatchMaking_req(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private MMS_MatchMaking_req() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new MMS_MatchMaking_req();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private MMS_MatchMaking_req(
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
          case 10: {
            com.cms.client.network.gpro.UserToken.Builder subBuilder = null;
            if (((bitField0_ & 0x00000001) != 0)) {
              subBuilder = utoken_.toBuilder();
            }
            utoken_ = input.readMessage(com.cms.client.network.gpro.UserToken.parser(), extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(utoken_);
              utoken_ = subBuilder.buildPartial();
            }
            bitField0_ |= 0x00000001;
            break;
          }
          case 18: {
            com.cms.client.network.gpro.DBUserBattles.Builder subBuilder = null;
            if (battles_ != null) {
              subBuilder = battles_.toBuilder();
            }
            battles_ = input.readMessage(com.cms.client.network.gpro.DBUserBattles.parser(), extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(battles_);
              battles_ = subBuilder.buildPartial();
            }

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
    return com.cms.client.network.gpro.MatchmakingSystem.internal_static_PRO_MMS_MatchMaking_req_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.MatchmakingSystem.internal_static_PRO_MMS_MatchMaking_req_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.MMS_MatchMaking_req.class, com.cms.client.network.gpro.MMS_MatchMaking_req.Builder.class);
  }

  private int bitField0_;
  public static final int UTOKEN_FIELD_NUMBER = 1;
  private com.cms.client.network.gpro.UserToken utoken_;
  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   * @return Whether the utoken field is set.
   */
  @java.lang.Override
  public boolean hasUtoken() {
    return ((bitField0_ & 0x00000001) != 0);
  }
  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   * @return The utoken.
   */
  @java.lang.Override
  public com.cms.client.network.gpro.UserToken getUtoken() {
    return utoken_ == null ? com.cms.client.network.gpro.UserToken.getDefaultInstance() : utoken_;
  }
  /**
   * <code>optional .PRO.UserToken utoken = 1;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.UserTokenOrBuilder getUtokenOrBuilder() {
    return utoken_ == null ? com.cms.client.network.gpro.UserToken.getDefaultInstance() : utoken_;
  }

  public static final int BATTLES_FIELD_NUMBER = 2;
  private com.cms.client.network.gpro.DBUserBattles battles_;
  /**
   * <code>.PRO.DBUserBattles battles = 2;</code>
   * @return Whether the battles field is set.
   */
  @java.lang.Override
  public boolean hasBattles() {
    return battles_ != null;
  }
  /**
   * <code>.PRO.DBUserBattles battles = 2;</code>
   * @return The battles.
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserBattles getBattles() {
    return battles_ == null ? com.cms.client.network.gpro.DBUserBattles.getDefaultInstance() : battles_;
  }
  /**
   * <code>.PRO.DBUserBattles battles = 2;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserBattlesOrBuilder getBattlesOrBuilder() {
    return getBattles();
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
    if (((bitField0_ & 0x00000001) != 0)) {
      output.writeMessage(1, getUtoken());
    }
    if (battles_ != null) {
      output.writeMessage(2, getBattles());
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (((bitField0_ & 0x00000001) != 0)) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(1, getUtoken());
    }
    if (battles_ != null) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(2, getBattles());
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
    if (!(obj instanceof com.cms.client.network.gpro.MMS_MatchMaking_req)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.MMS_MatchMaking_req other = (com.cms.client.network.gpro.MMS_MatchMaking_req) obj;

    if (hasUtoken() != other.hasUtoken()) return false;
    if (hasUtoken()) {
      if (!getUtoken()
          .equals(other.getUtoken())) return false;
    }
    if (hasBattles() != other.hasBattles()) return false;
    if (hasBattles()) {
      if (!getBattles()
          .equals(other.getBattles())) return false;
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
    if (hasUtoken()) {
      hash = (37 * hash) + UTOKEN_FIELD_NUMBER;
      hash = (53 * hash) + getUtoken().hashCode();
    }
    if (hasBattles()) {
      hash = (37 * hash) + BATTLES_FIELD_NUMBER;
      hash = (53 * hash) + getBattles().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.MMS_MatchMaking_req parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.MMS_MatchMaking_req prototype) {
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
   * Protobuf type {@code PRO.MMS_MatchMaking_req}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.MMS_MatchMaking_req)
      com.cms.client.network.gpro.MMS_MatchMaking_reqOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.MatchmakingSystem.internal_static_PRO_MMS_MatchMaking_req_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.MatchmakingSystem.internal_static_PRO_MMS_MatchMaking_req_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.MMS_MatchMaking_req.class, com.cms.client.network.gpro.MMS_MatchMaking_req.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.MMS_MatchMaking_req.newBuilder()
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
        getUtokenFieldBuilder();
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      if (utokenBuilder_ == null) {
        utoken_ = null;
      } else {
        utokenBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000001);
      if (battlesBuilder_ == null) {
        battles_ = null;
      } else {
        battles_ = null;
        battlesBuilder_ = null;
      }
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.MatchmakingSystem.internal_static_PRO_MMS_MatchMaking_req_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MMS_MatchMaking_req getDefaultInstanceForType() {
      return com.cms.client.network.gpro.MMS_MatchMaking_req.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MMS_MatchMaking_req build() {
      com.cms.client.network.gpro.MMS_MatchMaking_req result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.MMS_MatchMaking_req buildPartial() {
      com.cms.client.network.gpro.MMS_MatchMaking_req result = new com.cms.client.network.gpro.MMS_MatchMaking_req(this);
      int from_bitField0_ = bitField0_;
      int to_bitField0_ = 0;
      if (((from_bitField0_ & 0x00000001) != 0)) {
        if (utokenBuilder_ == null) {
          result.utoken_ = utoken_;
        } else {
          result.utoken_ = utokenBuilder_.build();
        }
        to_bitField0_ |= 0x00000001;
      }
      if (battlesBuilder_ == null) {
        result.battles_ = battles_;
      } else {
        result.battles_ = battlesBuilder_.build();
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
      if (other instanceof com.cms.client.network.gpro.MMS_MatchMaking_req) {
        return mergeFrom((com.cms.client.network.gpro.MMS_MatchMaking_req)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.MMS_MatchMaking_req other) {
      if (other == com.cms.client.network.gpro.MMS_MatchMaking_req.getDefaultInstance()) return this;
      if (other.hasUtoken()) {
        mergeUtoken(other.getUtoken());
      }
      if (other.hasBattles()) {
        mergeBattles(other.getBattles());
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
      com.cms.client.network.gpro.MMS_MatchMaking_req parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.MMS_MatchMaking_req) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private com.cms.client.network.gpro.UserToken utoken_;
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.UserToken, com.cms.client.network.gpro.UserToken.Builder, com.cms.client.network.gpro.UserTokenOrBuilder> utokenBuilder_;
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     * @return Whether the utoken field is set.
     */
    public boolean hasUtoken() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     * @return The utoken.
     */
    public com.cms.client.network.gpro.UserToken getUtoken() {
      if (utokenBuilder_ == null) {
        return utoken_ == null ? com.cms.client.network.gpro.UserToken.getDefaultInstance() : utoken_;
      } else {
        return utokenBuilder_.getMessage();
      }
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public Builder setUtoken(com.cms.client.network.gpro.UserToken value) {
      if (utokenBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        utoken_ = value;
        onChanged();
      } else {
        utokenBuilder_.setMessage(value);
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public Builder setUtoken(
        com.cms.client.network.gpro.UserToken.Builder builderForValue) {
      if (utokenBuilder_ == null) {
        utoken_ = builderForValue.build();
        onChanged();
      } else {
        utokenBuilder_.setMessage(builderForValue.build());
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public Builder mergeUtoken(com.cms.client.network.gpro.UserToken value) {
      if (utokenBuilder_ == null) {
        if (((bitField0_ & 0x00000001) != 0) &&
            utoken_ != null &&
            utoken_ != com.cms.client.network.gpro.UserToken.getDefaultInstance()) {
          utoken_ =
            com.cms.client.network.gpro.UserToken.newBuilder(utoken_).mergeFrom(value).buildPartial();
        } else {
          utoken_ = value;
        }
        onChanged();
      } else {
        utokenBuilder_.mergeFrom(value);
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public Builder clearUtoken() {
      if (utokenBuilder_ == null) {
        utoken_ = null;
        onChanged();
      } else {
        utokenBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000001);
      return this;
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public com.cms.client.network.gpro.UserToken.Builder getUtokenBuilder() {
      bitField0_ |= 0x00000001;
      onChanged();
      return getUtokenFieldBuilder().getBuilder();
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    public com.cms.client.network.gpro.UserTokenOrBuilder getUtokenOrBuilder() {
      if (utokenBuilder_ != null) {
        return utokenBuilder_.getMessageOrBuilder();
      } else {
        return utoken_ == null ?
            com.cms.client.network.gpro.UserToken.getDefaultInstance() : utoken_;
      }
    }
    /**
     * <code>optional .PRO.UserToken utoken = 1;</code>
     */
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.UserToken, com.cms.client.network.gpro.UserToken.Builder, com.cms.client.network.gpro.UserTokenOrBuilder> 
        getUtokenFieldBuilder() {
      if (utokenBuilder_ == null) {
        utokenBuilder_ = new com.google.protobuf.SingleFieldBuilderV3<
            com.cms.client.network.gpro.UserToken, com.cms.client.network.gpro.UserToken.Builder, com.cms.client.network.gpro.UserTokenOrBuilder>(
                getUtoken(),
                getParentForChildren(),
                isClean());
        utoken_ = null;
      }
      return utokenBuilder_;
    }

    private com.cms.client.network.gpro.DBUserBattles battles_;
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserBattles, com.cms.client.network.gpro.DBUserBattles.Builder, com.cms.client.network.gpro.DBUserBattlesOrBuilder> battlesBuilder_;
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     * @return Whether the battles field is set.
     */
    public boolean hasBattles() {
      return battlesBuilder_ != null || battles_ != null;
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     * @return The battles.
     */
    public com.cms.client.network.gpro.DBUserBattles getBattles() {
      if (battlesBuilder_ == null) {
        return battles_ == null ? com.cms.client.network.gpro.DBUserBattles.getDefaultInstance() : battles_;
      } else {
        return battlesBuilder_.getMessage();
      }
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public Builder setBattles(com.cms.client.network.gpro.DBUserBattles value) {
      if (battlesBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        battles_ = value;
        onChanged();
      } else {
        battlesBuilder_.setMessage(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public Builder setBattles(
        com.cms.client.network.gpro.DBUserBattles.Builder builderForValue) {
      if (battlesBuilder_ == null) {
        battles_ = builderForValue.build();
        onChanged();
      } else {
        battlesBuilder_.setMessage(builderForValue.build());
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public Builder mergeBattles(com.cms.client.network.gpro.DBUserBattles value) {
      if (battlesBuilder_ == null) {
        if (battles_ != null) {
          battles_ =
            com.cms.client.network.gpro.DBUserBattles.newBuilder(battles_).mergeFrom(value).buildPartial();
        } else {
          battles_ = value;
        }
        onChanged();
      } else {
        battlesBuilder_.mergeFrom(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public Builder clearBattles() {
      if (battlesBuilder_ == null) {
        battles_ = null;
        onChanged();
      } else {
        battles_ = null;
        battlesBuilder_ = null;
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public com.cms.client.network.gpro.DBUserBattles.Builder getBattlesBuilder() {
      
      onChanged();
      return getBattlesFieldBuilder().getBuilder();
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    public com.cms.client.network.gpro.DBUserBattlesOrBuilder getBattlesOrBuilder() {
      if (battlesBuilder_ != null) {
        return battlesBuilder_.getMessageOrBuilder();
      } else {
        return battles_ == null ?
            com.cms.client.network.gpro.DBUserBattles.getDefaultInstance() : battles_;
      }
    }
    /**
     * <code>.PRO.DBUserBattles battles = 2;</code>
     */
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserBattles, com.cms.client.network.gpro.DBUserBattles.Builder, com.cms.client.network.gpro.DBUserBattlesOrBuilder> 
        getBattlesFieldBuilder() {
      if (battlesBuilder_ == null) {
        battlesBuilder_ = new com.google.protobuf.SingleFieldBuilderV3<
            com.cms.client.network.gpro.DBUserBattles, com.cms.client.network.gpro.DBUserBattles.Builder, com.cms.client.network.gpro.DBUserBattlesOrBuilder>(
                getBattles(),
                getParentForChildren(),
                isClean());
        battles_ = null;
      }
      return battlesBuilder_;
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


    // @@protoc_insertion_point(builder_scope:PRO.MMS_MatchMaking_req)
  }

  // @@protoc_insertion_point(class_scope:PRO.MMS_MatchMaking_req)
  private static final com.cms.client.network.gpro.MMS_MatchMaking_req DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.MMS_MatchMaking_req();
  }

  public static com.cms.client.network.gpro.MMS_MatchMaking_req getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<MMS_MatchMaking_req>
      PARSER = new com.google.protobuf.AbstractParser<MMS_MatchMaking_req>() {
    @java.lang.Override
    public MMS_MatchMaking_req parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new MMS_MatchMaking_req(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<MMS_MatchMaking_req> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<MMS_MatchMaking_req> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.MMS_MatchMaking_req getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

