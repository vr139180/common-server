// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.0
// source: robot_test_internal.proto

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

type ROBOT_CONST int32

const (
	ROBOT_CONST_ROBOT_CONST_UNIVERSAL ROBOT_CONST = 0
	ROBOT_CONST_ROBOT_VERSION         ROBOT_CONST = 1
)

// Enum value maps for ROBOT_CONST.
var (
	ROBOT_CONST_name = map[int32]string{
		0: "ROBOT_CONST_UNIVERSAL",
		1: "ROBOT_VERSION",
	}
	ROBOT_CONST_value = map[string]int32{
		"ROBOT_CONST_UNIVERSAL": 0,
		"ROBOT_VERSION":         1,
	}
)

func (x ROBOT_CONST) Enum() *ROBOT_CONST {
	p := new(ROBOT_CONST)
	*p = x
	return p
}

func (x ROBOT_CONST) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ROBOT_CONST) Descriptor() protoreflect.EnumDescriptor {
	return file_robot_test_internal_proto_enumTypes[0].Descriptor()
}

func (ROBOT_CONST) Type() protoreflect.EnumType {
	return &file_robot_test_internal_proto_enumTypes[0]
}

func (x ROBOT_CONST) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ROBOT_CONST.Descriptor instead.
func (ROBOT_CONST) EnumDescriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{0}
}

type Robot_Config_Req struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Version   int32  `protobuf:"varint,1,opt,name=version,proto3" json:"version,omitempty"`
	Robotname string `protobuf:"bytes,2,opt,name=robotname,proto3" json:"robotname,omitempty"`
	Robotpwd  string `protobuf:"bytes,3,opt,name=robotpwd,proto3" json:"robotpwd,omitempty"`
}

func (x *Robot_Config_Req) Reset() {
	*x = Robot_Config_Req{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_Config_Req) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_Config_Req) ProtoMessage() {}

func (x *Robot_Config_Req) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_Config_Req.ProtoReflect.Descriptor instead.
func (*Robot_Config_Req) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{0}
}

func (x *Robot_Config_Req) GetVersion() int32 {
	if x != nil {
		return x.Version
	}
	return 0
}

func (x *Robot_Config_Req) GetRobotname() string {
	if x != nil {
		return x.Robotname
	}
	return ""
}

func (x *Robot_Config_Req) GetRobotpwd() string {
	if x != nil {
		return x.Robotpwd
	}
	return ""
}

type Robot_Config_Ack struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	//机器人协议版本
	Version int32 `protobuf:"varint,1,opt,name=version,proto3" json:"version,omitempty"`
	//0 成功,-1没有此机器人,-2密码错,-3其他错误, -4版本错误
	Result int32 `protobuf:"varint,2,opt,name=result,proto3" json:"result,omitempty"`
	//只有ret == 0 时,才有下面选项
	//机器人在服务器的唯一标识
	Robotid     *int32  `protobuf:"varint,3,opt,name=robotid,proto3,oneof" json:"robotid,omitempty"`
	Urladdr     *string `protobuf:"bytes,4,opt,name=urladdr,proto3,oneof" json:"urladdr,omitempty"`
	Openprefix  *string `protobuf:"bytes,5,opt,name=openprefix,proto3,oneof" json:"openprefix,omitempty"`
	Startuserid *int64  `protobuf:"varint,7,opt,name=startuserid,proto3,oneof" json:"startuserid,omitempty"`
	Users       *int32  `protobuf:"varint,8,opt,name=users,proto3,oneof" json:"users,omitempty"`
	Dbuser      *string `protobuf:"bytes,10,opt,name=dbuser,proto3,oneof" json:"dbuser,omitempty"`
	Dbpwd       *string `protobuf:"bytes,11,opt,name=dbpwd,proto3,oneof" json:"dbpwd,omitempty"`
	Dbname      *string `protobuf:"bytes,12,opt,name=dbname,proto3,oneof" json:"dbname,omitempty"`
	Dbip        *string `protobuf:"bytes,13,opt,name=dbip,proto3,oneof" json:"dbip,omitempty"`
}

func (x *Robot_Config_Ack) Reset() {
	*x = Robot_Config_Ack{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_Config_Ack) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_Config_Ack) ProtoMessage() {}

func (x *Robot_Config_Ack) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_Config_Ack.ProtoReflect.Descriptor instead.
func (*Robot_Config_Ack) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{1}
}

func (x *Robot_Config_Ack) GetVersion() int32 {
	if x != nil {
		return x.Version
	}
	return 0
}

