// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid_internal.proto

#include "proto_iid_internal.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace PRO {
}  // namespace PRO
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_proto_5fiid_5finternal_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_proto_5fiid_5finternal_2eproto = nullptr;
const uint32_t TableStruct_proto_5fiid_5finternal_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_proto_5fiid_5finternal_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\030proto_iid_internal.proto\022\003PRO*\326\004\n\013ERK_"
  "PROTYPE\022\017\n\013UNIVERSAL_1\020\000\022\030\n\024ERK_EUREKARE"
  "GIST_REQ\020\001\022\030\n\024ERK_EUREKAREGIST_ACK\020\002\022\026\n\022"
  "ERK_EUREKABIND_REQ\020\003\022\026\n\022ERK_EUREKABIND_A"
  "CK\020\004\022\030\n\024ERK_EUREKAUPDATE_NTF\020\005\022\030\n\024ERK_MA"
  "STERCHANGE_NTF\020\006\022\031\n\025ERK_SERVICEREGIST_RE"
  "Q\020\n\022\031\n\025ERK_SERVICEREGIST_ACK\020\013\022\027\n\023ERK_SE"
  "RVICESYNC_NTF\020\014\022\027\n\023ERK_SERVICEBIND_REQ\020\r"
  "\022\027\n\023ERK_SERVICEBIND_ACK\020\016\022\034\n\030ERK_SERVICE"
  "SUBSCRIBE_REQ\020\017\022\034\n\030ERK_SERVICESUBSCRIBE_"
  "NTF\020\020\022\033\n\027ERK_ROUTERSUBSCRIBE_REQ\020\021\022\033\n\027ER"
  "K_ROUTERSUBSCRIBE_NTF\020\022\022\030\n\024ERK_ROUTERONL"
  "INE_REQ\020\023\022\024\n\020SVR_LIVETICK_NTF\0201\022\036\n\032SVR_S"
  "ERVICEBINDSERVICE_REQ\0202\022\036\n\032SVR_SERVICEBI"
  "NDSERVICE_ACK\0203\022\030\n\024SVR_ROUTERONLINE_NTF\020"
  "4\022\035\n\031GSFR_GAMEREGIONREGIST_NTF\020<*\365\002\n\021ROB"
  "OTTEST_PROTYPE\022\021\n\rUNIVERSAL_300\020\000\022 \n\033ROB"
  "OTOTEST_ROBOT_CONFIG_REQ\020\255\002\022 \n\033ROBOTOTES"
  "T_ROBOT_CONFIG_ACK\020\256\002\022\037\n\032ROBOTOTEST_ROBO"
  "T_START_ACK\020\257\002\022\036\n\031ROBOTOTEST_ROBOT_STOP_"
  "ACK\020\260\002\022#\n\036ROBOTOTEST_UPLOADLOGSROBOT_NTF"
  "\020\261\002\022\"\n\035ROBOTOTEST_CLEARLOGSROBOT_NTF\020\262\002\022"
  "#\n\036ROBOTOTEST_USERPREFIXROBOT_NTF\020\263\002\022\037\n\032"
  "ROBOTOTEST_STARTRECORD_REG\020\264\002\022\035\n\030ROBOTOT"
  "EST_ENDRECORD_REG\020\265\002\022\032\n\025ROBOTOTEST_RECOR"
  "D_REG\020\266\002BT\n\033com.cms.client.network.gproP"
  "\001Z\025gamelib/protobuf/gpro\252\002\033com.cms.clien"
  "t.network.gprob\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_proto_5fiid_5finternal_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fiid_5finternal_2eproto = {
  false, false, 1102, descriptor_table_protodef_proto_5fiid_5finternal_2eproto, "proto_iid_internal.proto", 
  &descriptor_table_proto_5fiid_5finternal_2eproto_once, nullptr, 0, 0,
  schemas, file_default_instances, TableStruct_proto_5fiid_5finternal_2eproto::offsets,
  nullptr, file_level_enum_descriptors_proto_5fiid_5finternal_2eproto, file_level_service_descriptors_proto_5fiid_5finternal_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_proto_5fiid_5finternal_2eproto_getter() {
  return &descriptor_table_proto_5fiid_5finternal_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_proto_5fiid_5finternal_2eproto(&descriptor_table_proto_5fiid_5finternal_2eproto);
namespace PRO {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ERK_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_5finternal_2eproto);
  return file_level_enum_descriptors_proto_5fiid_5finternal_2eproto[0];
}
bool ERK_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 49:
    case 50:
    case 51:
    case 52:
    case 60:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ROBOTTEST_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_5finternal_2eproto);
  return file_level_enum_descriptors_proto_5fiid_5finternal_2eproto[1];
}
bool ROBOTTEST_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 301:
    case 302:
    case 303:
    case 304:
    case 305:
    case 306:
    case 307:
    case 308:
    case 309:
    case 310:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace PRO
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
