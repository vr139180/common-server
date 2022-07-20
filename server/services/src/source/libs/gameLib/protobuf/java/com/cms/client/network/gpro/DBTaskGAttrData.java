// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db_internal.proto

package com.cms.client.network.gpro;

/**
 * Protobuf type {@code PRO.DBTaskGAttrData}
 */
public final class DBTaskGAttrData extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:PRO.DBTaskGAttrData)
    DBTaskGAttrDataOrBuilder {
private static final long serialVersionUID = 0L;
  // Use DBTaskGAttrData.newBuilder() to construct.
  private DBTaskGAttrData(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private DBTaskGAttrData() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new DBTaskGAttrData();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private DBTaskGAttrData(
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
              datas_ = com.google.protobuf.MapField.newMapField(
                  DatasDefaultEntryHolder.defaultEntry);
              mutable_bitField0_ |= 0x00000001;
            }
            com.google.protobuf.MapEntry<java.lang.Integer, java.lang.Integer>
            datas__ = input.readMessage(
                DatasDefaultEntryHolder.defaultEntry.getParserForType(), extensionRegistry);
            datas_.getMutableMap().put(
                datas__.getKey(), datas__.getValue());
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
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_descriptor;
  }

  @SuppressWarnings({"rawtypes"})
  @java.lang.Override
  protected com.google.protobuf.MapField internalGetMapField(
      int number) {
    switch (number) {
      case 1:
        return internalGetDatas();
      default:
        throw new RuntimeException(
            "Invalid map field number: " + number);
    }
  }
  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.cms.client.network.gpro.DBTaskGAttrData.class, com.cms.client.network.gpro.DBTaskGAttrData.Builder.class);
  }

  public static final int DATAS_FIELD_NUMBER = 1;
  private static final class DatasDefaultEntryHolder {
    static final com.google.protobuf.MapEntry<
        java.lang.Integer, java.lang.Integer> defaultEntry =
            com.google.protobuf.MapEntry
            .<java.lang.Integer, java.lang.Integer>newDefaultInstance(
                com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_DatasEntry_descriptor, 
                com.google.protobuf.WireFormat.FieldType.INT32,
                0,
                com.google.protobuf.WireFormat.FieldType.INT32,
                0);
  }
  private com.google.protobuf.MapField<
      java.lang.Integer, java.lang.Integer> datas_;
  private com.google.protobuf.MapField<java.lang.Integer, java.lang.Integer>
  internalGetDatas() {
    if (datas_ == null) {
      return com.google.protobuf.MapField.emptyMapField(
          DatasDefaultEntryHolder.defaultEntry);
    }
    return datas_;
  }

  public int getDatasCount() {
    return internalGetDatas().getMap().size();
  }
  /**
   * <code>map&lt;int32, int32&gt; datas = 1;</code>
   */

  @java.lang.Override
  public boolean containsDatas(
      int key) {
    
    return internalGetDatas().getMap().containsKey(key);
  }
  /**
   * Use {@link #getDatasMap()} instead.
   */
  @java.lang.Override
  @java.lang.Deprecated
  public java.util.Map<java.lang.Integer, java.lang.Integer> getDatas() {
    return getDatasMap();
  }
  /**
   * <code>map&lt;int32, int32&gt; datas = 1;</code>
   */
  @java.lang.Override

  public java.util.Map<java.lang.Integer, java.lang.Integer> getDatasMap() {
    return internalGetDatas().getMap();
  }
  /**
   * <code>map&lt;int32, int32&gt; datas = 1;</code>
   */
  @java.lang.Override

  public int getDatasOrDefault(
      int key,
      int defaultValue) {
    
    java.util.Map<java.lang.Integer, java.lang.Integer> map =
        internalGetDatas().getMap();
    return map.containsKey(key) ? map.get(key) : defaultValue;
  }
  /**
   * <code>map&lt;int32, int32&gt; datas = 1;</code>
   */
  @java.lang.Override

  public int getDatasOrThrow(
      int key) {
    
    java.util.Map<java.lang.Integer, java.lang.Integer> map =
        internalGetDatas().getMap();
    if (!map.containsKey(key)) {
      throw new java.lang.IllegalArgumentException();
    }
    return map.get(key);
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
    com.google.protobuf.GeneratedMessageV3
      .serializeIntegerMapTo(
        output,
        internalGetDatas(),
        DatasDefaultEntryHolder.defaultEntry,
        1);
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    for (java.util.Map.Entry<java.lang.Integer, java.lang.Integer> entry
         : internalGetDatas().getMap().entrySet()) {
      com.google.protobuf.MapEntry<java.lang.Integer, java.lang.Integer>
      datas__ = DatasDefaultEntryHolder.defaultEntry.newBuilderForType()
          .setKey(entry.getKey())
          .setValue(entry.getValue())
          .build();
      size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(1, datas__);
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
    if (!(obj instanceof com.cms.client.network.gpro.DBTaskGAttrData)) {
      return super.equals(obj);
    }
    com.cms.client.network.gpro.DBTaskGAttrData other = (com.cms.client.network.gpro.DBTaskGAttrData) obj;

    if (!internalGetDatas().equals(
        other.internalGetDatas())) return false;
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
    if (!internalGetDatas().getMap().isEmpty()) {
      hash = (37 * hash) + DATAS_FIELD_NUMBER;
      hash = (53 * hash) + internalGetDatas().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.cms.client.network.gpro.DBTaskGAttrData parseFrom(
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
  public static Builder newBuilder(com.cms.client.network.gpro.DBTaskGAttrData prototype) {
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
   * Protobuf type {@code PRO.DBTaskGAttrData}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:PRO.DBTaskGAttrData)
      com.cms.client.network.gpro.DBTaskGAttrDataOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_descriptor;
    }

    @SuppressWarnings({"rawtypes"})
    protected com.google.protobuf.MapField internalGetMapField(
        int number) {
      switch (number) {
        case 1:
          return internalGetDatas();
        default:
          throw new RuntimeException(
              "Invalid map field number: " + number);
      }
    }
    @SuppressWarnings({"rawtypes"})
    protected com.google.protobuf.MapField internalGetMutableMapField(
        int number) {
      switch (number) {
        case 1:
          return internalGetMutableDatas();
        default:
          throw new RuntimeException(
              "Invalid map field number: " + number);
      }
    }
    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.cms.client.network.gpro.DBTaskGAttrData.class, com.cms.client.network.gpro.DBTaskGAttrData.Builder.class);
    }

    // Construct using com.cms.client.network.gpro.DBTaskGAttrData.newBuilder()
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
      internalGetMutableDatas().clear();
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.cms.client.network.gpro.DbInternal.internal_static_PRO_DBTaskGAttrData_descriptor;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBTaskGAttrData getDefaultInstanceForType() {
      return com.cms.client.network.gpro.DBTaskGAttrData.getDefaultInstance();
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBTaskGAttrData build() {
      com.cms.client.network.gpro.DBTaskGAttrData result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.cms.client.network.gpro.DBTaskGAttrData buildPartial() {
      com.cms.client.network.gpro.DBTaskGAttrData result = new com.cms.client.network.gpro.DBTaskGAttrData(this);
      int from_bitField0_ = bitField0_;
      result.datas_ = internalGetDatas();
      result.datas_.makeImmutable();
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
      if (other instanceof com.cms.client.network.gpro.DBTaskGAttrData) {
        return mergeFrom((com.cms.client.network.gpro.DBTaskGAttrData)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.cms.client.network.gpro.DBTaskGAttrData other) {
      if (other == com.cms.client.network.gpro.DBTaskGAttrData.getDefaultInstance()) return this;
      internalGetMutableDatas().mergeFrom(
          other.internalGetDatas());
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
      com.cms.client.network.gpro.DBTaskGAttrData parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.cms.client.network.gpro.DBTaskGAttrData) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private com.google.protobuf.MapField<
        java.lang.Integer, java.lang.Integer> datas_;
    private com.google.protobuf.MapField<java.lang.Integer, java.lang.Integer>
    internalGetDatas() {
      if (datas_ == null) {
        return com.google.protobuf.MapField.emptyMapField(
            DatasDefaultEntryHolder.defaultEntry);
      }
      return datas_;
    }
    private com.google.protobuf.MapField<java.lang.Integer, java.lang.Integer>
    internalGetMutableDatas() {
      onChanged();;
      if (datas_ == null) {
        datas_ = com.google.protobuf.MapField.newMapField(
            DatasDefaultEntryHolder.defaultEntry);
      }
      if (!datas_.isMutable()) {
        datas_ = datas_.copy();
      }
      return datas_;
    }

    public int getDatasCount() {
      return internalGetDatas().getMap().size();
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */

    @java.lang.Override
    public boolean containsDatas(
        int key) {
      
      return internalGetDatas().getMap().containsKey(key);
    }
    /**
     * Use {@link #getDatasMap()} instead.
     */
    @java.lang.Override
    @java.lang.Deprecated
    public java.util.Map<java.lang.Integer, java.lang.Integer> getDatas() {
      return getDatasMap();
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */
    @java.lang.Override

    public java.util.Map<java.lang.Integer, java.lang.Integer> getDatasMap() {
      return internalGetDatas().getMap();
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */
    @java.lang.Override

    public int getDatasOrDefault(
        int key,
        int defaultValue) {
      
      java.util.Map<java.lang.Integer, java.lang.Integer> map =
          internalGetDatas().getMap();
      return map.containsKey(key) ? map.get(key) : defaultValue;
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */
    @java.lang.Override

    public int getDatasOrThrow(
        int key) {
      
      java.util.Map<java.lang.Integer, java.lang.Integer> map =
          internalGetDatas().getMap();
      if (!map.containsKey(key)) {
        throw new java.lang.IllegalArgumentException();
      }
      return map.get(key);
    }

    public Builder clearDatas() {
      internalGetMutableDatas().getMutableMap()
          .clear();
      return this;
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */

    public Builder removeDatas(
        int key) {
      
      internalGetMutableDatas().getMutableMap()
          .remove(key);
      return this;
    }
    /**
     * Use alternate mutation accessors instead.
     */
    @java.lang.Deprecated
    public java.util.Map<java.lang.Integer, java.lang.Integer>
    getMutableDatas() {
      return internalGetMutableDatas().getMutableMap();
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */
    public Builder putDatas(
        int key,
        int value) {
      
      
      internalGetMutableDatas().getMutableMap()
          .put(key, value);
      return this;
    }
    /**
     * <code>map&lt;int32, int32&gt; datas = 1;</code>
     */

    public Builder putAllDatas(
        java.util.Map<java.lang.Integer, java.lang.Integer> values) {
      internalGetMutableDatas().getMutableMap()
          .putAll(values);
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


    // @@protoc_insertion_point(builder_scope:PRO.DBTaskGAttrData)
  }

  // @@protoc_insertion_point(class_scope:PRO.DBTaskGAttrData)
  private static final com.cms.client.network.gpro.DBTaskGAttrData DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.cms.client.network.gpro.DBTaskGAttrData();
  }

  public static com.cms.client.network.gpro.DBTaskGAttrData getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<DBTaskGAttrData>
      PARSER = new com.google.protobuf.AbstractParser<DBTaskGAttrData>() {
    @java.lang.Override
    public DBTaskGAttrData parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new DBTaskGAttrData(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<DBTaskGAttrData> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<DBTaskGAttrData> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.cms.client.network.gpro.DBTaskGAttrData getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

