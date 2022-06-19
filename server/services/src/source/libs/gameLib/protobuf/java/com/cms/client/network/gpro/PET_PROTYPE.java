// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid.proto

package com.cms.client.network.gpro;

/**
 * Protobuf enum {@code PRO.PET_PROTYPE}
 */
public enum PET_PROTYPE
    implements com.google.protobuf.ProtocolMessageEnum {
  /**
   * <code>UNIVERSAL_4000 = 0;</code>
   */
  UNIVERSAL_4000(0),
  /**
   * <code>PET_ADOPTONE_REQ = 4001;</code>
   */
  PET_ADOPTONE_REQ(4001),
  /**
   * <code>PET_ADOPTONE_ACK = 4002;</code>
   */
  PET_ADOPTONE_ACK(4002),
  /**
   * <code>PET_RELEASEONE_REQ = 4003;</code>
   */
  PET_RELEASEONE_REQ(4003),
  /**
   * <code>PET_RELEASEONE_ACK = 4004;</code>
   */
  PET_RELEASEONE_ACK(4004),
  UNRECOGNIZED(-1),
  ;

  /**
   * <code>UNIVERSAL_4000 = 0;</code>
   */
  public static final int UNIVERSAL_4000_VALUE = 0;
  /**
   * <code>PET_ADOPTONE_REQ = 4001;</code>
   */
  public static final int PET_ADOPTONE_REQ_VALUE = 4001;
  /**
   * <code>PET_ADOPTONE_ACK = 4002;</code>
   */
  public static final int PET_ADOPTONE_ACK_VALUE = 4002;
  /**
   * <code>PET_RELEASEONE_REQ = 4003;</code>
   */
  public static final int PET_RELEASEONE_REQ_VALUE = 4003;
  /**
   * <code>PET_RELEASEONE_ACK = 4004;</code>
   */
  public static final int PET_RELEASEONE_ACK_VALUE = 4004;


  public final int getNumber() {
    if (this == UNRECOGNIZED) {
      throw new java.lang.IllegalArgumentException(
          "Can't get the number of an unknown enum value.");
    }
    return value;
  }

  /**
   * @param value The numeric wire value of the corresponding enum entry.
   * @return The enum associated with the given numeric wire value.
   * @deprecated Use {@link #forNumber(int)} instead.
   */
  @java.lang.Deprecated
  public static PET_PROTYPE valueOf(int value) {
    return forNumber(value);
  }

  /**
   * @param value The numeric wire value of the corresponding enum entry.
   * @return The enum associated with the given numeric wire value.
   */
  public static PET_PROTYPE forNumber(int value) {
    switch (value) {
      case 0: return UNIVERSAL_4000;
      case 4001: return PET_ADOPTONE_REQ;
      case 4002: return PET_ADOPTONE_ACK;
      case 4003: return PET_RELEASEONE_REQ;
      case 4004: return PET_RELEASEONE_ACK;
      default: return null;
    }
  }

  public static com.google.protobuf.Internal.EnumLiteMap<PET_PROTYPE>
      internalGetValueMap() {
    return internalValueMap;
  }
  private static final com.google.protobuf.Internal.EnumLiteMap<
      PET_PROTYPE> internalValueMap =
        new com.google.protobuf.Internal.EnumLiteMap<PET_PROTYPE>() {
          public PET_PROTYPE findValueByNumber(int number) {
            return PET_PROTYPE.forNumber(number);
          }
        };

  public final com.google.protobuf.Descriptors.EnumValueDescriptor
      getValueDescriptor() {
    if (this == UNRECOGNIZED) {
      throw new java.lang.IllegalStateException(
          "Can't get the descriptor of an unrecognized enum value.");
    }
    return getDescriptor().getValues().get(ordinal());
  }
  public final com.google.protobuf.Descriptors.EnumDescriptor
      getDescriptorForType() {
    return getDescriptor();
  }
  public static final com.google.protobuf.Descriptors.EnumDescriptor
      getDescriptor() {
    return com.cms.client.network.gpro.ProtoIid.getDescriptor().getEnumTypes().get(2);
  }

  private static final PET_PROTYPE[] VALUES = values();

  public static PET_PROTYPE valueOf(
      com.google.protobuf.Descriptors.EnumValueDescriptor desc) {
    if (desc.getType() != getDescriptor()) {
      throw new java.lang.IllegalArgumentException(
        "EnumValueDescriptor is not for this type.");
    }
    if (desc.getIndex() == -1) {
      return UNRECOGNIZED;
    }
    return VALUES[desc.getIndex()];
  }

  private final int value;

  private PET_PROTYPE(int value) {
    this.value = value;
  }

  // @@protoc_insertion_point(enum_scope:PRO.PET_PROTYPE)
}

