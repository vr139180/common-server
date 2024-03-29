// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.0
// source: user_pet.proto

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

type Pet_AdoptOneReq struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	//宠物id
	PetIid int64 `protobuf:"varint,1,opt,name=pet_iid,json=petIid,proto3" json:"pet_iid,omitempty"`
}

func (x *Pet_AdoptOneReq) Reset() {
	*x = Pet_AdoptOneReq{}
	if protoimpl.UnsafeEnabled {
		mi := &file_user_pet_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Pet_AdoptOneReq) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Pet_AdoptOneReq) ProtoMessage() {}

func (x *Pet_AdoptOneReq) ProtoReflect() protoreflect.Message {
	mi := &file_user_pet_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Pet_AdoptOneReq.ProtoReflect.Descriptor instead.
func (*Pet_AdoptOneReq) Descriptor() ([]byte, []int) {
	return file_user_pet_proto_rawDescGZIP(), []int{0}
}

func (x *Pet_AdoptOneReq) GetPetIid() int64 {
	if x != nil {
		return x.PetIid
	}
	return 0
}

type Pet_AdoptOneAck struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	//0 success 1 资源不存在
	Result int32 `protobuf:"varint,1,opt,name=result,proto3" json:"result,omitempty"`
	//success 返回pet对象
	Pet *DBUserPetItem `protobuf:"bytes,2,opt,name=pet,proto3,oneof" json:"pet,omitempty"`
}

func (x *Pet_AdoptOneAck) Reset() {
	*x = Pet_AdoptOneAck{}
	if protoimpl.UnsafeEnabled {
		mi := &file_user_pet_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Pet_AdoptOneAck) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Pet_AdoptOneAck) ProtoMessage() {}

func (x *Pet_AdoptOneAck) ProtoReflect() protoreflect.Message {
	mi := &file_user_pet_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Pet_AdoptOneAck.ProtoReflect.Descriptor instead.
func (*Pet_AdoptOneAck) Descriptor() ([]byte, []int) {
	return file_user_pet_proto_rawDescGZIP(), []int{1}
}

func (x *Pet_AdoptOneAck) GetResult() int32 {
	if x != nil {
		return x.Result
	}
	return 0
}

func (x *Pet_AdoptOneAck) GetPet() *DBUserPetItem {
	if x != nil {
		return x.Pet
	}
	return nil
}

type Pet_ReleaseOneReq struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	MypetIid int64 `protobuf:"varint,1,opt,name=mypet_iid,json=mypetIid,proto3" json:"mypet_iid,omitempty"`
}

func (x *Pet_ReleaseOneReq) Reset() {
	*x = Pet_ReleaseOneReq{}
	if protoimpl.UnsafeEnabled {
		mi := &file_user_pet_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Pet_ReleaseOneReq) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Pet_ReleaseOneReq) ProtoMessage() {}

func (x *Pet_ReleaseOneReq) ProtoReflect() protoreflect.Message {
	mi := &file_user_pet_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Pet_ReleaseOneReq.ProtoReflect.Descriptor instead.
func (*Pet_ReleaseOneReq) Descriptor() ([]byte, []int) {
	return file_user_pet_proto_rawDescGZIP(), []int{2}
}

func (x *Pet_ReleaseOneReq) GetMypetIid() int64 {
	if x != nil {
		return x.MypetIid
	}
	return 0
}

type Pet_ReleaseOneAck struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	MypetIid int64 `protobuf:"varint,1,opt,name=mypet_iid,json=mypetIid,proto3" json:"mypet_iid,omitempty"`
	//0 success 1 fail
	Result int32 `protobuf:"varint,2,opt,name=result,proto3" json:"result,omitempty"`
}

func (x *Pet_ReleaseOneAck) Reset() {
	*x = Pet_ReleaseOneAck{}
	if protoimpl.UnsafeEnabled {
		mi := &file_user_pet_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Pet_ReleaseOneAck) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Pet_ReleaseOneAck) ProtoMessage() {}

