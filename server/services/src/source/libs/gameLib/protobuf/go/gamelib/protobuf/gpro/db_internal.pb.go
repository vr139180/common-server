// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.0
// source: db_internal.proto

package gpro

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type DBRowDel struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ver_ uint32 `protobuf:"varint,1,opt,name=ver_,json=ver,proto3" json:"ver_,omitempty"`
	Id   int64  `protobuf:"varint,2,opt,name=id,proto3" json:"id,omitempty"`
}

func (x *DBRowDel) Reset() {
	*x = DBRowDel{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBRowDel) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBRowDel) ProtoMessage() {}

func (x *DBRowDel) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBRowDel.ProtoReflect.Descriptor instead.
func (*DBRowDel) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{0}
}

func (x *DBRowDel) GetVer_() uint32 {
	if x != nil {
		return x.Ver_
	}
	return 0
}

func (x *DBRowDel) GetId() int64 {
	if x != nil {
		return x.Id
	}
	return 0
}

type DBRowDeletes struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Dels []*DBRowDel `protobuf:"bytes,1,rep,name=dels,proto3" json:"dels,omitempty"`
}

func (x *DBRowDeletes) Reset() {
	*x = DBRowDeletes{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBRowDeletes) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBRowDeletes) ProtoMessage() {}

func (x *DBRowDeletes) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBRowDeletes.ProtoReflect.Descriptor instead.
func (*DBRowDeletes) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{1}
}

func (x *DBRowDeletes) GetDels() []*DBRowDel {
	if x != nil {
		return x.Dels
	}
	return nil
}

type DBRoleBaseInfo struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ver_      uint32 `protobuf:"varint,1,opt,name=ver_,json=ver,proto3" json:"ver_,omitempty"`
	RoleIid   int64  `protobuf:"varint,2,opt,name=role_iid,json=roleIid,proto3" json:"role_iid,omitempty"`
	UserIid   int64  `protobuf:"varint,3,opt,name=user_iid,json=userIid,proto3" json:"user_iid,omitempty"`
	Nickname  string `protobuf:"bytes,4,opt,name=nickname,proto3" json:"nickname,omitempty"`
	Registime int32  `protobuf:"varint,5,opt,name=registime,proto3" json:"registime,omitempty"`
}

func (x *DBRoleBaseInfo) Reset() {
	*x = DBRoleBaseInfo{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBRoleBaseInfo) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBRoleBaseInfo) ProtoMessage() {}

func (x *DBRoleBaseInfo) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBRoleBaseInfo.ProtoReflect.Descriptor instead.
func (*DBRoleBaseInfo) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{2}
}

func (x *DBRoleBaseInfo) GetVer_() uint32 {
	if x != nil {
		return x.Ver_
	}
	return 0
}

func (x *DBRoleBaseInfo) GetRoleIid() int64 {
	if x != nil {
		return x.RoleIid
	}
	return 0
}

func (x *DBRoleBaseInfo) GetUserIid() int64 {
	if x != nil {
		return x.UserIid
	}
	return 0
}

func (x *DBRoleBaseInfo) GetNickname() string {
	if x != nil {
		return x.Nickname
	}
	return ""
}

func (x *DBRoleBaseInfo) GetRegistime() int32 {
	if x != nil {
		return x.Registime
	}
	return 0
}

type DBUserRoles struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Roles []*DBRoleBaseInfo `protobuf:"bytes,1,rep,name=roles,proto3" json:"roles,omitempty"`
}

func (x *DBUserRoles) Reset() {
	*x = DBUserRoles{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserRoles) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserRoles) ProtoMessage() {}

func (x *DBUserRoles) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserRoles.ProtoReflect.Descriptor instead.
func (*DBUserRoles) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{3}
}

func (x *DBUserRoles) GetRoles() []*DBRoleBaseInfo {
	if x != nil {
		return x.Roles
	}
	return nil
}

