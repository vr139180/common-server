// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_system.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.Task_MyTaskList_ack}
 */
public final class Task_MyTaskList_ack extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.Task_MyTaskList_ack)
    Task_MyTaskList_ackOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Task_MyTaskList_ack.newBuilder() to construct.
  private Task_MyTaskList_ack(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Task_MyTaskList_ack() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Task_MyTaskList_ack();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Task_MyTaskList_ack(
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
            com.cms.client.network.gpro.DBUserTaskGroups.Builder subBuilder = null;
            if (groups_ != null) {
              subBuilder = groups_.toBuilder();
            }
            groups_ = input.readMessage(com.cms.client.network.gpro.DBUserTaskGroups.parser(), extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(groups_);
              groups_ = subBuilder.buildPartial();
            }

            break;
          }
          case 26: {
            com.cms.client.network.gpro.DBUserTasks.Builder subBuilder = null;
            if (tasks_ != null) {
              subBuilder = tasks_.toBuilder();
            }
            tasks_ = input.readMessage(com.cms.client.network.gpro.DBUserTasks.parser(), extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(tasks_);
              tasks_ = subBuilder.buildPartial();
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
    return com.cms.client.network.gpro.TaskSystem.internal_static_PRO_Task_MyTaskList_ack_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.TaskSystem.internal_static_PRO_Task_MyTaskList_ack_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.Task_MyTaskList_ack.class, com.cms.client.network.gpro.Task_MyTaskList_ack.Builder.class);
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

  public static final int GROUPS_FIELD_NUMBER = 2;
  private com.cms.client.network.gpro.DBUserTaskGroups groups_;
  /**
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   * @return Whether the groups field is set.
   */
  @java.lang.Override
  public boolean hasGroups() {
    return groups_ != null;
  }
  /**
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   * @return The groups.
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTaskGroups getGroups() {
    return groups_ == null ? com.cms.client.network.gpro.DBUserTaskGroups.getDefaultInstance() : groups_;
  }
  /**
   * <code>.PRO.DBUserTaskGroups groups = 2;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder getGroupsOrBuilder() {
    return getGroups();
  }

  public static final int TASKS_FIELD_NUMBER = 3;
  private com.cms.client.network.gpro.DBUserTasks tasks_;
  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   * @return Whether the tasks field is set.
   */
  @java.lang.Override
  public boolean hasTasks() {
    return tasks_ != null;
  }
  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   * @return The tasks.
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTasks getTasks() {
    return tasks_ == null ? com.cms.client.network.gpro.DBUserTasks.getDefaultInstance() : tasks_;
  }
  /**
   * <code>.PRO.DBUserTasks tasks = 3;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTasksOrBuilder getTasksOrBuilder() {
    return getTasks();
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
    if (groups_ != null) {
      output.writeMessage(2, getGroups());
    }
    if (tasks_ != null) {
      output.writeMessage(3, getTasks());
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
    if (groups_ != null) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(2, getGroups());
    }
    if (tasks_ != null) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(3, getTasks());
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
    if (!(obj instanceof com.cms.client.network.gpro.Task_MyTaskList_ack)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.Task_MyTaskList_ack other = (com.cms.client.network.gpro.Task_MyTaskList_ack) obj;

    if (hasUtoken() != other.hasUtoken()) return false;
    if (hasUtoken()) {
      if (!getUtoken()
          .equals(other.getUtoken())) return false;
    }
    if (hasGroups() != other.hasGroups()) return false;
    if (hasGroups()) {
      if (!getGroups()
          .equals(other.getGroups())) return false;
    }
    if (hasTasks() != other.hasTasks()) return false;
    if (hasTasks()) {
      if (!getTasks()
          .equals(other.getTasks())) return false;
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
    if (hasGroups()) {
      hash = (37 * hash) + GROUPS_FIELD_NUMBER;
      hash = (53 * hash) + getGroups().hashCode();
    }
    if (hasTasks()) {
      hash = (37 * hash) + TASKS_FIELD_NUMBER;
      hash = (53 * hash) + getTasks().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.Task_MyTaskList_ack parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.Task_MyTaskList_ack prototype) {
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
   * Protobuf type {@code PRO.Task_MyTaskList_ack}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.Task_MyTaskList_ack)
      com.cms.client.network.gpro.Task_MyTaskList_ackOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.TaskSystem.internal_static_PRO_Task_MyTaskList_ack_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.TaskSystem.internal_static_PRO_Task_MyTaskList_ack_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.Task_MyTaskList_ack.class, com.cms.client.network.gpro.Task_MyTaskList_ack.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.Task_MyTaskList_ack.newBuilder()
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
      if (groupsBuilder_ == null) {
        groups_ = null;
      } else {
        groups_ = null;
        groupsBuilder_ = null;
      }
      if (tasksBuilder_ == null) {
        tasks_ = null;
      } else {
        tasks_ = null;
        tasksBuilder_ = null;
      }
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.TaskSystem.internal_static_PRO_Task_MyTaskList_ack_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Task_MyTaskList_ack getDefaultInstanceForType() {
      return com.cms.client.network.gpro.Task_MyTaskList_ack.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Task_MyTaskList_ack build() {
      com.cms.client.network.gpro.Task_MyTaskList_ack result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.Task_MyTaskList_ack buildPartial() {
      com.cms.client.network.gpro.Task_MyTaskList_ack result = new com.cms.client.network.gpro.Task_MyTaskList_ack(this);
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
      if (groupsBuilder_ == null) {
        result.groups_ = groups_;
      } else {
        result.groups_ = groupsBuilder_.build();
      }
      if (tasksBuilder_ == null) {
        result.tasks_ = tasks_;
      } else {
        result.tasks_ = tasksBuilder_.build();
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
      if (other instanceof com.cms.client.network.gpro.Task_MyTaskList_ack) {
        return mergeFrom((com.cms.client.network.gpro.Task_MyTaskList_ack)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.Task_MyTaskList_ack other) {
      if (other == com.cms.client.network.gpro.Task_MyTaskList_ack.getDefaultInstance()) return this;
      if (other.hasUtoken()) {
        mergeUtoken(other.getUtoken());
      }
      if (other.hasGroups()) {
        mergeGroups(other.getGroups());
      }
      if (other.hasTasks()) {
        mergeTasks(other.getTasks());
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
      com.cms.client.network.gpro.Task_MyTaskList_ack parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.Task_MyTaskList_ack) e.getUnfinishedMessage();
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

    private com.cms.client.network.gpro.DBUserTaskGroups groups_;
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTaskGroups, com.cms.client.network.gpro.DBUserTaskGroups.Builder, com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder> groupsBuilder_;
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     * @return Whether the groups field is set.
     */
    public boolean hasGroups() {
      return groupsBuilder_ != null || groups_ != null;
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     * @return The groups.
     */
    public com.cms.client.network.gpro.DBUserTaskGroups getGroups() {
      if (groupsBuilder_ == null) {
        return groups_ == null ? com.cms.client.network.gpro.DBUserTaskGroups.getDefaultInstance() : groups_;
      } else {
        return groupsBuilder_.getMessage();
      }
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public Builder setGroups(com.cms.client.network.gpro.DBUserTaskGroups value) {
      if (groupsBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        groups_ = value;
        onChanged();
      } else {
        groupsBuilder_.setMessage(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public Builder setGroups(
        com.cms.client.network.gpro.DBUserTaskGroups.Builder builderForValue) {
      if (groupsBuilder_ == null) {
        groups_ = builderForValue.build();
        onChanged();
      } else {
        groupsBuilder_.setMessage(builderForValue.build());
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public Builder mergeGroups(com.cms.client.network.gpro.DBUserTaskGroups value) {
      if (groupsBuilder_ == null) {
        if (groups_ != null) {
          groups_ =
            com.cms.client.network.gpro.DBUserTaskGroups.newBuilder(groups_).mergeFrom(value).buildPartial();
        } else {
          groups_ = value;
        }
        onChanged();
      } else {
        groupsBuilder_.mergeFrom(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public Builder clearGroups() {
      if (groupsBuilder_ == null) {
        groups_ = null;
        onChanged();
      } else {
        groups_ = null;
        groupsBuilder_ = null;
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskGroups.Builder getGroupsBuilder() {
      
      onChanged();
      return getGroupsFieldBuilder().getBuilder();
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder getGroupsOrBuilder() {
      if (groupsBuilder_ != null) {
        return groupsBuilder_.getMessageOrBuilder();
      } else {
        return groups_ == null ?
            com.cms.client.network.gpro.DBUserTaskGroups.getDefaultInstance() : groups_;
      }
    }
    /**
     * <code>.PRO.DBUserTaskGroups groups = 2;</code>
     */
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTaskGroups, com.cms.client.network.gpro.DBUserTaskGroups.Builder, com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder> 
        getGroupsFieldBuilder() {
      if (groupsBuilder_ == null) {
        groupsBuilder_ = new com.google.protobuf.SingleFieldBuilderV3<
            com.cms.client.network.gpro.DBUserTaskGroups, com.cms.client.network.gpro.DBUserTaskGroups.Builder, com.cms.client.network.gpro.DBUserTaskGroupsOrBuilder>(
                getGroups(),
                getParentForChildren(),
                isClean());
        groups_ = null;
      }
      return groupsBuilder_;
    }

    private com.cms.client.network.gpro.DBUserTasks tasks_;
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTasks, com.cms.client.network.gpro.DBUserTasks.Builder, com.cms.client.network.gpro.DBUserTasksOrBuilder> tasksBuilder_;
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     * @return Whether the tasks field is set.
     */
    public boolean hasTasks() {
      return tasksBuilder_ != null || tasks_ != null;
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     * @return The tasks.
     */
    public com.cms.client.network.gpro.DBUserTasks getTasks() {
      if (tasksBuilder_ == null) {
        return tasks_ == null ? com.cms.client.network.gpro.DBUserTasks.getDefaultInstance() : tasks_;
      } else {
        return tasksBuilder_.getMessage();
      }
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public Builder setTasks(com.cms.client.network.gpro.DBUserTasks value) {
      if (tasksBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        tasks_ = value;
        onChanged();
      } else {
        tasksBuilder_.setMessage(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public Builder setTasks(
        com.cms.client.network.gpro.DBUserTasks.Builder builderForValue) {
      if (tasksBuilder_ == null) {
        tasks_ = builderForValue.build();
        onChanged();
      } else {
        tasksBuilder_.setMessage(builderForValue.build());
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public Builder mergeTasks(com.cms.client.network.gpro.DBUserTasks value) {
      if (tasksBuilder_ == null) {
        if (tasks_ != null) {
          tasks_ =
            com.cms.client.network.gpro.DBUserTasks.newBuilder(tasks_).mergeFrom(value).buildPartial();
        } else {
          tasks_ = value;
        }
        onChanged();
      } else {
        tasksBuilder_.mergeFrom(value);
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public Builder clearTasks() {
      if (tasksBuilder_ == null) {
        tasks_ = null;
        onChanged();
      } else {
        tasks_ = null;
        tasksBuilder_ = null;
      }

      return this;
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public com.cms.client.network.gpro.DBUserTasks.Builder getTasksBuilder() {
      
      onChanged();
      return getTasksFieldBuilder().getBuilder();
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    public com.cms.client.network.gpro.DBUserTasksOrBuilder getTasksOrBuilder() {
      if (tasksBuilder_ != null) {
        return tasksBuilder_.getMessageOrBuilder();
      } else {
        return tasks_ == null ?
            com.cms.client.network.gpro.DBUserTasks.getDefaultInstance() : tasks_;
      }
    }
    /**
     * <code>.PRO.DBUserTasks tasks = 3;</code>
     */
    private com.google.protobuf.SingleFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTasks, com.cms.client.network.gpro.DBUserTasks.Builder, com.cms.client.network.gpro.DBUserTasksOrBuilder> 
        getTasksFieldBuilder() {
      if (tasksBuilder_ == null) {
        tasksBuilder_ = new com.google.protobuf.SingleFieldBuilderV3<
            com.cms.client.network.gpro.DBUserTasks, com.cms.client.network.gpro.DBUserTasks.Builder, com.cms.client.network.gpro.DBUserTasksOrBuilder>(
                getTasks(),
                getParentForChildren(),
                isClean());
        tasks_ = null;
      }
      return tasksBuilder_;
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


    // @@protoc_insertion_point(builder_scope:PRO.Task_MyTaskList_ack)
  }

  // @@protoc_insertion_point(class_scope:PRO.Task_MyTaskList_ack)
  private static final com.cms.client.network.gpro.Task_MyTaskList_ack DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.Task_MyTaskList_ack();
  }

  public static com.cms.client.network.gpro.Task_MyTaskList_ack getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Task_MyTaskList_ack>
      PARSER = new com.google.protobuf.AbstractParser<Task_MyTaskList_ack>() {
    @java.lang.Override
    public Task_MyTaskList_ack parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Task_MyTaskList_ack(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Task_MyTaskList_ack> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Task_MyTaskList_ack> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.Task_MyTaskList_ack getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}