func (x *Pet_ReleaseOneAck) ProtoReflect() protoreflect.Message {
	mi := &file_user_pet_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Pet_ReleaseOneAck.ProtoReflect.Descriptor instead.
func (*Pet_ReleaseOneAck) Descriptor() ([]byte, []int) {
	return file_user_pet_proto_rawDescGZIP(), []int{3}
}

func (x *Pet_ReleaseOneAck) GetMypetIid() int64 {
	if x != nil {
		return x.MypetIid
	}
	return 0
}

func (x *Pet_ReleaseOneAck) GetResult() int32 {
	if x != nil {
		return x.Result
	}
	return 0
}

var File_user_pet_proto protoreflect.FileDescriptor

var file_user_pet_proto_rawDesc = []byte{
	0x0a, 0x0e, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x70, 0x65, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x12, 0x03, 0x50, 0x52, 0x4f, 0x1a, 0x11, 0x64, 0x62, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e,
	0x61, 0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22, 0x2b, 0x0a, 0x10, 0x50, 0x65, 0x74, 0x5f,
	0x41, 0x64, 0x6f, 0x70, 0x74, 0x4f, 0x6e, 0x65, 0x5f, 0x72, 0x65, 0x71, 0x12, 0x17, 0x0a, 0x07,
	0x70, 0x65, 0x74, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x06, 0x70,
	0x65, 0x74, 0x49, 0x69, 0x64, 0x22, 0x5d, 0x0a, 0x10, 0x50, 0x65, 0x74, 0x5f, 0x41, 0x64, 0x6f,
	0x70, 0x74, 0x4f, 0x6e, 0x65, 0x5f, 0x61, 0x63, 0x6b, 0x12, 0x16, 0x0a, 0x06, 0x72, 0x65, 0x73,
	0x75, 0x6c, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x06, 0x72, 0x65, 0x73, 0x75, 0x6c,
	0x74, 0x12, 0x29, 0x0a, 0x03, 0x70, 0x65, 0x74, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x12,
	0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x44, 0x42, 0x55, 0x73, 0x65, 0x72, 0x50, 0x65, 0x74, 0x49, 0x74,
	0x65, 0x6d, 0x48, 0x00, 0x52, 0x03, 0x70, 0x65, 0x74, 0x88, 0x01, 0x01, 0x42, 0x06, 0x0a, 0x04,
	0x5f, 0x70, 0x65, 0x74, 0x22, 0x31, 0x0a, 0x12, 0x50, 0x65, 0x74, 0x5f, 0x52, 0x65, 0x6c, 0x65,
	0x61, 0x73, 0x65, 0x4f, 0x6e, 0x65, 0x5f, 0x72, 0x65, 0x71, 0x12, 0x1b, 0x0a, 0x09, 0x6d, 0x79,
	0x70, 0x65, 0x74, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x08, 0x6d,
	0x79, 0x70, 0x65, 0x74, 0x49, 0x69, 0x64, 0x22, 0x49, 0x0a, 0x12, 0x50, 0x65, 0x74, 0x5f, 0x52,
	0x65, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x4f, 0x6e, 0x65, 0x5f, 0x61, 0x63, 0x6b, 0x12, 0x1b, 0x0a,
	0x09, 0x6d, 0x79, 0x70, 0x65, 0x74, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03,
	0x52, 0x08, 0x6d, 0x79, 0x70, 0x65, 0x74, 0x49, 0x69, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x72, 0x65,
	0x73, 0x75, 0x6c, 0x74, 0x18, 0x02, 0x20, 0x01, 0x28, 0x05, 0x52, 0x06, 0x72, 0x65, 0x73, 0x75,
	0x6c, 0x74, 0x42, 0x54, 0x0a, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c,
	0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72,
	0x6f, 0x50, 0x01, 0x5a, 0x15, 0x67, 0x61, 0x6d, 0x65, 0x6c, 0x69, 0x62, 0x2f, 0x70, 0x72, 0x6f,
	0x74, 0x6f, 0x62, 0x75, 0x66, 0x2f, 0x67, 0x70, 0x72, 0x6f, 0xaa, 0x02, 0x1b, 0x63, 0x6f, 0x6d,
	0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77,
	0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_user_pet_proto_rawDescOnce sync.Once
	file_user_pet_proto_rawDescData = file_user_pet_proto_rawDesc
)

func file_user_pet_proto_rawDescGZIP() []byte {
	file_user_pet_proto_rawDescOnce.Do(func() {
		file_user_pet_proto_rawDescData = protoimpl.X.CompressGZIP(file_user_pet_proto_rawDescData)
	})
	return file_user_pet_proto_rawDescData
}

var file_user_pet_proto_msgTypes = make([]protoimpl.MessageInfo, 4)
var file_user_pet_proto_goTypes = []interface{}{
	(*Pet_AdoptOneReq)(nil),   // 0: PRO.Pet_AdoptOne_req
	(*Pet_AdoptOneAck)(nil),   // 1: PRO.Pet_AdoptOne_ack
	(*Pet_ReleaseOneReq)(nil), // 2: PRO.Pet_ReleaseOne_req
	(*Pet_ReleaseOneAck)(nil), // 3: PRO.Pet_ReleaseOne_ack
	(*DBUserPetItem)(nil),     // 4: PRO.DBUserPetItem
}
var file_user_pet_proto_depIdxs = []int32{
	4, // 0: PRO.Pet_AdoptOne_ack.pet:type_name -> PRO.DBUserPetItem
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_user_pet_proto_init() }
func file_user_pet_proto_init() {
	if File_user_pet_proto != nil {
		return
	}
	file_db_internal_proto_init()
	if !protoimpl.UnsafeEnabled {
		file_user_pet_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Pet_AdoptOneReq); i {
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
		file_user_pet_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Pet_AdoptOneAck); i {
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
		file_user_pet_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Pet_ReleaseOneReq); i {
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
		file_user_pet_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Pet_ReleaseOneAck); i {
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
	file_user_pet_proto_msgTypes[1].OneofWrappers = []interface{}{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_user_pet_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   4,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_user_pet_proto_goTypes,
		DependencyIndexes: file_user_pet_proto_depIdxs,
		MessageInfos:      file_user_pet_proto_msgTypes,
	}.Build()
	File_user_pet_proto = out.File
	file_user_pet_proto_rawDesc = nil
	file_user_pet_proto_goTypes = nil
	file_user_pet_proto_depIdxs = nil
}