type DBUserHome struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ver_           uint32 `protobuf:"varint,1,opt,name=ver_,json=ver,proto3" json:"ver_,omitempty"`
	RoleIid        int64  `protobuf:"varint,2,opt,name=role_iid,json=roleIid,proto3" json:"role_iid,omitempty"`
	HomeName       string `protobuf:"bytes,3,opt,name=home_name,json=homeName,proto3" json:"home_name,omitempty"`
	GroundResid    int64  `protobuf:"varint,4,opt,name=ground_resid,json=groundResid,proto3" json:"ground_resid,omitempty"`
	LookAt         string `protobuf:"bytes,5,opt,name=look_at,json=lookAt,proto3" json:"look_at,omitempty"`
	GeoPos         string `protobuf:"bytes,6,opt,name=geo_pos,json=geoPos,proto3" json:"geo_pos,omitempty"`
	ResideTime     int32  `protobuf:"varint,7,opt,name=reside_time,json=resideTime,proto3" json:"reside_time,omitempty"`
	LastResidedate int32  `protobuf:"varint,8,opt,name=last_residedate,json=lastResidedate,proto3" json:"last_residedate,omitempty"`
}

func (x *DBUserHome) Reset() {
	*x = DBUserHome{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[4]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserHome) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserHome) ProtoMessage() {}

func (x *DBUserHome) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[4]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserHome.ProtoReflect.Descriptor instead.
func (*DBUserHome) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{4}
}

func (x *DBUserHome) GetVer_() uint32 {
	if x != nil {
		return x.Ver_
	}
	return 0
}

func (x *DBUserHome) GetRoleIid() int64 {
	if x != nil {
		return x.RoleIid
	}
	return 0
}

func (x *DBUserHome) GetHomeName() string {
	if x != nil {
		return x.HomeName
	}
	return ""
}

func (x *DBUserHome) GetGroundResid() int64 {
	if x != nil {
		return x.GroundResid
	}
	return 0
}

func (x *DBUserHome) GetLookAt() string {
	if x != nil {
		return x.LookAt
	}
	return ""
}

func (x *DBUserHome) GetGeoPos() string {
	if x != nil {
		return x.GeoPos
	}
	return ""
}

func (x *DBUserHome) GetResideTime() int32 {
	if x != nil {
		return x.ResideTime
	}
	return 0
}

func (x *DBUserHome) GetLastResidedate() int32 {
	if x != nil {
		return x.LastResidedate
	}
	return 0
}

type DBUserHomeStructureItem struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ver_           uint32 `protobuf:"varint,1,opt,name=ver_,json=ver,proto3" json:"ver_,omitempty"`
	BuildingIid    int64  `protobuf:"varint,2,opt,name=building_iid,json=buildingIid,proto3" json:"building_iid,omitempty"`
	HomeIid        int64  `protobuf:"varint,3,opt,name=home_iid,json=homeIid,proto3" json:"home_iid,omitempty"`
	ParentBuilding int64  `protobuf:"varint,4,opt,name=parent_building,json=parentBuilding,proto3" json:"parent_building,omitempty"`
	BuildingResid  int64  `protobuf:"varint,5,opt,name=building_resid,json=buildingResid,proto3" json:"building_resid,omitempty"`
	LookAt         string `protobuf:"bytes,6,opt,name=look_at,json=lookAt,proto3" json:"look_at,omitempty"`
	BuildingPos    string `protobuf:"bytes,7,opt,name=building_pos,json=buildingPos,proto3" json:"building_pos,omitempty"`
}

func (x *DBUserHomeStructureItem) Reset() {
	*x = DBUserHomeStructureItem{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[5]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserHomeStructureItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserHomeStructureItem) ProtoMessage() {}

func (x *DBUserHomeStructureItem) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[5]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserHomeStructureItem.ProtoReflect.Descriptor instead.
func (*DBUserHomeStructureItem) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{5}
}

