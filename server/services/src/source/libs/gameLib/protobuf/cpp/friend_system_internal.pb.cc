// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: friend_system_internal.proto

#include "friend_system_internal.pb.h"

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
constexpr Frd_FriendChangeOther_ntf::Frd_FriendChangeOther_ntf(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : friend__(nullptr)
  , notify_roleiid_(int64_t{0})
  , friendiid_(int64_t{0})
  , addordel_(false){}
struct Frd_FriendChangeOther_ntfDefaultTypeInternal {
  constexpr Frd_FriendChangeOther_ntfDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~Frd_FriendChangeOther_ntfDefaultTypeInternal() {}
  union {
    Frd_FriendChangeOther_ntf _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT Frd_FriendChangeOther_ntfDefaultTypeInternal _Frd_FriendChangeOther_ntf_default_instance_;
}  // namespace PRO
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_friend_5fsystem_5finternal_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_friend_5fsystem_5finternal_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_friend_5fsystem_5finternal_2eproto = nullptr;

const uint32_t TableStruct_friend_5fsystem_5finternal_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, notify_roleiid_),
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, addordel_),
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, friendiid_),
  PROTOBUF_FIELD_OFFSET(::PRO::Frd_FriendChangeOther_ntf, friend__),
  ~0u,
  ~0u,
  ~0u,
  0,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 10, -1, sizeof(::PRO::Frd_FriendChangeOther_ntf)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::PRO::_Frd_FriendChangeOther_ntf_default_instance_),
};

const char descriptor_table_protodef_friend_5fsystem_5finternal_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\034friend_system_internal.proto\022\003PRO\032\025glo"
  "bal_internal.proto\"\215\001\n\031Frd_FriendChangeO"
  "ther_ntf\022\026\n\016notify_roleiid\030\001 \001(\003\022\020\n\010addo"
  "rdel\030\002 \001(\010\022\021\n\tfriendiid\030\003 \001(\003\022(\n\006friend\030"
  "\004 \001(\0132\023.PRO.FriendRelationH\000\210\001\001B\t\n\007_frie"
  "ndBT\n\033com.cms.client.network.gproP\001Z\025gam"
  "elib/protobuf/gpro\252\002\033com.cms.client.netw"
  "ork.gprob\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_friend_5fsystem_5finternal_2eproto_deps[1] = {
  &::descriptor_table_global_5finternal_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_friend_5fsystem_5finternal_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_friend_5fsystem_5finternal_2eproto = {
  false, false, 296, descriptor_table_protodef_friend_5fsystem_5finternal_2eproto, "friend_system_internal.proto", 
  &descriptor_table_friend_5fsystem_5finternal_2eproto_once, descriptor_table_friend_5fsystem_5finternal_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_friend_5fsystem_5finternal_2eproto::offsets,
  file_level_metadata_friend_5fsystem_5finternal_2eproto, file_level_enum_descriptors_friend_5fsystem_5finternal_2eproto, file_level_service_descriptors_friend_5fsystem_5finternal_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_friend_5fsystem_5finternal_2eproto_getter() {
  return &descriptor_table_friend_5fsystem_5finternal_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_friend_5fsystem_5finternal_2eproto(&descriptor_table_friend_5fsystem_5finternal_2eproto);
namespace PRO {

// ===================================================================

class Frd_FriendChangeOther_ntf::_Internal {
 public:
  using HasBits = decltype(std::declval<Frd_FriendChangeOther_ntf>()._has_bits_);
  static const ::PRO::FriendRelation& friend_(const Frd_FriendChangeOther_ntf* msg);
  static void set_has_friend_(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

const ::PRO::FriendRelation&
Frd_FriendChangeOther_ntf::_Internal::friend_(const Frd_FriendChangeOther_ntf* msg) {
  return *msg->friend__;
}
void Frd_FriendChangeOther_ntf::clear_friend_() {
  if (friend__ != nullptr) friend__->Clear();
  _has_bits_[0] &= ~0x00000001u;
}
Frd_FriendChangeOther_ntf::Frd_FriendChangeOther_ntf(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:PRO.Frd_FriendChangeOther_ntf)
}
Frd_FriendChangeOther_ntf::Frd_FriendChangeOther_ntf(const Frd_FriendChangeOther_ntf& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_friend_()) {
    friend__ = new ::PRO::FriendRelation(*from.friend__);
  } else {
    friend__ = nullptr;
  }
  ::memcpy(&notify_roleiid_, &from.notify_roleiid_,
    static_cast<size_t>(reinterpret_cast<char*>(&addordel_) -
    reinterpret_cast<char*>(&notify_roleiid_)) + sizeof(addordel_));
  // @@protoc_insertion_point(copy_constructor:PRO.Frd_FriendChangeOther_ntf)
}

inline void Frd_FriendChangeOther_ntf::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&friend__) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&addordel_) -
    reinterpret_cast<char*>(&friend__)) + sizeof(addordel_));
}

