// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid_internal.proto

package com.cms.client.network.gpro;

/**
 * <pre>
 * 对应的协议编号
 * </pre>
 *
 * Protobuf enum {@code PRO.ERK_PROTYPE}
 */
public enum ERK_PROTYPE
    implements com.google.protobuf.ProtocolMessageEnum {
  /**
   * <code>UNIVERSAL_1 = 0;</code>
   */
  UNIVERSAL_1(0),
  /**
   * <pre>
   *绑定请求
   * </pre>
   *
   * <code>ERK_EUREKABIND_REQ = 1;</code>
   */
  ERK_EUREKABIND_REQ(1),
  /**
   * <code>ERK_EUREKABIND_ACK = 2;</code>
   */
  ERK_EUREKABIND_ACK(2),
  /**
   * <code>ERK_EUREKAUPDATE_NTF = 3;</code>
   */
  ERK_EUREKAUPDATE_NTF(3),
  /**
   * <code>ERK_EUREKA_SYNC = 4;</code>
   */
  ERK_EUREKA_SYNC(4),
  /**
   * <code>ERK_SERVICEREGIST_REQ = 10;</code>
   */
  ERK_SERVICEREGIST_REQ(10),
  /**
   * <code>ERK_SERVICEREGIST_ACK = 11;</code>
   */
  ERK_SERVICEREGIST_ACK(11),
  /**
   * <code>ERK_SERVICEREGIST_CONFIRM = 12;</code>
   */
  ERK_SERVICEREGIST_CONFIRM(12),
  /**
   * <code>ERK_SERVICEBIND_REQ = 13;</code>
   */
  ERK_SERVICEBIND_REQ(13),
  /**
   * <code>ERK_SERVICEBIND_ACK = 14;</code>
   */
  ERK_SERVICEBIND_ACK(14),
  /**
   * <code>ERK_SERVICESUBSCRIBE_REQ = 15;</code>
   */
  ERK_SERVICESUBSCRIBE_REQ(15),
  /**
   * <code>ERK_SERVICESUBSCRIBE_ACK = 16;</code>
   */
  ERK_SERVICESUBSCRIBE_ACK(16),
  /**
   * <code>ERK_SERVICESHUTDOWN_NTF = 17;</code>
   */
  ERK_SERVICESHUTDOWN_NTF(17),
  /**
   * <code>SVR_LIVETICK_NTF = 49;</code>
   */
  SVR_LIVETICK_NTF(49),
  /**
   * <code>SVR_SERVICEBINDSERVICE_REQ = 50;</code>
   */
  SVR_SERVICEBINDSERVICE_REQ(50),
  /**
   * <code>SVR_SERVICEBINDSERVICE_ACK = 51;</code>
   */
  SVR_SERVICEBINDSERVICE_ACK(51),
  /**
   * <pre>
   *资源服务
   * </pre>
   *
   * <code>RES_SYNCGATESLOT_NTF = 100;</code>
   */
  RES_SYNCGATESLOT_NTF(100),
  UNRECOGNIZED(-1),
  ;

  /**
   * <code>UNIVERSAL_1 = 0;</code>
   */
  public static final int UNIVERSAL_1_VALUE = 0;
  /**
   * <pre>
   *绑定请求
   * </pre>
   *
   * <code>ERK_EUREKABIND_REQ = 1;</code>
   */
  public static final int ERK_EUREKABIND_REQ_VALUE = 1;
  /**
   * <code>ERK_EUREKABIND_ACK = 2;</code>
   */
  public static final int ERK_EUREKABIND_ACK_VALUE = 2;
  /**
   * <code>ERK_EUREKAUPDATE_NTF = 3;</code>
   */
  public static final int ERK_EUREKAUPDATE_NTF_VALUE = 3;
  /**
   * <code>ERK_EUREKA_SYNC = 4;</code>
   */
  public static final int ERK_EUREKA_SYNC_VALUE = 4;
  /**
   * <code>ERK_SERVICEREGIST_REQ = 10;</code>
   */
  public static final int ERK_SERVICEREGIST_REQ_VALUE = 10;
  /**
   * <code>ERK_SERVICEREGIST_ACK = 11;</code>
   */
  public static final int ERK_SERVICEREGIST_ACK_VALUE = 11;
  /**
   * <code>ERK_SERVICEREGIST_CONFIRM = 12;</code>
   */
  public static final int ERK_SERVICEREGIST_CONFIRM_VALUE = 12;
  /**
   * <code>ERK_SERVICEBIND_REQ = 13;</code>
   */
  public static final int ERK_SERVICEBIND_REQ_VALUE = 13;
  /**
   * <code>ERK_SERVICEBIND_ACK = 14;</code>
   */
  public static final int ERK_SERVICEBIND_ACK_VALUE = 14;
  /**
   * <code>ERK_SERVICESUBSCRIBE_REQ = 15;</code>
   */
  public static final int ERK_SERVICESUBSCRIBE_REQ_VALUE = 15;
  /**
   * <code>ERK_SERVICESUBSCRIBE_ACK = 16;</code>
   */
  public static final int ERK_SERVICESUBSCRIBE_ACK_VALUE = 16;
  /**
   * <code>ERK_SERVICESHUTDOWN_NTF = 17;</code>
   */
  public static final int ERK_SERVICESHUTDOWN_NTF_VALUE = 17;
  /**
   * <code>SVR_LIVETICK_NTF = 49;</code>
   */
  public static final int SVR_LIVETICK_NTF_VALUE = 49;
  /**
   * <code>SVR_SERVICEBINDSERVICE_REQ = 50;</code>
   */
  public static final int SVR_SERVICEBINDSERVICE_REQ_VALUE = 50;
  /**
   * <code>SVR_SERVICEBINDSERVICE_ACK = 51;</code>
   */
  public static final int SVR_SERVICEBINDSERVICE_ACK_VALUE = 51;
  /**
   * <pre>
   *资源服务
   * </pre>
   *
   * <code>RES_SYNCGATESLOT_NTF = 100;</code>
   */
  public static final int RES_SYNCGATESLOT_NTF_VALUE = 100;


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
  public static ERK_PROTYPE valueOf(int value) {
    return forNumber(value);
  }

  /**
   * @param value The numeric wire value of the corresponding enum entry.
   * @return The enum associated with the given numeric wire value.
   */
  public static ERK_PROTYPE forNumber(int value) {
    switch (value) {
      case 0: return UNIVERSAL_1;
      case 1: return ERK_EUREKABIND_REQ;
      case 2: return ERK_EUREKABIND_ACK;
      case 3: return ERK_EUREKAUPDATE_NTF;
      case 4: return ERK_EUREKA_SYNC;
      case 10: return ERK_SERVICEREGIST_REQ;
      case 11: return ERK_SERVICEREGIST_ACK;
      case 12: return ERK_SERVICEREGIST_CONFIRM;
      case 13: return ERK_SERVICEBIND_REQ;
      case 14: return ERK_SERVICEBIND_ACK;
      case 15: return ERK_SERVICESUBSCRIBE_REQ;
      case 16: return ERK_SERVICESUBSCRIBE_ACK;
      case 17: return ERK_SERVICESHUTDOWN_NTF;
      case 49: return SVR_LIVETICK_NTF;
      case 50: return SVR_SERVICEBINDSERVICE_REQ;
      case 51: return SVR_SERVICEBINDSERVICE_ACK;
      case 100: return RES_SYNCGATESLOT_NTF;
      default: return null;
    }
  }

  public static com.google.protobuf.Internal.EnumLiteMap<ERK_PROTYPE>
      internalGetValueMap() {
    return internalValueMap;
  }
  private static final com.google.protobuf.Internal.EnumLiteMap<
      ERK_PROTYPE> internalValueMap =
        new com.google.protobuf.Internal.EnumLiteMap<ERK_PROTYPE>() {
          public ERK_PROTYPE findValueByNumber(int number) {
            return ERK_PROTYPE.forNumber(number);
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
    return com.cms.client.network.gpro.ProtoIidInternal.getDescriptor().getEnumTypes().get(0);
  }

  private static final ERK_PROTYPE[] VALUES = values();

  public static ERK_PROTYPE valueOf(
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

  private ERK_PROTYPE(int value) {
    this.value = value;
  }

  // @@protoc_insertion_point(enum_scope:PRO.ERK_PROTYPE)
}