func (x *DBUserHomeStructureItem) GetVer_() uint32 {
	if x != nil {
		return x.Ver_
	}
	return 0
}

func (x *DBUserHomeStructureItem) GetBuildingIid() int64 {
	if x != nil {
		return x.BuildingIid
	}
	return 0
}

func (x *DBUserHomeStructureItem) GetHomeIid() int64 {
	if x != nil {
		return x.HomeIid
	}
	return 0
}

func (x *DBUserHomeStructureItem) GetParentBuilding() int64 {
	if x != nil {
		return x.ParentBuilding
	}
	return 0
}

func (x *DBUserHomeStructureItem) GetBuildingResid() int64 {
	if x != nil {
		return x.BuildingResid
	}
	return 0
}

func (x *DBUserHomeStructureItem) GetLookAt() string {
	if x != nil {
		return x.LookAt
	}
	return ""
}

func (x *DBUserHomeStructureItem) GetBuildingPos() string {
	if x != nil {
		return x.BuildingPos
	}
	return ""
}

type DBUserHomeStructure struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Items []*DBUserHomeStructureItem `protobuf:"bytes,1,rep,name=items,proto3" json:"items,omitempty"`
}

func (x *DBUserHomeStructure) Reset() {
	*x = DBUserHomeStructure{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[6]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserHomeStructure) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserHomeStructure) ProtoMessage() {}

func (x *DBUserHomeStructure) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[6]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserHomeStructure.ProtoReflect.Descriptor instead.
func (*DBUserHomeStructure) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{6}
}

func (x *DBUserHomeStructure) GetItems() []*DBUserHomeStructureItem {
	if x != nil {
		return x.Items
	}
	return nil
}

type DBPetCategory struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	PetIid   int64  `protobuf:"varint,2,opt,name=pet_iid,json=petIid,proto3" json:"pet_iid,omitempty"`
	PetType  int32  `protobuf:"varint,3,opt,name=pet_type,json=petType,proto3" json:"pet_type,omitempty"`
	PetResid int64  `protobuf:"varint,4,opt,name=pet_resid,json=petResid,proto3" json:"pet_resid,omitempty"`
	PetSex   int32  `protobuf:"varint,5,opt,name=pet_sex,json=petSex,proto3" json:"pet_sex,omitempty"`
	PetName  string `protobuf:"bytes,6,opt,name=pet_name,json=petName,proto3" json:"pet_name,omitempty"`
}

func (x *DBPetCategory) Reset() {
	*x = DBPetCategory{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[7]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBPetCategory) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBPetCategory) ProtoMessage() {}

func (x *DBPetCategory) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[7]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBPetCategory.ProtoReflect.Descriptor instead.
func (*DBPetCategory) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{7}
}

func (x *DBPetCategory) GetPetIid() int64 {
	if x != nil {
		return x.PetIid
	}
	return 0
}

func (x *DBPetCategory) GetPetType() int32 {
	if x != nil {
		return x.PetType
	}
	return 0
}

func (x *DBPetCategory) GetPetResid() int64 {
	if x != nil {
		return x.PetResid
	}
	return 0
}

func (x *DBPetCategory) GetPetSex() int32 {
	if x != nil {
		return x.PetSex
	}
	return 0
}

func (x *DBPetCategory) GetPetName() string {
	if x != nil {
		return x.PetName
	}
	return ""
}

type DBUserPetItem struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Ver_     uint32 `protobuf:"varint,1,opt,name=ver_,json=ver,proto3" json:"ver_,omitempty"`
	MypetIid int64  `protobuf:"varint,2,opt,name=mypet_iid,json=mypetIid,proto3" json:"mypet_iid,omitempty"`
	RoleIid  int64  `protobuf:"varint,3,opt,name=role_iid,json=roleIid,proto3" json:"role_iid,omitempty"`
	PetIid   int64  `protobuf:"varint,4,opt,name=pet_iid,json=petIid,proto3" json:"pet_iid,omitempty"`
	PetAge   int32  `protobuf:"varint,5,opt,name=pet_age,json=petAge,proto3" json:"pet_age,omitempty"`
	Birthday int32  `protobuf:"varint,6,opt,name=birthday,proto3" json:"birthday,omitempty"`
}

