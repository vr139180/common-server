// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db_internal.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.DBUserTasks}
 */
public final class DBUserTasks extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.DBUserTasks)
    DBUserTasksOrBuilder {
private static final long serialVersionUID = 0L;
  // Use DBUserTasks.newBuilder() to construct.
  private DBUserTasks(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private DBUserTasks() {
    items_ = java.util.Collections.emptyList();
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new DBUserTasks();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private DBUserTasks(
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
            if (!((mutable_bitField0_ & 0x00000001) != 0)) {
              items_ = new java.util.ArrayList<com.cms.client.network.gpro.DBUserTaskItem>();
              mutable_bitField0_ |= 0x00000001;
            }
            items_.add(
                input.readMessage(com.cms.client.network.gpro.DBUserTaskItem.parser(), extensionRegistry));
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
      if (((mutable_bitField0_ & 0x00000001) != 0)) {
        items_ = java.util.Collections.unmodifiableList(items_);
      }
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTasks_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTasks_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.DBUserTasks.class, com.cms.client.network.gpro.DBUserTasks.Builder.class);
  }

  public static final int ITEMS_FIELD_NUMBER = 1;
  private java.util.List<com.cms.client.network.gpro.DBUserTaskItem> items_;
  /**
   * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
   */
  @java.lang.Override
  public java.util.List<com.cms.client.network.gpro.DBUserTaskItem> getItemsList() {
    return items_;
  }
  /**
   * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
   */
  @java.lang.Override
  public java.util.List<? extends com.cms.client.network.gpro.DBUserTaskItemOrBuilder> 
      getItemsOrBuilderList() {
    return items_;
  }
  /**
   * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
   */
  @java.lang.Override
  public int getItemsCount() {
    return items_.size();
  }
  /**
   * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTaskItem getItems(int index) {
    return items_.get(index);
  }
  /**
   * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
   */
  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTaskItemOrBuilder getItemsOrBuilder(
      int index) {
    return items_.get(index);
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
    for (int i = 0; i < items_.size(); i++) {
      output.writeMessage(1, items_.get(i));
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    for (int i = 0; i < items_.size(); i++) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(1, items_.get(i));
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
    if (!(obj instanceof com.cms.client.network.gpro.DBUserTasks)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.DBUserTasks other = (com.cms.client.network.gpro.DBUserTasks) obj;

    if (!getItemsList()
        .equals(other.getItemsList())) return false;
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
    if (getItemsCount() > 0) {
      hash = (37 * hash) + ITEMS_FIELD_NUMBER;
      hash = (53 * hash) + getItemsList().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBUserTasks parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.DBUserTasks prototype) {
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
   * Protobuf type {@code PRO.DBUserTasks}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.DBUserTasks)
      com.cms.client.network.gpro.DBUserTasksOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTasks_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTasks_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.DBUserTasks.class, com.cms.client.network.gpro.DBUserTasks.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.DBUserTasks.newBuilder()
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
        getItemsFieldBuilder();
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      if (itemsBuilder_ == null) {
        items_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
      } else {
        itemsBuilder_.clear();
      }
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBUserTasks_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTasks getDefaultInstanceForType() {
      return com.cms.client.network.gpro.DBUserTasks.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTasks build() {
      com.cms.client.network.gpro.DBUserTasks result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBUserTasks buildPartial() {
      com.cms.client.network.gpro.DBUserTasks result = new com.cms.client.network.gpro.DBUserTasks(this);
      int from_bitField0_ = bitField0_;
      if (itemsBuilder_ == null) {
        if (((bitField0_ & 0x00000001) != 0)) {
          items_ = java.util.Collections.unmodifiableList(items_);
          bitField0_ = (bitField0_ & ~0x00000001);
        }
        result.items_ = items_;
      } else {
        result.items_ = itemsBuilder_.build();
      }
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
      if (other instanceof com.cms.client.network.gpro.DBUserTasks) {
        return mergeFrom((com.cms.client.network.gpro.DBUserTasks)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.DBUserTasks other) {
      if (other == com.cms.client.network.gpro.DBUserTasks.getDefaultInstance()) return this;
      if (itemsBuilder_ == null) {
        if (!other.items_.isEmpty()) {
          if (items_.isEmpty()) {
            items_ = other.items_;
            bitField0_ = (bitField0_ & ~0x00000001);
          } else {
            ensureItemsIsMutable();
            items_.addAll(other.items_);
          }
          onChanged();
        }
      } else {
        if (!other.items_.isEmpty()) {
          if (itemsBuilder_.isEmpty()) {
            itemsBuilder_.dispose();
            itemsBuilder_ = null;
            items_ = other.items_;
            bitField0_ = (bitField0_ & ~0x00000001);
            itemsBuilder_ = 
              com.google.protobuf.GeneratedMessageV3.alwaysUseFieldBuilders ?
                 getItemsFieldBuilder() : null;
          } else {
            itemsBuilder_.addAllMessages(other.items_);
          }
        }
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
      com.cms.client.network.gpro.DBUserTasks parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.DBUserTasks) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private java.util.List<com.cms.client.network.gpro.DBUserTaskItem> items_ =
      java.util.Collections.emptyList();
    private void ensureItemsIsMutable() {
      if (!((bitField0_ & 0x00000001) != 0)) {
        items_ = new java.util.ArrayList<com.cms.client.network.gpro.DBUserTaskItem>(items_);
        bitField0_ |= 0x00000001;
       }
    }

    private com.google.protobuf.RepeatedFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTaskItem, com.cms.client.network.gpro.DBUserTaskItem.Builder, com.cms.client.network.gpro.DBUserTaskItemOrBuilder> itemsBuilder_;

    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public java.util.List<com.cms.client.network.gpro.DBUserTaskItem> getItemsList() {
      if (itemsBuilder_ == null) {
        return java.util.Collections.unmodifiableList(items_);
      } else {
        return itemsBuilder_.getMessageList();
      }
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public int getItemsCount() {
      if (itemsBuilder_ == null) {
        return items_.size();
      } else {
        return itemsBuilder_.getCount();
      }
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskItem getItems(int index) {
      if (itemsBuilder_ == null) {
        return items_.get(index);
      } else {
        return itemsBuilder_.getMessage(index);
      }
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder setItems(
        int index, com.cms.client.network.gpro.DBUserTaskItem value) {
      if (itemsBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureItemsIsMutable();
        items_.set(index, value);
        onChanged();
      } else {
        itemsBuilder_.setMessage(index, value);
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder setItems(
        int index, com.cms.client.network.gpro.DBUserTaskItem.Builder builderForValue) {
      if (itemsBuilder_ == null) {
        ensureItemsIsMutable();
        items_.set(index, builderForValue.build());
        onChanged();
      } else {
        itemsBuilder_.setMessage(index, builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder addItems(com.cms.client.network.gpro.DBUserTaskItem value) {
      if (itemsBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureItemsIsMutable();
        items_.add(value);
        onChanged();
      } else {
        itemsBuilder_.addMessage(value);
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder addItems(
        int index, com.cms.client.network.gpro.DBUserTaskItem value) {
      if (itemsBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureItemsIsMutable();
        items_.add(index, value);
        onChanged();
      } else {
        itemsBuilder_.addMessage(index, value);
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder addItems(
        com.cms.client.network.gpro.DBUserTaskItem.Builder builderForValue) {
      if (itemsBuilder_ == null) {
        ensureItemsIsMutable();
        items_.add(builderForValue.build());
        onChanged();
      } else {
        itemsBuilder_.addMessage(builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder addItems(
        int index, com.cms.client.network.gpro.DBUserTaskItem.Builder builderForValue) {
      if (itemsBuilder_ == null) {
        ensureItemsIsMutable();
        items_.add(index, builderForValue.build());
        onChanged();
      } else {
        itemsBuilder_.addMessage(index, builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder addAllItems(
        java.lang.Iterable<? extends com.cms.client.network.gpro.DBUserTaskItem> values) {
      if (itemsBuilder_ == null) {
        ensureItemsIsMutable();
        com.google.protobuf.AbstractMessageLite.Builder.addAll(
            values, items_);
        onChanged();
      } else {
        itemsBuilder_.addAllMessages(values);
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder clearItems() {
      if (itemsBuilder_ == null) {
        items_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        onChanged();
      } else {
        itemsBuilder_.clear();
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public Builder removeItems(int index) {
      if (itemsBuilder_ == null) {
        ensureItemsIsMutable();
        items_.remove(index);
        onChanged();
      } else {
        itemsBuilder_.remove(index);
      }
      return this;
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskItem.Builder getItemsBuilder(
        int index) {
      return getItemsFieldBuilder().getBuilder(index);
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskItemOrBuilder getItemsOrBuilder(
        int index) {
      if (itemsBuilder_ == null) {
        return items_.get(index);  } else {
        return itemsBuilder_.getMessageOrBuilder(index);
      }
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public java.util.List<? extends com.cms.client.network.gpro.DBUserTaskItemOrBuilder> 
         getItemsOrBuilderList() {
      if (itemsBuilder_ != null) {
        return itemsBuilder_.getMessageOrBuilderList();
      } else {
        return java.util.Collections.unmodifiableList(items_);
      }
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskItem.Builder addItemsBuilder() {
      return getItemsFieldBuilder().addBuilder(
          com.cms.client.network.gpro.DBUserTaskItem.getDefaultInstance());
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public com.cms.client.network.gpro.DBUserTaskItem.Builder addItemsBuilder(
        int index) {
      return getItemsFieldBuilder().addBuilder(
          index, com.cms.client.network.gpro.DBUserTaskItem.getDefaultInstance());
    }
    /**
     * <code>repeated .PRO.DBUserTaskItem items = 1;</code>
     */
    public java.util.List<com.cms.client.network.gpro.DBUserTaskItem.Builder> 
         getItemsBuilderList() {
      return getItemsFieldBuilder().getBuilderList();
    }
    private com.google.protobuf.RepeatedFieldBuilderV3<
        com.cms.client.network.gpro.DBUserTaskItem, com.cms.client.network.gpro.DBUserTaskItem.Builder, com.cms.client.network.gpro.DBUserTaskItemOrBuilder> 
        getItemsFieldBuilder() {
      if (itemsBuilder_ == null) {
        itemsBuilder_ = new com.google.protobuf.RepeatedFieldBuilderV3<
            com.cms.client.network.gpro.DBUserTaskItem, com.cms.client.network.gpro.DBUserTaskItem.Builder, com.cms.client.network.gpro.DBUserTaskItemOrBuilder>(
                items_,
                ((bitField0_ & 0x00000001) != 0),
                getParentForChildren(),
                isClean());
        items_ = null;
      }
      return itemsBuilder_;
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


    // @@protoc_insertion_point(builder_scope:PRO.DBUserTasks)
  }

  // @@protoc_insertion_point(class_scope:PRO.DBUserTasks)
  private static final com.cms.client.network.gpro.DBUserTasks DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.DBUserTasks();
  }

  public static com.cms.client.network.gpro.DBUserTasks getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<DBUserTasks>
      PARSER = new com.google.protobuf.AbstractParser<DBUserTasks>() {
    @java.lang.Override
    public DBUserTasks parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new DBUserTasks(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<DBUserTasks> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<DBUserTasks> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.DBUserTasks getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}