func (x *Robot_Config_Ack) GetResult() int32 {
	if x != nil {
		return x.Result
	}
	return 0
}

func (x *Robot_Config_Ack) GetRobotid() int32 {
	if x != nil && x.Robotid != nil {
		return *x.Robotid
	}
	return 0
}

func (x *Robot_Config_Ack) GetUrladdr() string {
	if x != nil && x.Urladdr != nil {
		return *x.Urladdr
	}
	return ""
}

func (x *Robot_Config_Ack) GetOpenprefix() string {
	if x != nil && x.Openprefix != nil {
		return *x.Openprefix
	}
	return ""
}

func (x *Robot_Config_Ack) GetStartuserid() int64 {
	if x != nil && x.Startuserid != nil {
		return *x.Startuserid
	}
	return 0
}

func (x *Robot_Config_Ack) GetUsers() int32 {
	if x != nil && x.Users != nil {
		return *x.Users
	}
	return 0
}

func (x *Robot_Config_Ack) GetDbuser() string {
	if x != nil && x.Dbuser != nil {
		return *x.Dbuser
	}
	return ""
}

func (x *Robot_Config_Ack) GetDbpwd() string {
	if x != nil && x.Dbpwd != nil {
		return *x.Dbpwd
	}
	return ""
}

func (x *Robot_Config_Ack) GetDbname() string {
	if x != nil && x.Dbname != nil {
		return *x.Dbname
	}
	return ""
}

func (x *Robot_Config_Ack) GetDbip() string {
	if x != nil && x.Dbip != nil {
		return *x.Dbip
	}
	return ""
}

type Robot_Start_Ack struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	//开始的用户数,不能大于配置里的 users
	Startusernum int32 `protobuf:"varint,1,opt,name=startusernum,proto3" json:"startusernum,omitempty"`
	//两个用户间登录时间间隔,单位(毫秒)
	Logininterval int64 `protobuf:"varint,2,opt,name=logininterval,proto3" json:"logininterval,omitempty"`
	//机器人类型,参照 TEST_TYPE
	Behavior []string `protobuf:"bytes,3,rep,name=behavior,proto3" json:"behavior,omitempty"`
}

func (x *Robot_Start_Ack) Reset() {
	*x = Robot_Start_Ack{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_Start_Ack) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_Start_Ack) ProtoMessage() {}

func (x *Robot_Start_Ack) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_Start_Ack.ProtoReflect.Descriptor instead.
func (*Robot_Start_Ack) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{2}
}

func (x *Robot_Start_Ack) GetStartusernum() int32 {
	if x != nil {
		return x.Startusernum
	}
	return 0
}

func (x *Robot_Start_Ack) GetLogininterval() int64 {
	if x != nil {
		return x.Logininterval
	}
	return 0
}

func (x *Robot_Start_Ack) GetBehavior() []string {
	if x != nil {
		return x.Behavior
	}
	return nil
}

type Robot_Stop_Ack struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}

func (x *Robot_Stop_Ack) Reset() {
	*x = Robot_Stop_Ack{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_Stop_Ack) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_Stop_Ack) ProtoMessage() {}

func (x *Robot_Stop_Ack) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_Stop_Ack.ProtoReflect.Descriptor instead.
func (*Robot_Stop_Ack) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{3}
}

type Robot_RecordReq struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Robotid int32 `protobuf:"varint,1,opt,name=robotid,proto3" json:"robotid,omitempty"`
	Logs    int32 `protobuf:"varint,2,opt,name=logs,proto3" json:"logs,omitempty"`
}

func (x *Robot_RecordReq) Reset() {
	*x = Robot_RecordReq{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[4]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_RecordReq) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_RecordReq) ProtoMessage() {}

func (x *Robot_RecordReq) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[4]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_RecordReq.ProtoReflect.Descriptor instead.
func (*Robot_RecordReq) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{4}
}

func (x *Robot_RecordReq) GetRobotid() int32 {
	if x != nil {
		return x.Robotid
	}
	return 0
}

func (x *Robot_RecordReq) GetLogs() int32 {
	if x != nil {
		return x.Logs
	}
	return 0
}

type Robot_StartRecordReq struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Robotid int32 `protobuf:"varint,1,opt,name=robotid,proto3" json:"robotid,omitempty"`
}

func (x *Robot_StartRecordReq) Reset() {
	*x = Robot_StartRecordReq{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[5]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_StartRecordReq) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_StartRecordReq) ProtoMessage() {}