func (x *DBUserPetItem) Reset() {
	*x = DBUserPetItem{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[8]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserPetItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserPetItem) ProtoMessage() {}

func (x *DBUserPetItem) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[8]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserPetItem.ProtoReflect.Descriptor instead.
func (*DBUserPetItem) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{8}
}

func (x *DBUserPetItem) GetVer_() uint32 {
	if x != nil {
		return x.Ver_
	}
	return 0
}

func (x *DBUserPetItem) GetMypetIid() int64 {
	if x != nil {
		return x.MypetIid
	}
	return 0
}

func (x *DBUserPetItem) GetRoleIid() int64 {
	if x != nil {
		return x.RoleIid
	}
	return 0
}

func (x *DBUserPetItem) GetPetIid() int64 {
	if x != nil {
		return x.PetIid
	}
	return 0
}

func (x *DBUserPetItem) GetPetAge() int32 {
	if x != nil {
		return x.PetAge
	}
	return 0
}

func (x *DBUserPetItem) GetBirthday() int32 {
	if x != nil {
		return x.Birthday
	}
	return 0
}

type DBUserPets struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Pets []*DBUserPetItem `protobuf:"bytes,1,rep,name=pets,proto3" json:"pets,omitempty"`
}

func (x *DBUserPets) Reset() {
	*x = DBUserPets{}
	if protoimpl.UnsafeEnabled {
		mi := &file_db_internal_proto_msgTypes[9]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *DBUserPets) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*DBUserPets) ProtoMessage() {}

