// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid.proto

package com.cms.client.network.gpro;

/**
 * Protobuf enum {@code PRO.TASK_PROTYPE}
 */
public enum TASK_PROTYPE
    implements com.google.protobuf.ProtocolMessageEnum {
  /**
   * <code>UNIVERSAL_2750 = 0;</code>
   */
  UNIVERSAL_2750(0),
  /**
   * <code>TASK_MSG_BEGIN = 2750;</code>
   */
  TASK_MSG_BEGIN(2750),
  /**
   * <code>TASK_WAITLIST_REQ = 2751;</code>
   */
  TASK_WAITLIST_REQ(2751),
  /**
   * <code>TASK_WAITLIST_ACK = 2752;</code>
   */
  TASK_WAITLIST_ACK(2752),
  /**
   * <code>TASK_MYTASKLIST_REQ = 2753;</code>
   */
  TASK_MYTASKLIST_REQ(2753),
  /**
   * <code>TASK_MYTASKLIST_ACK = 2754;</code>
   */
  TASK_MYTASKLIST_ACK(2754),
  /**
   * <code>TASK_GETTASK_REQ = 2755;</code>
   */
  TASK_GETTASK_REQ(2755),
  /**
   * <code>TASK_GETTASK_ACK = 2756;</code>
   */
  TASK_GETTASK_ACK(2756),
  /**
   * <code>TASK_SUBMITTASK_REQ = 2757;</code>
   */
  TASK_SUBMITTASK_REQ(2757),
  /**
   * <code>TASK_SUBMITTASK_ACK = 2758;</code>
   */
  TASK_SUBMITTASK_ACK(2758),
  /**
   * <code>TASK_OBTAINREWARD_NTF = 2760;</code>
   */
  TASK_OBTAINREWARD_NTF(2760),
  /**
   * <code>TASK_GIVEUPTASK_REQ = 2761;</code>
   */
  TASK_GIVEUPTASK_REQ(2761),
  /**
   * <code>TASK_GIVEUPTASK_ACK = 2762;</code>
   */
  TASK_GIVEUPTASK_ACK(2762),
  /**
   * <code>TASK_MSG_END = 2950;</code>
   */
  TASK_MSG_END(2950),
  /**
   * <code>TASK_MSGALL_END = 2999;</code>
   */
  TASK_MSGALL_END(2999),
  UNRECOGNIZED(-1),
  ;

  /**
   * <code>UNIVERSAL_2750 = 0;</code>
   */
  public static final int UNIVERSAL_2750_VALUE = 0;
  /**
   * <code>TASK_MSG_BEGIN = 2750;</code>
   */
  public static final int TASK_MSG_BEGIN_VALUE = 2750;
  /**
   * <code>TASK_WAITLIST_REQ = 2751;</code>
   */
  public static final int TASK_WAITLIST_REQ_VALUE = 2751;
  /**
   * <code>TASK_WAITLIST_ACK = 2752;</code>
   */
  public static final int TASK_WAITLIST_ACK_VALUE = 2752;
  /**
   * <code>TASK_MYTASKLIST_REQ = 2753;</code>
   */
  public static final int TASK_MYTASKLIST_REQ_VALUE = 2753;
  /**
   * <code>TASK_MYTASKLIST_ACK = 2754;</code>
   */
  public static final int TASK_MYTASKLIST_ACK_VALUE = 2754;
  /**
   * <code>TASK_GETTASK_REQ = 2755;</code>
   */
  public static final int TASK_GETTASK_REQ_VALUE = 2755;
  /**
   * <code>TASK_GETTASK_ACK = 2756;</code>
   */
  public static final int TASK_GETTASK_ACK_VALUE = 2756;
  /**
   * <code>TASK_SUBMITTASK_REQ = 2757;</code>
   */
  public static final int TASK_SUBMITTASK_REQ_VALUE = 2757;
  /**
   * <code>TASK_SUBMITTASK_ACK = 2758;</code>
   */
  public static final int TASK_SUBMITTASK_ACK_VALUE = 2758;
  /**
   * <code>TASK_OBTAINREWARD_NTF = 2760;</code>
   */
  public static final int TASK_OBTAINREWARD_NTF_VALUE = 2760;
  /**
   * <code>TASK_GIVEUPTASK_REQ = 2761;</code>
   */
  public static final int TASK_GIVEUPTASK_REQ_VALUE = 2761;
  /**
   * <code>TASK_GIVEUPTASK_ACK = 2762;</code>
   */
  public static final int TASK_GIVEUPTASK_ACK_VALUE = 2762;
  /**
   * <code>TASK_MSG_END = 2950;</code>
   */
  public static final int TASK_MSG_END_VALUE = 2950;
  /**
   * <code>TASK_MSGALL_END = 2999;</code>
   */
  public static final int TASK_MSGALL_END_VALUE = 2999;


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
  public static TASK_PROTYPE valueOf(int value) {
    return forNumber(value);
  }

  /**
   * @param value The numeric wire value of the corresponding enum entry.
   * @return The enum associated with the given numeric wire value.
   */
  public static TASK_PROTYPE forNumber(int value) {
    switch (value) {
      case 0: return UNIVERSAL_2750;
      case 2750: return TASK_MSG_BEGIN;
      case 2751: return TASK_WAITLIST_REQ;
      case 2752: return TASK_WAITLIST_ACK;
      case 2753: return TASK_MYTASKLIST_REQ;
      case 2754: return TASK_MYTASKLIST_ACK;
      case 2755: return TASK_GETTASK_REQ;
      case 2756: return TASK_GETTASK_ACK;
      case 2757: return TASK_SUBMITTASK_REQ;
      case 2758: return TASK_SUBMITTASK_ACK;
      case 2760: return TASK_OBTAINREWARD_NTF;
      case 2761: return TASK_GIVEUPTASK_REQ;
      case 2762: return TASK_GIVEUPTASK_ACK;
      case 2950: return TASK_MSG_END;
      case 2999: return TASK_MSGALL_END;
      default: return null;
    }
  }

  public static com.google.protobuf.Internal.EnumLiteMap<TASK_PROTYPE>
      internalGetValueMap() {
    return internalValueMap;
  }
  private static final com.google.protobuf.Internal.EnumLiteMap<
      TASK_PROTYPE> internalValueMap =
        new com.google.protobuf.Internal.EnumLiteMap<TASK_PROTYPE>() {
          public TASK_PROTYPE findValueByNumber(int number) {
            return TASK_PROTYPE.forNumber(number);
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
    return com.cms.client.network.gpro.ProtoIid.getDescriptor().getEnumTypes().get(7);
  }

  private static final TASK_PROTYPE[] VALUES = values();

  public static TASK_PROTYPE valueOf(
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

  private TASK_PROTYPE(int value) {
    this.value = value;
  }

  // @@protoc_insertion_point(enum_scope:PRO.TASK_PROTYPE)
}