func (x *Robot_StartRecordReq) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[5]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_StartRecordReq.ProtoReflect.Descriptor instead.
func (*Robot_StartRecordReq) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{5}
}

func (x *Robot_StartRecordReq) GetRobotid() int32 {
	if x != nil {
		return x.Robotid
	}
	return 0
}

type Robot_EndRecordReq struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Robotid int32 `protobuf:"varint,1,opt,name=robotid,proto3" json:"robotid,omitempty"`
	Logs    int32 `protobuf:"varint,2,opt,name=logs,proto3" json:"logs,omitempty"`
}

func (x *Robot_EndRecordReq) Reset() {
	*x = Robot_EndRecordReq{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[6]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_EndRecordReq) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_EndRecordReq) ProtoMessage() {}

func (x *Robot_EndRecordReq) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[6]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_EndRecordReq.ProtoReflect.Descriptor instead.
func (*Robot_EndRecordReq) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{6}
}

func (x *Robot_EndRecordReq) GetRobotid() int32 {
	if x != nil {
		return x.Robotid
	}
	return 0
}

func (x *Robot_EndRecordReq) GetLogs() int32 {
	if x != nil {
		return x.Logs
	}
	return 0
}

type Robot_UploadLogsNtf struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}

func (x *Robot_UploadLogsNtf) Reset() {
	*x = Robot_UploadLogsNtf{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[7]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_UploadLogsNtf) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_UploadLogsNtf) ProtoMessage() {}

func (x *Robot_UploadLogsNtf) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[7]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_UploadLogsNtf.ProtoReflect.Descriptor instead.
func (*Robot_UploadLogsNtf) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{7}
}

type Robot_ClearLogsNtf struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields
}

func (x *Robot_ClearLogsNtf) Reset() {
	*x = Robot_ClearLogsNtf{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[8]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_ClearLogsNtf) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_ClearLogsNtf) ProtoMessage() {}

func (x *Robot_ClearLogsNtf) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[8]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_ClearLogsNtf.ProtoReflect.Descriptor instead.
func (*Robot_ClearLogsNtf) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{8}
}

type Robot_UserPrefixChgNtf struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Prefix string `protobuf:"bytes,1,opt,name=prefix,proto3" json:"prefix,omitempty"`
}

func (x *Robot_UserPrefixChgNtf) Reset() {
	*x = Robot_UserPrefixChgNtf{}
	if protoimpl.UnsafeEnabled {
		mi := &file_robot_test_internal_proto_msgTypes[9]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *Robot_UserPrefixChgNtf) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Robot_UserPrefixChgNtf) ProtoMessage() {}

