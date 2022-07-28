// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db_internal.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.DBUserTaskGroupEnd}
 */
public final class DBUserTaskGroupEnd extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.DBUserTaskGroupEnd)
    DBUserTaskGroupEndOrBuilder {
private static final long serialVersionUID = 0L;
  // Use DBUserTaskGroupEnd.newBuilder() to construct.
  private DBUserTaskGroupEnd(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private DBUserTaskGroupEnd() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new DBUserTaskGroupEnd();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private DBUserTaskGroupEnd(
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

            ver_ = input.readUInt32();
            break;
          }
          case 16: {

            iid_ = input.readInt64();
            break;
          }
          case 32: {

            taskGroup_ = input.readInt32();
            break;
          }
          case 56: {

            gstate_ = input.readInt32();
            break;
          }
          case 80: {

            endtime_ = input.readInt32();
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
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTaskGroupEnd_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTaskGroupEnd_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.DBUserTaskGroupEnd.class, com.cms.client.network.gpro.DBUserTaskGroupEnd.Builder.class);
  }

  public static final int VER__FIELD_NUMBER = 1;
  private int ver_;
  /**
   * <code>uint32 ver_ = 1;</code>
   * @return The ver.
   */
  @java.lang.Override
  public int getVer() {
    return ver_;
  }

  public static final int IID_FIELD_NUMBER = 2;
  private long iid_;
  /**
   * <code>int64 iid = 2;</code>
   * @return The iid.
   */
  @java.lang.Override
  public long getIid() {
    return iid_;
  }

  public static final int TASK_GROUP_FIELD_NUMBER = 4;
  private int taskGroup_;
  /**
   * <code>int32 task_group = 4;</code>
   * @return The taskGroup.
   */
  @java.lang.Override
  public int getTaskGroup() {
    return taskGroup_;
  }

  public static final int GSTATE_FIELD_NUMBER = 7;
  private int gstate_;
  /**
   * <code>int32 gstate = 7;</code>
   * @return The gstate.
   */
  @java.lang.Override
  public int getGstate() {
    return gstate_;
  }

  public static final int ENDTIME_FIELD_NUMBER = 10;
  private int endtime_;
  /**
   * <code>int32 endtime = 10;</code>
   * @return The endtime.
   */
  @java.lang.Override
  public int getEndtime() {
    return endtime_;
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
      output.writeUInt32(1, ver_);
    }
    if (iid_ != 0L) {
      output.writeInt64(2, iid_);
    }
    if (taskGroup_ != 0) {
      output.writeInt32(4, taskGroup_);
    }
    if (gstate_ != 0) {
      output.writeInt32(7, gstate_);
    }
    if (endtime_ != 0) {
      output.writeInt32(10, endtime_);
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
        .computeUInt32Size(1, ver_);
    }
    if (iid_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, iid_);
    }
    if (taskGroup_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(4, taskGroup_);
    }
    if (gstate_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(7, gstate_);
    }
    if (endtime_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt32Size(10, endtime_);
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
    if (!(obj instanceof com.cms.client.network.gpro.DBUserTaskGroupEnd)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.DBUserTaskGroupEnd other = (com.cms.client.network.gpro.DBUserTaskGroupEnd) obj;

    if (getVer()
        != other.getVer()) return false;
    if (getIid()
        != other.getIid()) return false;
    if (getTaskGroup()
        != other.getTaskGroup()) return false;
    if (getGstate()
        != other.getGstate()) return false;
    if (getEndtime()
        != other.getEndtime()) return false;
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
    hash = (37 * hash) + IID_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getIid());
    hash = (37 * hash) + TASK_GROUP_FIELD_NUMBER;
    hash = (53 * hash) + getTaskGroup();
    hash = (37 * hash) + GSTATE_FIELD_NUMBER;
    hash = (53 * hash) + getGstate();
    hash = (37 * hash) + ENDTIME_FIELD_NUMBER;
    hash = (53 * hash) + getEndtime();
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTaskGroupEnd parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.DBUserTaskGroupEnd prototype) {
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
   * Protobuf type {@code PRO.DBUserTaskGroupEnd}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.DBUserTaskGroupEnd)
      com.cms.client.network.gpro.DBUserTaskGroupEndOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTaskGroupEnd_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTaskGroupEnd_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.DBUserTaskGroupEnd.class, com.cms.client.network.gpro.DBUserTaskGroupEnd.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.DBUserTaskGroupEnd.newBuilder()
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

      iid_ = 0L;

      taskGroup_ = 0;

      gstate_ = 0;

      endtime_ = 0;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTaskGroupEnd_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTaskGroupEnd getDefaultInstanceForType() {
      return com.cms.client.network.gpro.DBUserTaskGroupEnd.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTaskGroupEnd build() {
      com.cms.client.network.gpro.DBUserTaskGroupEnd result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTaskGroupEnd buildPartial() {
      com.cms.client.network.gpro.DBUserTaskGroupEnd result = new com.cms.client.network.gpro.DBUserTaskGroupEnd(this);
      result.ver_ = ver_;
      result.iid_ = iid_;
      result.taskGroup_ = taskGroup_;
      result.gstate_ = gstate_;
      result.endtime_ = endtime_;
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
      if (other instanceof com.cms.client.network.gpro.DBUserTaskGroupEnd) {
        return mergeFrom((com.cms.client.network.gpro.DBUserTaskGroupEnd)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.DBUserTaskGroupEnd other) {
      if (other == com.cms.client.network.gpro.DBUserTaskGroupEnd.getDefaultInstance()) return this;
      if (other.getVer() != 0) {
        setVer(other.getVer());
      }
      if (other.getIid() != 0L) {
        setIid(other.getIid());
      }
      if (other.getTaskGroup() != 0) {
        setTaskGroup(other.getTaskGroup());
      }
      if (other.getGstate() != 0) {
        setGstate(other.getGstate());
      }
      if (other.getEndtime() != 0) {
        setEndtime(other.getEndtime());
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
      com.cms.client.network.gpro.DBUserTaskGroupEnd parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.DBUserTaskGroupEnd) e.getUnfinishedMessage();
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
     * <code>uint32 ver_ = 1;</code>
     * @return The ver.
     */
    @java.lang.Override
    public int getVer() {
      return ver_;
    }
    /**
     * <code>uint32 ver_ = 1;</code>
     * @param value The ver to set.
     * @return This builder for chaining.
     */
    public Builder setVer(int value) {
      
      ver_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>uint32 ver_ = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearVer() {
      
      ver_ = 0;
      onChanged();
      return this;
    }

    private long iid_ ;
    /**
     * <code>int64 iid = 2;</code>
     * @return The iid.
     */
    @java.lang.Override
    public long getIid() {
      return iid_;
    }
    /**
     * <code>int64 iid = 2;</code>
     * @param value The iid to set.
     * @return This builder for chaining.
     */
    public Builder setIid(long value) {
      
      iid_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 iid = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearIid() {
      
      iid_ = 0L;
      onChanged();
      return this;
    }

    private int taskGroup_ ;
    /**
     * <code>int32 task_group = 4;</code>
     * @return The taskGroup.
     */
    @java.lang.Override
    public int getTaskGroup() {
      return taskGroup_;
    }
    /**
     * <code>int32 task_group = 4;</code>
     * @param value The taskGroup to set.
     * @return This builder for chaining.
     */
    public Builder setTaskGroup(int value) {
      
      taskGroup_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int32 task_group = 4;</code>
     * @return This builder for chaining.
     */
    public Builder clearTaskGroup() {
      
      taskGroup_ = 0;
      onChanged();
      return this;
    }

    private int gstate_ ;
    /**
     * <code>int32 gstate = 7;</code>
     * @return The gstate.
     */
    @java.lang.Override
    public int getGstate() {
      return gstate_;
    }
    /**
     * <code>int32 gstate = 7;</code>
     * @param value The gstate to set.
     * @return This builder for chaining.
     */
    public Builder setGstate(int value) {
      
      gstate_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int32 gstate = 7;</code>
     * @return This builder for chaining.
     */
    public Builder clearGstate() {
      
      gstate_ = 0;
      onChanged();
      return this;
    }

    private int endtime_ ;
    /**
     * <code>int32 endtime = 10;</code>
     * @return The endtime.
     */
    @java.lang.Override
    public int getEndtime() {
      return endtime_;
    }
    /**
     * <code>int32 endtime = 10;</code>
     * @param value The endtime to set.
     * @return This builder for chaining.
     */
    public Builder setEndtime(int value) {
      
      endtime_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int32 endtime = 10;</code>
     * @return This builder for chaining.
     */
    public Builder clearEndtime() {
      
      endtime_ = 0;
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


    // @@protoc_insertion_point(builder_scope:PRO.DBUserTaskGroupEnd)
  }

  // @@protoc_insertion_point(class_scope:PRO.DBUserTaskGroupEnd)
  private static final com.cms.client.network.gpro.DBUserTaskGroupEnd DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.DBUserTaskGroupEnd();
  }

  public static com.cms.client.network.gpro.DBUserTaskGroupEnd getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<DBUserTaskGroupEnd>
      PARSER = new com.google.protobuf.AbstractParser<DBUserTaskGroupEnd>() {
    @java.lang.Override
    public DBUserTaskGroupEnd parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new DBUserTaskGroupEnd(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<DBUserTaskGroupEnd> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<DBUserTaskGroupEnd> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTaskGroupEnd getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}
