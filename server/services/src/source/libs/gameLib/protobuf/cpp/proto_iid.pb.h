// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_proto_5fiid_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_proto_5fiid_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fiid_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace PRO {

enum USER_PROTYPE : int {
  UNIVERSAL_1000 = 0,
  ERK_USER_NUM = 1000,
  USER_LOGIN_REQ = 1001,
  USER_LOGIN_ACK = 1002,
  USER_PROXYLOGIN_REQ = 1003,
  USER_PROXYLOGIN_ACK = 1004,
  USER_LOGOUT_NTF = 1005,
  USER_ROLELIST_ACK = 1007,
  USER_ROLESELECT_REQ = 1008,
  USER_ROLESELECT_ACK = 1009,
  USER_ROLECREATE_REQ = 1010,
  USER_ROLECREATE_ACK = 1011,
  USER_ROLEDETAILA_NTF = 1012,
  USER_ROLEDETAILB_NTF = 1013,
  USER_ROLEDETAILEND_NTF = 1014,
  USER_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  USER_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool USER_PROTYPE_IsValid(int value);
constexpr USER_PROTYPE USER_PROTYPE_MIN = UNIVERSAL_1000;
constexpr USER_PROTYPE USER_PROTYPE_MAX = USER_ROLEDETAILEND_NTF;
constexpr int USER_PROTYPE_ARRAYSIZE = USER_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* USER_PROTYPE_descriptor();
template<typename T>
inline const std::string& USER_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, USER_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function USER_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    USER_PROTYPE_descriptor(), enum_t_value);
}
inline bool USER_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, USER_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<USER_PROTYPE>(
    USER_PROTYPE_descriptor(), name, value);
}
enum BUILD_PROTYPE : int {
  UNIVERSAL_1250 = 0,
  BUILD_ADDITEM_REQ = 1251,
  BUILD_ADDITEM_ACK = 1252,
  BUILD_DELITEM_REQ = 1253,
  BUILD_DELITEM_ACK = 1254,
  BUILD_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  BUILD_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool BUILD_PROTYPE_IsValid(int value);
constexpr BUILD_PROTYPE BUILD_PROTYPE_MIN = UNIVERSAL_1250;
constexpr BUILD_PROTYPE BUILD_PROTYPE_MAX = BUILD_DELITEM_ACK;
constexpr int BUILD_PROTYPE_ARRAYSIZE = BUILD_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* BUILD_PROTYPE_descriptor();
template<typename T>
inline const std::string& BUILD_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, BUILD_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function BUILD_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    BUILD_PROTYPE_descriptor(), enum_t_value);
}
inline bool BUILD_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, BUILD_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<BUILD_PROTYPE>(
    BUILD_PROTYPE_descriptor(), name, value);
}
enum PET_PROTYPE : int {
  UNIVERSAL_1500 = 0,
  PET_ADOPTONE_REQ = 1501,
  PET_ADOPTONE_ACK = 1502,
  PET_RELEASEONE_REQ = 1503,
  PET_RELEASEONE_ACK = 1504,
  PET_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  PET_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool PET_PROTYPE_IsValid(int value);
constexpr PET_PROTYPE PET_PROTYPE_MIN = UNIVERSAL_1500;
constexpr PET_PROTYPE PET_PROTYPE_MAX = PET_RELEASEONE_ACK;
constexpr int PET_PROTYPE_ARRAYSIZE = PET_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PET_PROTYPE_descriptor();
template<typename T>
inline const std::string& PET_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PET_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PET_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PET_PROTYPE_descriptor(), enum_t_value);
}
inline bool PET_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PET_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PET_PROTYPE>(
    PET_PROTYPE_descriptor(), name, value);
}
enum CHAT_PROTYPE : int {
  UNIVERSAL_1750 = 0,
  CHAT_MSG_BEGIN = 1750,
  CHAT_USERCHANNELS_ACTIVE = 1751,
  CHAT_CUSTOMCHANNELID_REQ = 1752,
  CHAT_CUSTOMCHANNELID_ACK = 1753,
  CHAT_USERMSG_SAY = 1754,
  CHAT_CHATMSG_NTF = 1755,
  CHAT_GLOBALMSG_NTF = 1949,
  CHAT_MSG_END = 1950,
  CHAT_MSGALL_END = 1999,
  CHAT_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  CHAT_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool CHAT_PROTYPE_IsValid(int value);
constexpr CHAT_PROTYPE CHAT_PROTYPE_MIN = UNIVERSAL_1750;
constexpr CHAT_PROTYPE CHAT_PROTYPE_MAX = CHAT_MSGALL_END;
constexpr int CHAT_PROTYPE_ARRAYSIZE = CHAT_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CHAT_PROTYPE_descriptor();
template<typename T>
inline const std::string& CHAT_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, CHAT_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function CHAT_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    CHAT_PROTYPE_descriptor(), enum_t_value);
}
inline bool CHAT_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, CHAT_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<CHAT_PROTYPE>(
    CHAT_PROTYPE_descriptor(), name, value);
}
enum MAIL_PROTYPE : int {
  UNIVERSAL_2000 = 0,
  MAIL_MSG_BEGIN = 2000,
  MAIL_NEWMAIL_NTF = 2001,
  MAIL_MAILGET_REQ = 2002,
  MAIL_MAILGET_ACK = 2003,
  MAIL_USERONLINE_ACTIVE = 2004,
  MAIL_READMAIL_REQ = 2005,
  MAIL_READMAIL_ACK = 2006,
  MAIL_DELETEMAIL_REQ = 2007,
  MAIL_DELETEMAIL_ACK = 2008,
  MAIL_UNBINDATTACHS_REQ = 2009,
  MAIL_UNBINDATTACHS_ACK = 2010,
  MAIL_SYSTEMMAIL_REQ = 2201,
  MAIL_SYSTEMMAIL_ACK = 2202,
  MAIL_SYSTEMMAIL_NTF = 2203,
  MAIL_NEWMAIL_REQ = 2204,
  MAIL_NEWMAIL_ACK = 2205,
  MAIL_MSG_END = 2240,
  MAIL_MSGALL_END = 2249,
  MAIL_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  MAIL_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool MAIL_PROTYPE_IsValid(int value);
constexpr MAIL_PROTYPE MAIL_PROTYPE_MIN = UNIVERSAL_2000;
constexpr MAIL_PROTYPE MAIL_PROTYPE_MAX = MAIL_MSGALL_END;
constexpr int MAIL_PROTYPE_ARRAYSIZE = MAIL_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MAIL_PROTYPE_descriptor();
template<typename T>
inline const std::string& MAIL_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MAIL_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MAIL_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MAIL_PROTYPE_descriptor(), enum_t_value);
}
inline bool MAIL_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MAIL_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MAIL_PROTYPE>(
    MAIL_PROTYPE_descriptor(), name, value);
}
enum FRIEND_PROTYPE : int {
  UNIVERSAL_2250 = 0,
  FRIEND_MSG_BEGIN = 2250,
  FRD_FRIENDLIST_REQ = 2251,
  FRD_FRIENDLIST_ACK = 2252,
  FRD_FRIENDINVITE_REQ = 2253,
  FRD_FRIENDINVITE_ACK = 2254,
  FRD_INVITECONFIRM_REQ = 2255,
  FRD_INVITECONFIRM_ACK = 2256,
  FRD_FRIENDDELETE_REQ = 2257,
  FRD_FRIENDDELETE_ACK = 2258,
  FRIEND_MSG_END = 2450,
  FRD_FRIENDCHANGEOTHER_NTF = 2451,
  FRIEND_MSGALL_END = 2499,
  FRIEND_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  FRIEND_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool FRIEND_PROTYPE_IsValid(int value);
constexpr FRIEND_PROTYPE FRIEND_PROTYPE_MIN = UNIVERSAL_2250;
constexpr FRIEND_PROTYPE FRIEND_PROTYPE_MAX = FRIEND_MSGALL_END;
constexpr int FRIEND_PROTYPE_ARRAYSIZE = FRIEND_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FRIEND_PROTYPE_descriptor();
template<typename T>
inline const std::string& FRIEND_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, FRIEND_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function FRIEND_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    FRIEND_PROTYPE_descriptor(), enum_t_value);
}
inline bool FRIEND_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, FRIEND_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<FRIEND_PROTYPE>(
    FRIEND_PROTYPE_descriptor(), name, value);
}
enum ITEMS_PROTYPE : int {
  UNIVERSAL_2500 = 0,
  ITEM_USERBINDITEMS_REQ = 2501,
  ITEM_USERBINDITEMS_ACK = 2502,
  ITEMS_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  ITEMS_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool ITEMS_PROTYPE_IsValid(int value);
constexpr ITEMS_PROTYPE ITEMS_PROTYPE_MIN = UNIVERSAL_2500;
constexpr ITEMS_PROTYPE ITEMS_PROTYPE_MAX = ITEM_USERBINDITEMS_ACK;
constexpr int ITEMS_PROTYPE_ARRAYSIZE = ITEMS_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ITEMS_PROTYPE_descriptor();
template<typename T>
inline const std::string& ITEMS_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ITEMS_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ITEMS_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ITEMS_PROTYPE_descriptor(), enum_t_value);
}
inline bool ITEMS_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ITEMS_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ITEMS_PROTYPE>(
    ITEMS_PROTYPE_descriptor(), name, value);
}
enum TASK_PROTYPE : int {
  UNIVERSAL_2750 = 0,
  TASK_MSG_BEGIN = 2750,
  TASK_WAITLIST_REQ = 2751,
  TASK_WAITLIST_ACK = 2752,
  TASK_MYTASKLIST_REQ = 2753,
  TASK_MYTASKLIST_ACK = 2754,
  TASK_GETTASK_REQ = 2755,
  TASK_GETTASK_ACK = 2756,
  TASK_SUBMITTASK_REQ = 2757,
  TASK_SUBMITTASK_ACK = 2758,
  TASK_OBTAINREWARD_NTF = 2760,
  TASK_GIVEUPTASK_REQ = 2761,
  TASK_GIVEUPTASK_ACK = 2762,
  TASK_MSG_END = 2950,
  TASK_MSGALL_END = 2999,
  TASK_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  TASK_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool TASK_PROTYPE_IsValid(int value);
constexpr TASK_PROTYPE TASK_PROTYPE_MIN = UNIVERSAL_2750;
constexpr TASK_PROTYPE TASK_PROTYPE_MAX = TASK_MSGALL_END;
constexpr int TASK_PROTYPE_ARRAYSIZE = TASK_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* TASK_PROTYPE_descriptor();
template<typename T>
inline const std::string& TASK_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, TASK_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function TASK_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    TASK_PROTYPE_descriptor(), enum_t_value);
}
inline bool TASK_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, TASK_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<TASK_PROTYPE>(
    TASK_PROTYPE_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace PRO

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::PRO::USER_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::USER_PROTYPE>() {
  return ::PRO::USER_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::BUILD_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::BUILD_PROTYPE>() {
  return ::PRO::BUILD_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::PET_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::PET_PROTYPE>() {
  return ::PRO::PET_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::CHAT_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::CHAT_PROTYPE>() {
  return ::PRO::CHAT_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::MAIL_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::MAIL_PROTYPE>() {
  return ::PRO::MAIL_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::FRIEND_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::FRIEND_PROTYPE>() {
  return ::PRO::FRIEND_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::ITEMS_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::ITEMS_PROTYPE>() {
  return ::PRO::ITEMS_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::TASK_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::TASK_PROTYPE>() {
  return ::PRO::TASK_PROTYPE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_2eproto