Frd_FriendChangeOther_ntf::~Frd_FriendChangeOther_ntf() {
  // @@protoc_insertion_point(destructor:PRO.Frd_FriendChangeOther_ntf)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void Frd_FriendChangeOther_ntf::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete friend__;
}

void Frd_FriendChangeOther_ntf::ArenaDtor(void* object) {
  Frd_FriendChangeOther_ntf* _this = reinterpret_cast< Frd_FriendChangeOther_ntf* >(object);
  (void)_this;
}
void Frd_FriendChangeOther_ntf::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Frd_FriendChangeOther_ntf::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Frd_FriendChangeOther_ntf::Clear() {
// @@protoc_insertion_point(message_clear_start:PRO.Frd_FriendChangeOther_ntf)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    GOOGLE_DCHECK(friend__ != nullptr);
    friend__->Clear();
  }
  ::memset(&notify_roleiid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&addordel_) -
      reinterpret_cast<char*>(&notify_roleiid_)) + sizeof(addordel_));
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Frd_FriendChangeOther_ntf::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int64 notify_roleiid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          notify_roleiid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bool addordel = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          addordel_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 friendiid = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          friendiid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional .PRO.FriendRelation friend = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          ptr = ctx->ParseMessage(_internal_mutable_friend_(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Frd_FriendChangeOther_ntf::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:PRO.Frd_FriendChangeOther_ntf)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 notify_roleiid = 1;
  if (this->_internal_notify_roleiid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_notify_roleiid(), target);
  }

  // bool addordel = 2;
  if (this->_internal_addordel() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(2, this->_internal_addordel(), target);
  }

  // int64 friendiid = 3;
  if (this->_internal_friendiid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(3, this->_internal_friendiid(), target);
  }

  // optional .PRO.FriendRelation friend = 4;
  if (_internal_has_friend_()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        4, _Internal::friend_(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PRO.Frd_FriendChangeOther_ntf)
  return target;
}

size_t Frd_FriendChangeOther_ntf::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PRO.Frd_FriendChangeOther_ntf)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional .PRO.FriendRelation friend = 4;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *friend__);
  }

  // int64 notify_roleiid = 1;
  if (this->_internal_notify_roleiid() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64SizePlusOne(this->_internal_notify_roleiid());
  }

  // int64 friendiid = 3;
  if (this->_internal_friendiid() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64SizePlusOne(this->_internal_friendiid());
  }

  // bool addordel = 2;
  if (this->_internal_addordel() != 0) {
    total_size += 1 + 1;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Frd_FriendChangeOther_ntf::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Frd_FriendChangeOther_ntf::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Frd_FriendChangeOther_ntf::GetClassData() const { return &_class_data_; }

void Frd_FriendChangeOther_ntf::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<Frd_FriendChangeOther_ntf *>(to)->MergeFrom(
      static_cast<const Frd_FriendChangeOther_ntf &>(from));
}


void Frd_FriendChangeOther_ntf::MergeFrom(const Frd_FriendChangeOther_ntf& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:PRO.Frd_FriendChangeOther_ntf)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_friend_()) {
    _internal_mutable_friend_()->::PRO::FriendRelation::MergeFrom(from._internal_friend_());
  }
  if (from._internal_notify_roleiid() != 0) {
    _internal_set_notify_roleiid(from._internal_notify_roleiid());
  }
  if (from._internal_friendiid() != 0) {
    _internal_set_friendiid(from._internal_friendiid());
  }
  if (from._internal_addordel() != 0) {
    _internal_set_addordel(from._internal_addordel());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Frd_FriendChangeOther_ntf::CopyFrom(const Frd_FriendChangeOther_ntf& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PRO.Frd_FriendChangeOther_ntf)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Frd_FriendChangeOther_ntf::IsInitialized() const {
  return true;
}

void Frd_FriendChangeOther_ntf::InternalSwap(Frd_FriendChangeOther_ntf* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Frd_FriendChangeOther_ntf, addordel_)
      + sizeof(Frd_FriendChangeOther_ntf::addordel_)
      - PROTOBUF_FIELD_OFFSET(Frd_FriendChangeOther_ntf, friend__)>(
          reinterpret_cast<char*>(&friend__),
          reinterpret_cast<char*>(&other->friend__));
}

::PROTOBUF_NAMESPACE_ID::Metadata Frd_FriendChangeOther_ntf::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_friend_5fsystem_5finternal_2eproto_getter, &descriptor_table_friend_5fsystem_5finternal_2eproto_once,
      file_level_metadata_friend_5fsystem_5finternal_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace PRO
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PRO::Frd_FriendChangeOther_ntf* Arena::CreateMaybeMessage< ::PRO::Frd_FriendChangeOther_ntf >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PRO::Frd_FriendChangeOther_ntf >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