func (x *DBUserPets) ProtoReflect() protoreflect.Message {
	mi := &file_db_internal_proto_msgTypes[9]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use DBUserPets.ProtoReflect.Descriptor instead.
func (*DBUserPets) Descriptor() ([]byte, []int) {
	return file_db_internal_proto_rawDescGZIP(), []int{9}
}

func (x *DBUserPets) GetPets() []*DBUserPetItem {
	if x != nil {
		return x.Pets
	}
	return nil
}

var File_db_internal_proto protoreflect.FileDescriptor

var file_db_internal_proto_rawDesc = []byte{
	0x0a, 0x11, 0x64, 0x62, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x2e, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x12, 0x03, 0x50, 0x52, 0x4f, 0x22, 0x2d, 0x0a, 0x08, 0x44, 0x42, 0x52, 0x6f,
	0x77, 0x44, 0x65, 0x6c, 0x12, 0x11, 0x0a, 0x04, 0x76, 0x65, 0x72, 0x5f, 0x18, 0x01, 0x20, 0x01,
	0x28, 0x0d, 0x52, 0x03, 0x76, 0x65, 0x72, 0x12, 0x0e, 0x0a, 0x02, 0x69, 0x64, 0x18, 0x02, 0x20,
	0x01, 0x28, 0x03, 0x52, 0x02, 0x69, 0x64, 0x22, 0x31, 0x0a, 0x0c, 0x44, 0x42, 0x52, 0x6f, 0x77,
	0x44, 0x65, 0x6c, 0x65, 0x74, 0x65, 0x73, 0x12, 0x21, 0x0a, 0x04, 0x64, 0x65, 0x6c, 0x73, 0x18,
	0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x0d, 0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x44, 0x42, 0x52, 0x6f,
	0x77, 0x44, 0x65, 0x6c, 0x52, 0x04, 0x64, 0x65, 0x6c, 0x73, 0x22, 0x93, 0x01, 0x0a, 0x0e, 0x44,
	0x42, 0x52, 0x6f, 0x6c, 0x65, 0x42, 0x61, 0x73, 0x65, 0x49, 0x6e, 0x66, 0x6f, 0x12, 0x11, 0x0a,
	0x04, 0x76, 0x65, 0x72, 0x5f, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x03, 0x76, 0x65, 0x72,
	0x12, 0x19, 0x0a, 0x08, 0x72, 0x6f, 0x6c, 0x65, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01,
	0x28, 0x03, 0x52, 0x07, 0x72, 0x6f, 0x6c, 0x65, 0x49, 0x69, 0x64, 0x12, 0x19, 0x0a, 0x08, 0x75,
	0x73, 0x65, 0x72, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x03, 0x52, 0x07, 0x75,
	0x73, 0x65, 0x72, 0x49, 0x69, 0x64, 0x12, 0x1a, 0x0a, 0x08, 0x6e, 0x69, 0x63, 0x6b, 0x6e, 0x61,
	0x6d, 0x65, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x6e, 0x69, 0x63, 0x6b, 0x6e, 0x61,
	0x6d, 0x65, 0x12, 0x1c, 0x0a, 0x09, 0x72, 0x65, 0x67, 0x69, 0x73, 0x74, 0x69, 0x6d, 0x65, 0x18,
	0x05, 0x20, 0x01, 0x28, 0x05, 0x52, 0x09, 0x72, 0x65, 0x67, 0x69, 0x73, 0x74, 0x69, 0x6d, 0x65,
	0x22, 0x38, 0x0a, 0x0b, 0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x52, 0x6f, 0x6c, 0x65, 0x73, 0x12,
	0x29, 0x0a, 0x05, 0x72, 0x6f, 0x6c, 0x65, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x13,
	0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x44, 0x42, 0x52, 0x6f, 0x6c, 0x65, 0x42, 0x61, 0x73, 0x65, 0x49,
	0x6e, 0x66, 0x6f, 0x52, 0x05, 0x72, 0x6f, 0x6c, 0x65, 0x73, 0x22, 0xf6, 0x01, 0x0a, 0x0a, 0x44,
	0x42, 0x55, 0x73, 0x65, 0x72, 0x48, 0x6f, 0x6d, 0x65, 0x12, 0x11, 0x0a, 0x04, 0x76, 0x65, 0x72,
	0x5f, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x03, 0x76, 0x65, 0x72, 0x12, 0x19, 0x0a, 0x08,
	0x72, 0x6f, 0x6c, 0x65, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x07,
	0x72, 0x6f, 0x6c, 0x65, 0x49, 0x69, 0x64, 0x12, 0x1b, 0x0a, 0x09, 0x68, 0x6f, 0x6d, 0x65, 0x5f,
	0x6e, 0x61, 0x6d, 0x65, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x68, 0x6f, 0x6d, 0x65,
	0x4e, 0x61, 0x6d, 0x65, 0x12, 0x21, 0x0a, 0x0c, 0x67, 0x72, 0x6f, 0x75, 0x6e, 0x64, 0x5f, 0x72,
	0x65, 0x73, 0x69, 0x64, 0x18, 0x04, 0x20, 0x01, 0x28, 0x03, 0x52, 0x0b, 0x67, 0x72, 0x6f, 0x75,
	0x6e, 0x64, 0x52, 0x65, 0x73, 0x69, 0x64, 0x12, 0x17, 0x0a, 0x07, 0x6c, 0x6f, 0x6f, 0x6b, 0x5f,
	0x61, 0x74, 0x18, 0x05, 0x20, 0x01, 0x28, 0x09, 0x52, 0x06, 0x6c, 0x6f, 0x6f, 0x6b, 0x41, 0x74,
	0x12, 0x17, 0x0a, 0x07, 0x67, 0x65, 0x6f, 0x5f, 0x70, 0x6f, 0x73, 0x18, 0x06, 0x20, 0x01, 0x28,
	0x09, 0x52, 0x06, 0x67, 0x65, 0x6f, 0x50, 0x6f, 0x73, 0x12, 0x1f, 0x0a, 0x0b, 0x72, 0x65, 0x73,
	0x69, 0x64, 0x65, 0x5f, 0x74, 0x69, 0x6d, 0x65, 0x18, 0x07, 0x20, 0x01, 0x28, 0x05, 0x52, 0x0a,
	0x72, 0x65, 0x73, 0x69, 0x64, 0x65, 0x54, 0x69, 0x6d, 0x65, 0x12, 0x27, 0x0a, 0x0f, 0x6c, 0x61,
	0x73, 0x74, 0x5f, 0x72, 0x65, 0x73, 0x69, 0x64, 0x65, 0x64, 0x61, 0x74, 0x65, 0x18, 0x08, 0x20,
	0x01, 0x28, 0x05, 0x52, 0x0e, 0x6c, 0x61, 0x73, 0x74, 0x52, 0x65, 0x73, 0x69, 0x64, 0x65, 0x64,
	0x61, 0x74, 0x65, 0x22, 0xf6, 0x01, 0x0a, 0x17, 0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x48, 0x6f,
	0x6d, 0x65, 0x53, 0x74, 0x72, 0x75, 0x63, 0x74, 0x75, 0x72, 0x65, 0x49, 0x74, 0x65, 0x6d, 0x12,
	0x11, 0x0a, 0x04, 0x76, 0x65, 0x72, 0x5f, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x03, 0x76,
	0x65, 0x72, 0x12, 0x21, 0x0a, 0x0c, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x69, 0x6e, 0x67, 0x5f, 0x69,
	0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x0b, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x69,
	0x6e, 0x67, 0x49, 0x69, 0x64, 0x12, 0x19, 0x0a, 0x08, 0x68, 0x6f, 0x6d, 0x65, 0x5f, 0x69, 0x69,
	0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x03, 0x52, 0x07, 0x68, 0x6f, 0x6d, 0x65, 0x49, 0x69, 0x64,
	0x12, 0x27, 0x0a, 0x0f, 0x70, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x5f, 0x62, 0x75, 0x69, 0x6c, 0x64,
	0x69, 0x6e, 0x67, 0x18, 0x04, 0x20, 0x01, 0x28, 0x03, 0x52, 0x0e, 0x70, 0x61, 0x72, 0x65, 0x6e,
	0x74, 0x42, 0x75, 0x69, 0x6c, 0x64, 0x69, 0x6e, 0x67, 0x12, 0x25, 0x0a, 0x0e, 0x62, 0x75, 0x69,
	0x6c, 0x64, 0x69, 0x6e, 0x67, 0x5f, 0x72, 0x65, 0x73, 0x69, 0x64, 0x18, 0x05, 0x20, 0x01, 0x28,
	0x03, 0x52, 0x0d, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x69, 0x6e, 0x67, 0x52, 0x65, 0x73, 0x69, 0x64,
	0x12, 0x17, 0x0a, 0x07, 0x6c, 0x6f, 0x6f, 0x6b, 0x5f, 0x61, 0x74, 0x18, 0x06, 0x20, 0x01, 0x28,
	0x09, 0x52, 0x06, 0x6c, 0x6f, 0x6f, 0x6b, 0x41, 0x74, 0x12, 0x21, 0x0a, 0x0c, 0x62, 0x75, 0x69,
	0x6c, 0x64, 0x69, 0x6e, 0x67, 0x5f, 0x70, 0x6f, 0x73, 0x18, 0x07, 0x20, 0x01, 0x28, 0x09, 0x52,
	0x0b, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x69, 0x6e, 0x67, 0x50, 0x6f, 0x73, 0x22, 0x49, 0x0a, 0x13,
	0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x48, 0x6f, 0x6d, 0x65, 0x53, 0x74, 0x72, 0x75, 0x63, 0x74,
	0x75, 0x72, 0x65, 0x12, 0x32, 0x0a, 0x05, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x18, 0x01, 0x20, 0x03,
	0x28, 0x0b, 0x32, 0x1c, 0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x48,
	0x6f, 0x6d, 0x65, 0x53, 0x74, 0x72, 0x75, 0x63, 0x74, 0x75, 0x72, 0x65, 0x49, 0x74, 0x65, 0x6d,
	0x52, 0x05, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x22, 0x94, 0x01, 0x0a, 0x0d, 0x44, 0x42, 0x50, 0x65,
	0x74, 0x43, 0x61, 0x74, 0x65, 0x67, 0x6f, 0x72, 0x79, 0x12, 0x17, 0x0a, 0x07, 0x70, 0x65, 0x74,
	0x5f, 0x69, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x06, 0x70, 0x65, 0x74, 0x49,
	0x69, 0x64, 0x12, 0x19, 0x0a, 0x08, 0x70, 0x65, 0x74, 0x5f, 0x74, 0x79, 0x70, 0x65, 0x18, 0x03,
	0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x70, 0x65, 0x74, 0x54, 0x79, 0x70, 0x65, 0x12, 0x1b, 0x0a,
	0x09, 0x70, 0x65, 0x74, 0x5f, 0x72, 0x65, 0x73, 0x69, 0x64, 0x18, 0x04, 0x20, 0x01, 0x28, 0x03,
	0x52, 0x08, 0x70, 0x65, 0x74, 0x52, 0x65, 0x73, 0x69, 0x64, 0x12, 0x17, 0x0a, 0x07, 0x70, 0x65,
	0x74, 0x5f, 0x73, 0x65, 0x78, 0x18, 0x05, 0x20, 0x01, 0x28, 0x05, 0x52, 0x06, 0x70, 0x65, 0x74,
	0x53, 0x65, 0x78, 0x12, 0x19, 0x0a, 0x08, 0x70, 0x65, 0x74, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x18,
	0x06, 0x20, 0x01, 0x28, 0x09, 0x52, 0x07, 0x70, 0x65, 0x74, 0x4e, 0x61, 0x6d, 0x65, 0x22, 0xa8,
	0x01, 0x0a, 0x0d, 0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x50, 0x65, 0x74, 0x49, 0x74, 0x65, 0x6d,
	0x12, 0x11, 0x0a, 0x04, 0x76, 0x65, 0x72, 0x5f, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x03,
	0x76, 0x65, 0x72, 0x12, 0x1b, 0x0a, 0x09, 0x6d, 0x79, 0x70, 0x65, 0x74, 0x5f, 0x69, 0x69, 0x64,
	0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x08, 0x6d, 0x79, 0x70, 0x65, 0x74, 0x49, 0x69, 0x64,
	0x12, 0x19, 0x0a, 0x08, 0x72, 0x6f, 0x6c, 0x65, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x03, 0x20, 0x01,
	0x28, 0x03, 0x52, 0x07, 0x72, 0x6f, 0x6c, 0x65, 0x49, 0x69, 0x64, 0x12, 0x17, 0x0a, 0x07, 0x70,
	0x65, 0x74, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x04, 0x20, 0x01, 0x28, 0x03, 0x52, 0x06, 0x70, 0x65,
	0x74, 0x49, 0x69, 0x64, 0x12, 0x17, 0x0a, 0x07, 0x70, 0x65, 0x74, 0x5f, 0x61, 0x67, 0x65, 0x18,
	0x05, 0x20, 0x01, 0x28, 0x05, 0x52, 0x06, 0x70, 0x65, 0x74, 0x41, 0x67, 0x65, 0x12, 0x1a, 0x0a,
	0x08, 0x62, 0x69, 0x72, 0x74, 0x68, 0x64, 0x61, 0x79, 0x18, 0x06, 0x20, 0x01, 0x28, 0x05, 0x52,
	0x08, 0x62, 0x69, 0x72, 0x74, 0x68, 0x64, 0x61, 0x79, 0x22, 0x34, 0x0a, 0x0a, 0x44, 0x42, 0x55,
	0x73, 0x65, 0x72, 0x50, 0x65, 0x74, 0x73, 0x12, 0x26, 0x0a, 0x04, 0x70, 0x65, 0x74, 0x73, 0x18,
	0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x12, 0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x44, 0x42, 0x55, 0x73,
	0x65, 0x72, 0x50, 0x65, 0x74, 0x49, 0x74, 0x65, 0x6d, 0x52, 0x04, 0x70, 0x65, 0x74, 0x73, 0x42,
	0x54, 0x0a, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e,
	0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x50, 0x01,
	0x5a, 0x15, 0x67, 0x61, 0x6d, 0x65, 0x6c, 0x69, 0x62, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62,
	0x75, 0x66, 0x2f, 0x67, 0x70, 0x72, 0x6f, 0xaa, 0x02, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d,
	0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b,
	0x2e, 0x67, 0x70, 0x72, 0x6f, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_db_internal_proto_rawDescOnce sync.Once
	file_db_internal_proto_rawDescData = file_db_internal_proto_rawDesc
)

func file_db_internal_proto_rawDescGZIP() []byte {
	file_db_internal_proto_rawDescOnce.Do(func() {
		file_db_internal_proto_rawDescData = protoimpl.X.CompressGZIP(file_db_internal_proto_rawDescData)
	})
	return file_db_internal_proto_rawDescData
}

var file_db_internal_proto_msgTypes = make([]protoimpl.MessageInfo, 10)
var file_db_internal_proto_goTypes = []interface{}{
	(*DBRowDel)(nil),                // 0: PRO.DBRowDel
	(*DBRowDeletes)(nil),            // 1: PRO.DBRowDeletes
	(*DBRoleBaseInfo)(nil),          // 2: PRO.DBRoleBaseInfo
	(*DBUserRoles)(nil),             // 3: PRO.DBUserRoles
	(*DBUserHome)(nil),              // 4: PRO.DBUserHome
	(*DBUserHomeStructureItem)(nil), // 5: PRO.DBUserHomeStructureItem
	(*DBUserHomeStructure)(nil),     // 6: PRO.DBUserHomeStructure
	(*DBPetCategory)(nil),           // 7: PRO.DBPetCategory
	(*DBUserPetItem)(nil),           // 8: PRO.DBUserPetItem
	(*DBUserPets)(nil),              // 9: PRO.DBUserPets
}
var file_db_internal_proto_depIdxs = []int32{
	0, // 0: PRO.DBRowDeletes.dels:type_name -> PRO.DBRowDel
	2, // 1: PRO.DBUserRoles.roles:type_name -> PRO.DBRoleBaseInfo
	5, // 2: PRO.DBUserHomeStructure.items:type_name -> PRO.DBUserHomeStructureItem
	8, // 3: PRO.DBUserPets.pets:type_name -> PRO.DBUserPetItem
	4, // [4:4] is the sub-list for method output_type
	4, // [4:4] is the sub-list for method input_type
	4, // [4:4] is the sub-list for extension type_name
	4, // [4:4] is the sub-list for extension extendee
	0, // [0:4] is the sub-list for field type_name
}

func init() { file_db_internal_proto_init() }
func file_db_internal_proto_init() {
	if File_db_internal_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_db_internal_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBRowDel); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBRowDeletes); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBRoleBaseInfo); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserRoles); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[4].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserHome); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[5].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserHomeStructureItem); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[6].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserHomeStructure); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[7].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBPetCategory); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[8].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserPetItem); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_db_internal_proto_msgTypes[9].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*DBUserPets); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_db_internal_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   10,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_db_internal_proto_goTypes,
		DependencyIndexes: file_db_internal_proto_depIdxs,
		MessageInfos:      file_db_internal_proto_msgTypes,
	}.Build()
	File_db_internal_proto = out.File
	file_db_internal_proto_rawDesc = nil
	file_db_internal_proto_goTypes = nil
	file_db_internal_proto_depIdxs = nil
}
