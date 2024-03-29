// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: friend_system_internal.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_friend_5fsystem_5finternal_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_friend_5fsystem_5finternal_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "global_internal.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_friend_5fsystem_5finternal_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_friend_5fsystem_5finternal_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_friend_5fsystem_5finternal_2eproto;
namespace PRO {
class Frd_FriendChangeOther_ntf;
struct Frd_FriendChangeOther_ntfDefaultTypeInternal;
extern Frd_FriendChangeOther_ntfDefaultTypeInternal _Frd_FriendChangeOther_ntf_default_instance_;
}  // namespace PRO
PROTOBUF_NAMESPACE_OPEN
template<> ::PRO::Frd_FriendChangeOther_ntf* Arena::CreateMaybeMessage<::PRO::Frd_FriendChangeOther_ntf>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace PRO {

// ===================================================================

class Frd_FriendChangeOther_ntf final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PRO.Frd_FriendChangeOther_ntf) */ {
 public:
  inline Frd_FriendChangeOther_ntf() : Frd_FriendChangeOther_ntf(nullptr) {}
  ~Frd_FriendChangeOther_ntf() override;
  explicit constexpr Frd_FriendChangeOther_ntf(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Frd_FriendChangeOther_ntf(const Frd_FriendChangeOther_ntf& from);
  Frd_FriendChangeOther_ntf(Frd_FriendChangeOther_ntf&& from) noexcept
    : Frd_FriendChangeOther_ntf() {
    *this = ::std::move(from);
  }

  inline Frd_FriendChangeOther_ntf& operator=(const Frd_FriendChangeOther_ntf& from) {
    CopyFrom(from);
    return *this;
  }
  inline Frd_FriendChangeOther_ntf& operator=(Frd_FriendChangeOther_ntf&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Frd_FriendChangeOther_ntf& default_instance() {
    return *internal_default_instance();
  }
  static inline const Frd_FriendChangeOther_ntf* internal_default_instance() {
    return reinterpret_cast<const Frd_FriendChangeOther_ntf*>(
               &_Frd_FriendChangeOther_ntf_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Frd_FriendChangeOther_ntf& a, Frd_FriendChangeOther_ntf& b) {
    a.Swap(&b);
  }
  inline void Swap(Frd_FriendChangeOther_ntf* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Frd_FriendChangeOther_ntf* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Frd_FriendChangeOther_ntf* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Frd_FriendChangeOther_ntf>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Frd_FriendChangeOther_ntf& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Frd_FriendChangeOther_ntf& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Frd_FriendChangeOther_ntf* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PRO.Frd_FriendChangeOther_ntf";
  }
  protected:
  explicit Frd_FriendChangeOther_ntf(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFriendFieldNumber = 4,
    kNotifyRoleiidFieldNumber = 1,
    kFriendiidFieldNumber = 3,
    kAddordelFieldNumber = 2,
  };
  // optional .PRO.FriendRelation friend = 4;
  bool has_friend_() const;
  private:
  bool _internal_has_friend_() const;
  public:
  void clear_friend_();
  const ::PRO::FriendRelation& friend_() const;
  PROTOBUF_NODISCARD ::PRO::FriendRelation* release_friend_();
  ::PRO::FriendRelation* mutable_friend_();
  void set_allocated_friend_(::PRO::FriendRelation* friend_);
  private:
  const ::PRO::FriendRelation& _internal_friend_() const;
  ::PRO::FriendRelation* _internal_mutable_friend_();
  public:
  void unsafe_arena_set_allocated_friend_(
      ::PRO::FriendRelation* friend_);
  ::PRO::FriendRelation* unsafe_arena_release_friend_();

  // int64 notify_roleiid = 1;
  void clear_notify_roleiid();
  int64_t notify_roleiid() const;
  void set_notify_roleiid(int64_t value);
  private:
  int64_t _internal_notify_roleiid() const;
  void _internal_set_notify_roleiid(int64_t value);
  public:

  // int64 friendiid = 3;
  void clear_friendiid();
  int64_t friendiid() const;
  void set_friendiid(int64_t value);
  private:
  int64_t _internal_friendiid() const;
  void _internal_set_friendiid(int64_t value);
  public:

  // bool addordel = 2;
  void clear_addordel();
  bool addordel() const;
  void set_addordel(bool value);
  private:
  bool _internal_addordel() const;
  void _internal_set_addordel(bool value);
  public:

  // @@protoc_insertion_point(class_scope:PRO.Frd_FriendChangeOther_ntf)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PRO::FriendRelation* friend__;
  int64_t notify_roleiid_;
  int64_t friendiid_;
  bool addordel_;
  friend struct ::TableStruct_friend_5fsystem_5finternal_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Frd_FriendChangeOther_ntf

// int64 notify_roleiid = 1;
inline void Frd_FriendChangeOther_ntf::clear_notify_roleiid() {
  notify_roleiid_ = int64_t{0};
}
inline int64_t Frd_FriendChangeOther_ntf::_internal_notify_roleiid() const {
  return notify_roleiid_;
}
inline int64_t Frd_FriendChangeOther_ntf::notify_roleiid() const {
  // @@protoc_insertion_point(field_get:PRO.Frd_FriendChangeOther_ntf.notify_roleiid)
  return _internal_notify_roleiid();
}
inline void Frd_FriendChangeOther_ntf::_internal_set_notify_roleiid(int64_t value) {
  
  notify_roleiid_ = value;
}
inline void Frd_FriendChangeOther_ntf::set_notify_roleiid(int64_t value) {
  _internal_set_notify_roleiid(value);
  // @@protoc_insertion_point(field_set:PRO.Frd_FriendChangeOther_ntf.notify_roleiid)
}

// bool addordel = 2;
inline void Frd_FriendChangeOther_ntf::clear_addordel() {
  addordel_ = false;
}
inline bool Frd_FriendChangeOther_ntf::_internal_addordel() const {
  return addordel_;
}
inline bool Frd_FriendChangeOther_ntf::addordel() const {
  // @@protoc_insertion_point(field_get:PRO.Frd_FriendChangeOther_ntf.addordel)
  return _internal_addordel();
}
inline void Frd_FriendChangeOther_ntf::_internal_set_addordel(bool value) {
  
  addordel_ = value;
}
inline void Frd_FriendChangeOther_ntf::set_addordel(bool value) {
  _internal_set_addordel(value);
  // @@protoc_insertion_point(field_set:PRO.Frd_FriendChangeOther_ntf.addordel)
}

// int64 friendiid = 3;
inline void Frd_FriendChangeOther_ntf::clear_friendiid() {
  friendiid_ = int64_t{0};
}
inline int64_t Frd_FriendChangeOther_ntf::_internal_friendiid() const {
  return friendiid_;
}
inline int64_t Frd_FriendChangeOther_ntf::friendiid() const {
  // @@protoc_insertion_point(field_get:PRO.Frd_FriendChangeOther_ntf.friendiid)
  return _internal_friendiid();
}
inline void Frd_FriendChangeOther_ntf::_internal_set_friendiid(int64_t value) {
  
  friendiid_ = value;
}
inline void Frd_FriendChangeOther_ntf::set_friendiid(int64_t value) {
  _internal_set_friendiid(value);
  // @@protoc_insertion_point(field_set:PRO.Frd_FriendChangeOther_ntf.friendiid)
}

// optional .PRO.FriendRelation friend = 4;
inline bool Frd_FriendChangeOther_ntf::_internal_has_friend_() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || friend__ != nullptr);
  return value;
}
inline bool Frd_FriendChangeOther_ntf::has_friend_() const {
  return _internal_has_friend_();
}
inline const ::PRO::FriendRelation& Frd_FriendChangeOther_ntf::_internal_friend_() const {
  const ::PRO::FriendRelation* p = friend__;
  return p != nullptr ? *p : reinterpret_cast<const ::PRO::FriendRelation&>(
      ::PRO::_FriendRelation_default_instance_);
}
inline const ::PRO::FriendRelation& Frd_FriendChangeOther_ntf::friend_() const {
  // @@protoc_insertion_point(field_get:PRO.Frd_FriendChangeOther_ntf.friend)
  return _internal_friend_();
}
inline void Frd_FriendChangeOther_ntf::unsafe_arena_set_allocated_friend_(
    ::PRO::FriendRelation* friend_) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(friend__);
  }
  friend__ = friend_;
  if (friend_) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:PRO.Frd_FriendChangeOther_ntf.friend)
}
inline ::PRO::FriendRelation* Frd_FriendChangeOther_ntf::release_friend_() {
  _has_bits_[0] &= ~0x00000001u;
  ::PRO::FriendRelation* temp = friend__;
  friend__ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::PRO::FriendRelation* Frd_FriendChangeOther_ntf::unsafe_arena_release_friend_() {
  // @@protoc_insertion_point(field_release:PRO.Frd_FriendChangeOther_ntf.friend)
  _has_bits_[0] &= ~0x00000001u;
  ::PRO::FriendRelation* temp = friend__;
  friend__ = nullptr;
  return temp;
}
inline ::PRO::FriendRelation* Frd_FriendChangeOther_ntf::_internal_mutable_friend_() {
  _has_bits_[0] |= 0x00000001u;
  if (friend__ == nullptr) {
    auto* p = CreateMaybeMessage<::PRO::FriendRelation>(GetArenaForAllocation());
    friend__ = p;
  }
  return friend__;
}
inline ::PRO::FriendRelation* Frd_FriendChangeOther_ntf::mutable_friend_() {
  ::PRO::FriendRelation* _msg = _internal_mutable_friend_();
  // @@protoc_insertion_point(field_mutable:PRO.Frd_FriendChangeOther_ntf.friend)
  return _msg;
}
inline void Frd_FriendChangeOther_ntf::set_allocated_friend_(::PRO::FriendRelation* friend_) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(friend__);
  }
  if (friend_) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper<
            ::PROTOBUF_NAMESPACE_ID::MessageLite>::GetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(friend_));
    if (message_arena != submessage_arena) {
      friend_ = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, friend_, submessage_arena);
    }
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  friend__ = friend_;
  // @@protoc_insertion_point(field_set_allocated:PRO.Frd_FriendChangeOther_ntf.friend)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace PRO

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_friend_5fsystem_5finternal_2eproto