func (x *Robot_UserPrefixChgNtf) ProtoReflect() protoreflect.Message {
	mi := &file_robot_test_internal_proto_msgTypes[9]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use Robot_UserPrefixChgNtf.ProtoReflect.Descriptor instead.
func (*Robot_UserPrefixChgNtf) Descriptor() ([]byte, []int) {
	return file_robot_test_internal_proto_rawDescGZIP(), []int{9}
}

func (x *Robot_UserPrefixChgNtf) GetPrefix() string {
	if x != nil {
		return x.Prefix
	}
	return ""
}

var File_robot_test_internal_proto protoreflect.FileDescriptor

var file_robot_test_internal_proto_rawDesc = []byte{
	0x0a, 0x19, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x69, 0x6e, 0x74,
	0x65, 0x72, 0x6e, 0x61, 0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x03, 0x50, 0x52, 0x4f,
	0x22, 0x66, 0x0a, 0x10, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67,
	0x5f, 0x52, 0x65, 0x71, 0x12, 0x18, 0x0a, 0x07, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x18,
	0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x12, 0x1c,
	0x0a, 0x09, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x6e, 0x61, 0x6d, 0x65, 0x18, 0x02, 0x20, 0x01, 0x28,
	0x09, 0x52, 0x09, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x6e, 0x61, 0x6d, 0x65, 0x12, 0x1a, 0x0a, 0x08,
	0x72, 0x6f, 0x62, 0x6f, 0x74, 0x70, 0x77, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08,
	0x72, 0x6f, 0x62, 0x6f, 0x74, 0x70, 0x77, 0x64, 0x22, 0xc1, 0x03, 0x0a, 0x10, 0x52, 0x6f, 0x62,
	0x6f, 0x74, 0x5f, 0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x5f, 0x41, 0x63, 0x6b, 0x12, 0x18, 0x0a,
	0x07, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07,
	0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x12, 0x16, 0x0a, 0x06, 0x72, 0x65, 0x73, 0x75, 0x6c,
	0x74, 0x18, 0x02, 0x20, 0x01, 0x28, 0x05, 0x52, 0x06, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x12,
	0x1d, 0x0a, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x05,
	0x48, 0x00, 0x52, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x88, 0x01, 0x01, 0x12, 0x1d,
	0x0a, 0x07, 0x75, 0x72, 0x6c, 0x61, 0x64, 0x64, 0x72, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x48,
	0x01, 0x52, 0x07, 0x75, 0x72, 0x6c, 0x61, 0x64, 0x64, 0x72, 0x88, 0x01, 0x01, 0x12, 0x23, 0x0a,
	0x0a, 0x6f, 0x70, 0x65, 0x6e, 0x70, 0x72, 0x65, 0x66, 0x69, 0x78, 0x18, 0x05, 0x20, 0x01, 0x28,
	0x09, 0x48, 0x02, 0x52, 0x0a, 0x6f, 0x70, 0x65, 0x6e, 0x70, 0x72, 0x65, 0x66, 0x69, 0x78, 0x88,
	0x01, 0x01, 0x12, 0x25, 0x0a, 0x0b, 0x73, 0x74, 0x61, 0x72, 0x74, 0x75, 0x73, 0x65, 0x72, 0x69,
	0x64, 0x18, 0x07, 0x20, 0x01, 0x28, 0x03, 0x48, 0x03, 0x52, 0x0b, 0x73, 0x74, 0x61, 0x72, 0x74,
	0x75, 0x73, 0x65, 0x72, 0x69, 0x64, 0x88, 0x01, 0x01, 0x12, 0x19, 0x0a, 0x05, 0x75, 0x73, 0x65,
	0x72, 0x73, 0x18, 0x08, 0x20, 0x01, 0x28, 0x05, 0x48, 0x04, 0x52, 0x05, 0x75, 0x73, 0x65, 0x72,
	0x73, 0x88, 0x01, 0x01, 0x12, 0x1b, 0x0a, 0x06, 0x64, 0x62, 0x75, 0x73, 0x65, 0x72, 0x18, 0x0a,
	0x20, 0x01, 0x28, 0x09, 0x48, 0x05, 0x52, 0x06, 0x64, 0x62, 0x75, 0x73, 0x65, 0x72, 0x88, 0x01,
	0x01, 0x12, 0x19, 0x0a, 0x05, 0x64, 0x62, 0x70, 0x77, 0x64, 0x18, 0x0b, 0x20, 0x01, 0x28, 0x09,
	0x48, 0x06, 0x52, 0x05, 0x64, 0x62, 0x70, 0x77, 0x64, 0x88, 0x01, 0x01, 0x12, 0x1b, 0x0a, 0x06,
	0x64, 0x62, 0x6e, 0x61, 0x6d, 0x65, 0x18, 0x0c, 0x20, 0x01, 0x28, 0x09, 0x48, 0x07, 0x52, 0x06,
	0x64, 0x62, 0x6e, 0x61, 0x6d, 0x65, 0x88, 0x01, 0x01, 0x12, 0x17, 0x0a, 0x04, 0x64, 0x62, 0x69,
	0x70, 0x18, 0x0d, 0x20, 0x01, 0x28, 0x09, 0x48, 0x08, 0x52, 0x04, 0x64, 0x62, 0x69, 0x70, 0x88,
	0x01, 0x01, 0x42, 0x0a, 0x0a, 0x08, 0x5f, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x42, 0x0a,
	0x0a, 0x08, 0x5f, 0x75, 0x72, 0x6c, 0x61, 0x64, 0x64, 0x72, 0x42, 0x0d, 0x0a, 0x0b, 0x5f, 0x6f,
	0x70, 0x65, 0x6e, 0x70, 0x72, 0x65, 0x66, 0x69, 0x78, 0x42, 0x0e, 0x0a, 0x0c, 0x5f, 0x73, 0x74,
	0x61, 0x72, 0x74, 0x75, 0x73, 0x65, 0x72, 0x69, 0x64, 0x42, 0x08, 0x0a, 0x06, 0x5f, 0x75, 0x73,
	0x65, 0x72, 0x73, 0x42, 0x09, 0x0a, 0x07, 0x5f, 0x64, 0x62, 0x75, 0x73, 0x65, 0x72, 0x42, 0x08,
	0x0a, 0x06, 0x5f, 0x64, 0x62, 0x70, 0x77, 0x64, 0x42, 0x09, 0x0a, 0x07, 0x5f, 0x64, 0x62, 0x6e,
	0x61, 0x6d, 0x65, 0x42, 0x07, 0x0a, 0x05, 0x5f, 0x64, 0x62, 0x69, 0x70, 0x22, 0x77, 0x0a, 0x0f,
	0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x53, 0x74, 0x61, 0x72, 0x74, 0x5f, 0x41, 0x63, 0x6b, 0x12,
	0x22, 0x0a, 0x0c, 0x73, 0x74, 0x61, 0x72, 0x74, 0x75, 0x73, 0x65, 0x72, 0x6e, 0x75, 0x6d, 0x18,
	0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x0c, 0x73, 0x74, 0x61, 0x72, 0x74, 0x75, 0x73, 0x65, 0x72,
	0x6e, 0x75, 0x6d, 0x12, 0x24, 0x0a, 0x0d, 0x6c, 0x6f, 0x67, 0x69, 0x6e, 0x69, 0x6e, 0x74, 0x65,
	0x72, 0x76, 0x61, 0x6c, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x0d, 0x6c, 0x6f, 0x67, 0x69,
	0x6e, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x76, 0x61, 0x6c, 0x12, 0x1a, 0x0a, 0x08, 0x62, 0x65, 0x68,
	0x61, 0x76, 0x69, 0x6f, 0x72, 0x18, 0x03, 0x20, 0x03, 0x28, 0x09, 0x52, 0x08, 0x62, 0x65, 0x68,
	0x61, 0x76, 0x69, 0x6f, 0x72, 0x22, 0x10, 0x0a, 0x0e, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x53,
	0x74, 0x6f, 0x70, 0x5f, 0x41, 0x63, 0x6b, 0x22, 0x40, 0x0a, 0x10, 0x52, 0x6f, 0x62, 0x6f, 0x74,
	0x5f, 0x52, 0x65, 0x63, 0x6f, 0x72, 0x64, 0x5f, 0x72, 0x65, 0x71, 0x12, 0x18, 0x0a, 0x07, 0x72,
	0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x72, 0x6f,
	0x62, 0x6f, 0x74, 0x69, 0x64, 0x12, 0x12, 0x0a, 0x04, 0x6c, 0x6f, 0x67, 0x73, 0x18, 0x02, 0x20,
	0x01, 0x28, 0x05, 0x52, 0x04, 0x6c, 0x6f, 0x67, 0x73, 0x22, 0x31, 0x0a, 0x15, 0x52, 0x6f, 0x62,
	0x6f, 0x74, 0x5f, 0x53, 0x74, 0x61, 0x72, 0x74, 0x52, 0x65, 0x63, 0x6f, 0x72, 0x64, 0x5f, 0x72,
	0x65, 0x71, 0x12, 0x18, 0x0a, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x18, 0x01, 0x20,
	0x01, 0x28, 0x05, 0x52, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x22, 0x43, 0x0a, 0x13,
	0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x45, 0x6e, 0x64, 0x52, 0x65, 0x63, 0x6f, 0x72, 0x64, 0x5f,
	0x72, 0x65, 0x71, 0x12, 0x18, 0x0a, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x18, 0x01,
	0x20, 0x01, 0x28, 0x05, 0x52, 0x07, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x69, 0x64, 0x12, 0x12, 0x0a,
	0x04, 0x6c, 0x6f, 0x67, 0x73, 0x18, 0x02, 0x20, 0x01, 0x28, 0x05, 0x52, 0x04, 0x6c, 0x6f, 0x67,
	0x73, 0x22, 0x16, 0x0a, 0x14, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x55, 0x70, 0x6c, 0x6f, 0x61,
	0x64, 0x4c, 0x6f, 0x67, 0x73, 0x5f, 0x6e, 0x74, 0x66, 0x22, 0x15, 0x0a, 0x13, 0x52, 0x6f, 0x62,
	0x6f, 0x74, 0x5f, 0x43, 0x6c, 0x65, 0x61, 0x72, 0x4c, 0x6f, 0x67, 0x73, 0x5f, 0x6e, 0x74, 0x66,
	0x22, 0x31, 0x0a, 0x17, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x5f, 0x55, 0x73, 0x65, 0x72, 0x50, 0x72,
	0x65, 0x66, 0x69, 0x78, 0x43, 0x68, 0x67, 0x5f, 0x6e, 0x74, 0x66, 0x12, 0x16, 0x0a, 0x06, 0x70,
	0x72, 0x65, 0x66, 0x69, 0x78, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x06, 0x70, 0x72, 0x65,
	0x66, 0x69, 0x78, 0x2a, 0x3b, 0x0a, 0x0b, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x5f, 0x43, 0x4f, 0x4e,
	0x53, 0x54, 0x12, 0x19, 0x0a, 0x15, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x5f, 0x43, 0x4f, 0x4e, 0x53,
	0x54, 0x5f, 0x55, 0x4e, 0x49, 0x56, 0x45, 0x52, 0x53, 0x41, 0x4c, 0x10, 0x00, 0x12, 0x11, 0x0a,
	0x0d, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x5f, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x10, 0x01,
	0x42, 0x54, 0x0a, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65,
	0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x50,
	0x01, 0x5a, 0x15, 0x67, 0x61, 0x6d, 0x65, 0x6c, 0x69, 0x62, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x62, 0x75, 0x66, 0x2f, 0x67, 0x70, 0x72, 0x6f, 0xaa, 0x02, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63,
	0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72,
	0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_robot_test_internal_proto_rawDescOnce sync.Once
	file_robot_test_internal_proto_rawDescData = file_robot_test_internal_proto_rawDesc
)

func file_robot_test_internal_proto_rawDescGZIP() []byte {
	file_robot_test_internal_proto_rawDescOnce.Do(func() {
		file_robot_test_internal_proto_rawDescData = protoimpl.X.CompressGZIP(file_robot_test_internal_proto_rawDescData)
	})
	return file_robot_test_internal_proto_rawDescData
}

var file_robot_test_internal_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_robot_test_internal_proto_msgTypes = make([]protoimpl.MessageInfo, 10)
var file_robot_test_internal_proto_goTypes = []interface{}{
	(ROBOT_CONST)(0),               // 0: PRO.ROBOT_CONST
	(*Robot_Config_Req)(nil),       // 1: PRO.Robot_Config_Req
	(*Robot_Config_Ack)(nil),       // 2: PRO.Robot_Config_Ack
	(*Robot_Start_Ack)(nil),        // 3: PRO.Robot_Start_Ack
	(*Robot_Stop_Ack)(nil),         // 4: PRO.Robot_Stop_Ack
	(*Robot_RecordReq)(nil),        // 5: PRO.Robot_Record_req
	(*Robot_StartRecordReq)(nil),   // 6: PRO.Robot_StartRecord_req
	(*Robot_EndRecordReq)(nil),     // 7: PRO.Robot_EndRecord_req
	(*Robot_UploadLogsNtf)(nil),    // 8: PRO.Robot_UploadLogs_ntf
	(*Robot_ClearLogsNtf)(nil),     // 9: PRO.Robot_ClearLogs_ntf
	(*Robot_UserPrefixChgNtf)(nil), // 10: PRO.Robot_UserPrefixChg_ntf
}
var file_robot_test_internal_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_robot_test_internal_proto_init() }
func file_robot_test_internal_proto_init() {
	if File_robot_test_internal_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_robot_test_internal_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_Config_Req); i {
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
		file_robot_test_internal_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_Config_Ack); i {
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
		file_robot_test_internal_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_Start_Ack); i {
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
		file_robot_test_internal_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_Stop_Ack); i {
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
		file_robot_test_internal_proto_msgTypes[4].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_RecordReq); i {
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
		file_robot_test_internal_proto_msgTypes[5].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_StartRecordReq); i {
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
		file_robot_test_internal_proto_msgTypes[6].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_EndRecordReq); i {
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
		file_robot_test_internal_proto_msgTypes[7].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_UploadLogsNtf); i {
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
		file_robot_test_internal_proto_msgTypes[8].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_ClearLogsNtf); i {
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
		file_robot_test_internal_proto_msgTypes[9].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*Robot_UserPrefixChgNtf); i {
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
	file_robot_test_internal_proto_msgTypes[1].OneofWrappers = []interface{}{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_robot_test_internal_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   10,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_robot_test_internal_proto_goTypes,
		DependencyIndexes: file_robot_test_internal_proto_depIdxs,
		EnumInfos:         file_robot_test_internal_proto_enumTypes,
		MessageInfos:      file_robot_test_internal_proto_msgTypes,
	}.Build()
	File_robot_test_internal_proto = out.File
	file_robot_test_internal_proto_rawDesc = nil
	file_robot_test_internal_proto_goTypes = nil
	file_robot_test_internal_proto_depIdxs = nil
}
