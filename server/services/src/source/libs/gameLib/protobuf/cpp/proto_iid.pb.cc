// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_iid.proto

#include "proto_iid.pb.h"

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
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_proto_5fiid_2eproto[8];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_proto_5fiid_2eproto = nullptr;
const uint32_t TableStruct_proto_5fiid_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_proto_5fiid_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017proto_iid.proto\022\003PRO*\374\002\n\014USER_PROTYPE\022"
  "\022\n\016UNIVERSAL_1000\020\000\022\021\n\014ERK_USER_NUM\020\350\007\022\023"
  "\n\016USER_LOGIN_REQ\020\351\007\022\023\n\016USER_LOGIN_ACK\020\352\007"
  "\022\030\n\023USER_PROXYLOGIN_REQ\020\353\007\022\030\n\023USER_PROXY"
  "LOGIN_ACK\020\354\007\022\024\n\017USER_LOGOUT_NTF\020\355\007\022\026\n\021US"
  "ER_ROLELIST_ACK\020\357\007\022\030\n\023USER_ROLESELECT_RE"
  "Q\020\360\007\022\030\n\023USER_ROLESELECT_ACK\020\361\007\022\030\n\023USER_R"
  "OLECREATE_REQ\020\362\007\022\030\n\023USER_ROLECREATE_ACK\020"
  "\363\007\022\031\n\024USER_ROLEDETAILA_NTF\020\364\007\022\031\n\024USER_RO"
  "LEDETAILB_NTF\020\365\007\022\033\n\026USER_ROLEDETAILEND_N"
  "TF\020\366\007*\203\001\n\rBUILD_PROTYPE\022\022\n\016UNIVERSAL_125"
  "0\020\000\022\026\n\021BUILD_ADDITEM_REQ\020\343\t\022\026\n\021BUILD_ADD"
  "ITEM_ACK\020\344\t\022\026\n\021BUILD_DELITEM_REQ\020\345\t\022\026\n\021B"
  "UILD_DELITEM_ACK\020\346\t*\201\001\n\013PET_PROTYPE\022\022\n\016U"
  "NIVERSAL_1500\020\000\022\025\n\020PET_ADOPTONE_REQ\020\335\013\022\025"
  "\n\020PET_ADOPTONE_ACK\020\336\013\022\027\n\022PET_RELEASEONE_"
  "REQ\020\337\013\022\027\n\022PET_RELEASEONE_ACK\020\340\013*\204\002\n\014CHAT"
  "_PROTYPE\022\022\n\016UNIVERSAL_1750\020\000\022\023\n\016CHAT_MSG"
  "_BEGIN\020\326\r\022\035\n\030CHAT_USERCHANNELS_ACTIVE\020\327\r"
  "\022\035\n\030CHAT_CUSTOMCHANNELID_REQ\020\330\r\022\035\n\030CHAT_"
  "CUSTOMCHANNELID_ACK\020\331\r\022\025\n\020CHAT_USERMSG_S"
  "AY\020\332\r\022\025\n\020CHAT_CHATMSG_NTF\020\333\r\022\027\n\022CHAT_GLO"
  "BALMSG_NTF\020\235\017\022\021\n\014CHAT_MSG_END\020\236\017\022\024\n\017CHAT"
  "_MSGALL_END\020\317\017*\334\003\n\014MAIL_PROTYPE\022\022\n\016UNIVE"
  "RSAL_2000\020\000\022\023\n\016MAIL_MSG_BEGIN\020\320\017\022\025\n\020MAIL"
  "_NEWMAIL_NTF\020\321\017\022\025\n\020MAIL_MAILGET_REQ\020\322\017\022\025"
  "\n\020MAIL_MAILGET_ACK\020\323\017\022\033\n\026MAIL_USERONLINE"
  "_ACTIVE\020\324\017\022\026\n\021MAIL_READMAIL_REQ\020\325\017\022\026\n\021MA"
  "IL_READMAIL_ACK\020\326\017\022\030\n\023MAIL_DELETEMAIL_RE"
  "Q\020\327\017\022\030\n\023MAIL_DELETEMAIL_ACK\020\330\017\022\033\n\026MAIL_U"
  "NBINDATTACHS_REQ\020\331\017\022\033\n\026MAIL_UNBINDATTACH"
  "S_ACK\020\332\017\022\030\n\023MAIL_SYSTEMMAIL_REQ\020\231\021\022\030\n\023MA"
  "IL_SYSTEMMAIL_ACK\020\232\021\022\030\n\023MAIL_SYSTEMMAIL_"
  "NTF\020\233\021\022\025\n\020MAIL_NEWMAIL_REQ\020\234\021\022\025\n\020MAIL_NE"
  "WMAIL_ACK\020\235\021\022\021\n\014MAIL_MSG_END\020\300\021\022\024\n\017MAIL_"
  "MSGALL_END\020\311\021*\336\002\n\016FRIEND_PROTYPE\022\022\n\016UNIV"
  "ERSAL_2250\020\000\022\025\n\020FRIEND_MSG_BEGIN\020\312\021\022\027\n\022F"
  "RD_FRIENDLIST_REQ\020\313\021\022\027\n\022FRD_FRIENDLIST_A"
  "CK\020\314\021\022\031\n\024FRD_FRIENDINVITE_REQ\020\315\021\022\031\n\024FRD_"
  "FRIENDINVITE_ACK\020\316\021\022\032\n\025FRD_INVITECONFIRM"
  "_REQ\020\317\021\022\032\n\025FRD_INVITECONFIRM_ACK\020\320\021\022\031\n\024F"
  "RD_FRIENDDELETE_REQ\020\321\021\022\031\n\024FRD_FRIENDDELE"
  "TE_ACK\020\322\021\022\023\n\016FRIEND_MSG_END\020\222\023\022\036\n\031FRD_FR"
  "IENDCHANGEOTHER_NTF\020\223\023\022\026\n\021FRIEND_MSGALL_"
  "END\020\303\023*]\n\rITEMS_PROTYPE\022\022\n\016UNIVERSAL_250"
  "0\020\000\022\033\n\026ITEM_USERBINDITEMS_REQ\020\305\023\022\033\n\026ITEM"
  "_USERBINDITEMS_ACK\020\306\023*\366\002\n\014TASK_PROTYPE\022\022"
  "\n\016UNIVERSAL_2750\020\000\022\023\n\016TASK_MSG_BEGIN\020\276\025\022"
  "\026\n\021TASK_WAITLIST_REQ\020\277\025\022\026\n\021TASK_WAITLIST"
  "_ACK\020\300\025\022\030\n\023TASK_MYTASKLIST_REQ\020\301\025\022\030\n\023TAS"
  "K_MYTASKLIST_ACK\020\302\025\022\025\n\020TASK_GETTASK_REQ\020"
  "\303\025\022\025\n\020TASK_GETTASK_ACK\020\304\025\022\030\n\023TASK_SUBMIT"
  "TASK_REQ\020\305\025\022\030\n\023TASK_SUBMITTASK_ACK\020\306\025\022\032\n"
  "\025TASK_OBTAINREWARD_NTF\020\310\025\022\030\n\023TASK_GIVEUP"
  "TASK_REQ\020\311\025\022\030\n\023TASK_GIVEUPTASK_ACK\020\312\025\022\021\n"
  "\014TASK_MSG_END\020\206\027\022\024\n\017TASK_MSGALL_END\020\267\027BT"
  "\n\033com.cms.client.network.gproP\001Z\025gamelib"
  "/protobuf/gpro\252\002\033com.cms.client.network."
  "gprob\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_proto_5fiid_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fiid_2eproto = {
  false, false, 2332, descriptor_table_protodef_proto_5fiid_2eproto, "proto_iid.proto", 
  &descriptor_table_proto_5fiid_2eproto_once, nullptr, 0, 0,
  schemas, file_default_instances, TableStruct_proto_5fiid_2eproto::offsets,
  nullptr, file_level_enum_descriptors_proto_5fiid_2eproto, file_level_service_descriptors_proto_5fiid_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_proto_5fiid_2eproto_getter() {
  return &descriptor_table_proto_5fiid_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_proto_5fiid_2eproto(&descriptor_table_proto_5fiid_2eproto);
namespace PRO {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* USER_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[0];
}
bool USER_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1000:
    case 1001:
    case 1002:
    case 1003:
    case 1004:
    case 1005:
    case 1007:
    case 1008:
    case 1009:
    case 1010:
    case 1011:
    case 1012:
    case 1013:
    case 1014:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* BUILD_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[1];
}
bool BUILD_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1251:
    case 1252:
    case 1253:
    case 1254:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PET_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[2];
}
bool PET_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1501:
    case 1502:
    case 1503:
    case 1504:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CHAT_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[3];
}
bool CHAT_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1750:
    case 1751:
    case 1752:
    case 1753:
    case 1754:
    case 1755:
    case 1949:
    case 1950:
    case 1999:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MAIL_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[4];
}
bool MAIL_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 2000:
    case 2001:
    case 2002:
    case 2003:
    case 2004:
    case 2005:
    case 2006:
    case 2007:
    case 2008:
    case 2009:
    case 2010:
    case 2201:
    case 2202:
    case 2203:
    case 2204:
    case 2205:
    case 2240:
    case 2249:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FRIEND_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[5];
}
bool FRIEND_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 2250:
    case 2251:
    case 2252:
    case 2253:
    case 2254:
    case 2255:
    case 2256:
    case 2257:
    case 2258:
    case 2450:
    case 2451:
    case 2499:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ITEMS_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[6];
}
bool ITEMS_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 2501:
    case 2502:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* TASK_PROTYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fiid_2eproto);
  return file_level_enum_descriptors_proto_5fiid_2eproto[7];
}
bool TASK_PROTYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 2750:
    case 2751:
    case 2752:
    case 2753:
    case 2754:
    case 2755:
    case 2756:
    case 2757:
    case 2758:
    case 2760:
    case 2761:
    case 2762:
    case 2950:
    case 2999:
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
