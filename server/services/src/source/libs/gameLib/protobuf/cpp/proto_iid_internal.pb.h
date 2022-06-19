// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid_internal.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_5finternal_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_5finternal_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_proto_5fiid_5finternal_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_proto_5fiid_5finternal_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fiid_5finternal_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace PRO {

enum ERK_PROTYPE : int {
  UNIVERSAL_1 = 0,
  ERK_EUREKABIND_REQ = 1,
  ERK_EUREKABIND_ACK = 2,
  ERK_EUREKAUPDATE_NTF = 3,
  ERK_EUREKA_SYNC = 4,
  ERK_SERVICEREGIST_REQ = 10,
  ERK_SERVICEREGIST_ACK = 11,
  ERK_SERVICEREGIST_CONFIRM = 12,
  ERK_SERVICEBIND_REQ = 13,
  ERK_SERVICEBIND_ACK = 14,
  ERK_SERVICESUBSCRIBE_REQ = 15,
  ERK_SERVICESUBSCRIBE_ACK = 16,
  ERK_SERVICESHUTDOWN_NTF = 17,
  SVR_LIVETICK_NTF = 49,
  SVR_SERVICEBINDSERVICE_REQ = 50,
  SVR_SERVICEBINDSERVICE_ACK = 51,
  SVR_GATEBINDHOME_REQ = 52,
  SVR_GATEBINDHOME_ACK = 53,
  SVR_GATEBINDHOME_CONFIRM = 54,
  RES_USERSLOT_REQ = 100,
  RES_USERSLOT_ACK = 101,
  RES_PROXYUSERSLOT_REQ = 102,
  RES_PROXYUSERSLOT_ACK = 103,
  RES_SYNCGATESLOT_NTF = 104,
  ERK_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  ERK_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool ERK_PROTYPE_IsValid(int value);
constexpr ERK_PROTYPE ERK_PROTYPE_MIN = UNIVERSAL_1;
constexpr ERK_PROTYPE ERK_PROTYPE_MAX = RES_SYNCGATESLOT_NTF;
constexpr int ERK_PROTYPE_ARRAYSIZE = ERK_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ERK_PROTYPE_descriptor();
template<typename T>
inline const std::string& ERK_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ERK_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ERK_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ERK_PROTYPE_descriptor(), enum_t_value);
}
inline bool ERK_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ERK_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ERK_PROTYPE>(
    ERK_PROTYPE_descriptor(), name, value);
}
enum GATEHOME_PROTYPE : int {
  UNIVERSAL_200 = 0,
  GHS_USERINIT_REQ = 201,
  GHS_USERLOGOUT_NTF = 202,
  GHS_ROLEDETAIL_ASK = 203,
  GATEHOME_PROTYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  GATEHOME_PROTYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool GATEHOME_PROTYPE_IsValid(int value);
constexpr GATEHOME_PROTYPE GATEHOME_PROTYPE_MIN = UNIVERSAL_200;
constexpr GATEHOME_PROTYPE GATEHOME_PROTYPE_MAX = GHS_ROLEDETAIL_ASK;
constexpr int GATEHOME_PROTYPE_ARRAYSIZE = GATEHOME_PROTYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* GATEHOME_PROTYPE_descriptor();
template<typename T>
inline const std::string& GATEHOME_PROTYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, GATEHOME_PROTYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function GATEHOME_PROTYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    GATEHOME_PROTYPE_descriptor(), enum_t_value);
}
inline bool GATEHOME_PROTYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, GATEHOME_PROTYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<GATEHOME_PROTYPE>(
    GATEHOME_PROTYPE_descriptor(), name, value);
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

template <> struct is_proto_enum< ::PRO::ERK_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::ERK_PROTYPE>() {
  return ::PRO::ERK_PROTYPE_descriptor();
}
template <> struct is_proto_enum< ::PRO::GATEHOME_PROTYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PRO::GATEHOME_PROTYPE>() {
  return ::PRO::GATEHOME_PROTYPE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_proto_5fiid_5finternal_2eproto
