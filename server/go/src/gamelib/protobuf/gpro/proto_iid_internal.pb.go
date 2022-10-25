// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.0
// source: proto_iid_internal.proto

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

// 对应的协议编号
type ERK_PROTYPE int32

const (
	ERK_PROTYPE_UNIVERSAL_1 ERK_PROTYPE = 0
	//绑定请求
	ERK_PROTYPE_ERK_EUREKABIND_REQ         ERK_PROTYPE = 1
	ERK_PROTYPE_ERK_EUREKABIND_ACK         ERK_PROTYPE = 2
	ERK_PROTYPE_ERK_EUREKAUPDATE_NTF       ERK_PROTYPE = 3
	ERK_PROTYPE_ERK_EUREKA_SYNC            ERK_PROTYPE = 4
	ERK_PROTYPE_ERK_SERVICEREGIST_REQ      ERK_PROTYPE = 10
	ERK_PROTYPE_ERK_SERVICEREGIST_ACK      ERK_PROTYPE = 11
	ERK_PROTYPE_ERK_SERVICEREGIST_CONFIRM  ERK_PROTYPE = 12
	ERK_PROTYPE_ERK_SERVICEBIND_REQ        ERK_PROTYPE = 13
	ERK_PROTYPE_ERK_SERVICEBIND_ACK        ERK_PROTYPE = 14
	ERK_PROTYPE_ERK_SERVICESUBSCRIBE_REQ   ERK_PROTYPE = 15
	ERK_PROTYPE_ERK_SERVICESUBSCRIBE_ACK   ERK_PROTYPE = 16
	ERK_PROTYPE_ERK_SERVICESHUTDOWN_NTF    ERK_PROTYPE = 17
	ERK_PROTYPE_SVR_LIVETICK_NTF           ERK_PROTYPE = 49
	ERK_PROTYPE_SVR_SERVICEBINDSERVICE_REQ ERK_PROTYPE = 50
	ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK ERK_PROTYPE = 51
	//资源服务
	ERK_PROTYPE_RES_USERSLOT_REQ      ERK_PROTYPE = 100
	ERK_PROTYPE_RES_USERSLOT_ACK      ERK_PROTYPE = 101
	ERK_PROTYPE_RES_PROXYUSERSLOT_REQ ERK_PROTYPE = 102
	ERK_PROTYPE_RES_PROXYUSERSLOT_ACK ERK_PROTYPE = 103
	ERK_PROTYPE_RES_SYNCGATESLOT_NTF  ERK_PROTYPE = 104
)

// Enum value maps for ERK_PROTYPE.
var (
	ERK_PROTYPE_name = map[int32]string{
		0:   "UNIVERSAL_1",
		1:   "ERK_EUREKABIND_REQ",
		2:   "ERK_EUREKABIND_ACK",
		3:   "ERK_EUREKAUPDATE_NTF",
		4:   "ERK_EUREKA_SYNC",
		10:  "ERK_SERVICEREGIST_REQ",
		11:  "ERK_SERVICEREGIST_ACK",
		12:  "ERK_SERVICEREGIST_CONFIRM",
		13:  "ERK_SERVICEBIND_REQ",
		14:  "ERK_SERVICEBIND_ACK",
		15:  "ERK_SERVICESUBSCRIBE_REQ",
		16:  "ERK_SERVICESUBSCRIBE_ACK",
		17:  "ERK_SERVICESHUTDOWN_NTF",
		49:  "SVR_LIVETICK_NTF",
		50:  "SVR_SERVICEBINDSERVICE_REQ",
		51:  "SVR_SERVICEBINDSERVICE_ACK",
		100: "RES_USERSLOT_REQ",
		101: "RES_USERSLOT_ACK",
		102: "RES_PROXYUSERSLOT_REQ",
		103: "RES_PROXYUSERSLOT_ACK",
		104: "RES_SYNCGATESLOT_NTF",
	}
	ERK_PROTYPE_value = map[string]int32{
		"UNIVERSAL_1":                0,
		"ERK_EUREKABIND_REQ":         1,
		"ERK_EUREKABIND_ACK":         2,
		"ERK_EUREKAUPDATE_NTF":       3,
		"ERK_EUREKA_SYNC":            4,
		"ERK_SERVICEREGIST_REQ":      10,
		"ERK_SERVICEREGIST_ACK":      11,
		"ERK_SERVICEREGIST_CONFIRM":  12,
		"ERK_SERVICEBIND_REQ":        13,
		"ERK_SERVICEBIND_ACK":        14,
		"ERK_SERVICESUBSCRIBE_REQ":   15,
		"ERK_SERVICESUBSCRIBE_ACK":   16,
		"ERK_SERVICESHUTDOWN_NTF":    17,
		"SVR_LIVETICK_NTF":           49,
		"SVR_SERVICEBINDSERVICE_REQ": 50,
		"SVR_SERVICEBINDSERVICE_ACK": 51,
		"RES_USERSLOT_REQ":           100,
		"RES_USERSLOT_ACK":           101,
		"RES_PROXYUSERSLOT_REQ":      102,
		"RES_PROXYUSERSLOT_ACK":      103,
		"RES_SYNCGATESLOT_NTF":       104,
	}
)

func (x ERK_PROTYPE) Enum() *ERK_PROTYPE {
	p := new(ERK_PROTYPE)
	*p = x
	return p
}

func (x ERK_PROTYPE) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ERK_PROTYPE) Descriptor() protoreflect.EnumDescriptor {
	return file_proto_iid_internal_proto_enumTypes[0].Descriptor()
}

func (ERK_PROTYPE) Type() protoreflect.EnumType {
	return &file_proto_iid_internal_proto_enumTypes[0]
}

func (x ERK_PROTYPE) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ERK_PROTYPE.Descriptor instead.
func (ERK_PROTYPE) EnumDescriptor() ([]byte, []int) {
	return file_proto_iid_internal_proto_rawDescGZIP(), []int{0}
}

//gatehome
type GATEHOME_PROTYPE int32

const (
	GATEHOME_PROTYPE_UNIVERSAL_200      GATEHOME_PROTYPE = 0
	GATEHOME_PROTYPE_GHS_USERINIT_REQ   GATEHOME_PROTYPE = 201
	GATEHOME_PROTYPE_GHS_USERLOGOUT_NTF GATEHOME_PROTYPE = 202
	GATEHOME_PROTYPE_GHS_ROLEDETAIL_ASK GATEHOME_PROTYPE = 203
)

// Enum value maps for GATEHOME_PROTYPE.
var (
	GATEHOME_PROTYPE_name = map[int32]string{
		0:   "UNIVERSAL_200",
		201: "GHS_USERINIT_REQ",
		202: "GHS_USERLOGOUT_NTF",
		203: "GHS_ROLEDETAIL_ASK",
	}
	GATEHOME_PROTYPE_value = map[string]int32{
		"UNIVERSAL_200":      0,
		"GHS_USERINIT_REQ":   201,
		"GHS_USERLOGOUT_NTF": 202,
		"GHS_ROLEDETAIL_ASK": 203,
	}
)

func (x GATEHOME_PROTYPE) Enum() *GATEHOME_PROTYPE {
	p := new(GATEHOME_PROTYPE)
	*p = x
	return p
}

func (x GATEHOME_PROTYPE) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (GATEHOME_PROTYPE) Descriptor() protoreflect.EnumDescriptor {
	return file_proto_iid_internal_proto_enumTypes[1].Descriptor()
}

func (GATEHOME_PROTYPE) Type() protoreflect.EnumType {
	return &file_proto_iid_internal_proto_enumTypes[1]
}

func (x GATEHOME_PROTYPE) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use GATEHOME_PROTYPE.Descriptor instead.
func (GATEHOME_PROTYPE) EnumDescriptor() ([]byte, []int) {
	return file_proto_iid_internal_proto_rawDescGZIP(), []int{1}
}

type ROBOTTEST_PROTYPE int32

const (
	ROBOTTEST_PROTYPE_UNIVERSAL_300               ROBOTTEST_PROTYPE = 0
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_CONFIG_REQ ROBOTTEST_PROTYPE = 301
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_CONFIG_ACK ROBOTTEST_PROTYPE = 302
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_START_ACK  ROBOTTEST_PROTYPE = 303
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_STOP_ACK   ROBOTTEST_PROTYPE = 304
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_STATE_RPT  ROBOTTEST_PROTYPE = 305
	ROBOTTEST_PROTYPE_ROBOTOTEST_ROBOT_STATE_ACK  ROBOTTEST_PROTYPE = 306
)

// Enum value maps for ROBOTTEST_PROTYPE.
var (
	ROBOTTEST_PROTYPE_name = map[int32]string{
		0:   "UNIVERSAL_300",
		301: "ROBOTOTEST_ROBOT_CONFIG_REQ",
		302: "ROBOTOTEST_ROBOT_CONFIG_ACK",
		303: "ROBOTOTEST_ROBOT_START_ACK",
		304: "ROBOTOTEST_ROBOT_STOP_ACK",
		305: "ROBOTOTEST_ROBOT_STATE_RPT",
		306: "ROBOTOTEST_ROBOT_STATE_ACK",
	}
	ROBOTTEST_PROTYPE_value = map[string]int32{
		"UNIVERSAL_300":               0,
		"ROBOTOTEST_ROBOT_CONFIG_REQ": 301,
		"ROBOTOTEST_ROBOT_CONFIG_ACK": 302,
		"ROBOTOTEST_ROBOT_START_ACK":  303,
		"ROBOTOTEST_ROBOT_STOP_ACK":   304,
		"ROBOTOTEST_ROBOT_STATE_RPT":  305,
		"ROBOTOTEST_ROBOT_STATE_ACK":  306,
	}
)

func (x ROBOTTEST_PROTYPE) Enum() *ROBOTTEST_PROTYPE {
	p := new(ROBOTTEST_PROTYPE)
	*p = x
	return p
}

func (x ROBOTTEST_PROTYPE) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ROBOTTEST_PROTYPE) Descriptor() protoreflect.EnumDescriptor {
	return file_proto_iid_internal_proto_enumTypes[2].Descriptor()
}

func (ROBOTTEST_PROTYPE) Type() protoreflect.EnumType {
	return &file_proto_iid_internal_proto_enumTypes[2]
}

func (x ROBOTTEST_PROTYPE) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ROBOTTEST_PROTYPE.Descriptor instead.
func (ROBOTTEST_PROTYPE) EnumDescriptor() ([]byte, []int) {
	return file_proto_iid_internal_proto_rawDescGZIP(), []int{2}
}

var File_proto_iid_internal_proto protoreflect.FileDescriptor

var file_proto_iid_internal_proto_rawDesc = []byte{
	0x0a, 0x18, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x5f, 0x69, 0x69, 0x64, 0x5f, 0x69, 0x6e, 0x74, 0x65,
	0x72, 0x6e, 0x61, 0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x03, 0x50, 0x52, 0x4f, 0x2a,
	0xaf, 0x04, 0x0a, 0x0b, 0x45, 0x52, 0x4b, 0x5f, 0x50, 0x52, 0x4f, 0x54, 0x59, 0x50, 0x45, 0x12,
	0x0f, 0x0a, 0x0b, 0x55, 0x4e, 0x49, 0x56, 0x45, 0x52, 0x53, 0x41, 0x4c, 0x5f, 0x31, 0x10, 0x00,
	0x12, 0x16, 0x0a, 0x12, 0x45, 0x52, 0x4b, 0x5f, 0x45, 0x55, 0x52, 0x45, 0x4b, 0x41, 0x42, 0x49,
	0x4e, 0x44, 0x5f, 0x52, 0x45, 0x51, 0x10, 0x01, 0x12, 0x16, 0x0a, 0x12, 0x45, 0x52, 0x4b, 0x5f,
	0x45, 0x55, 0x52, 0x45, 0x4b, 0x41, 0x42, 0x49, 0x4e, 0x44, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0x02,
	0x12, 0x18, 0x0a, 0x14, 0x45, 0x52, 0x4b, 0x5f, 0x45, 0x55, 0x52, 0x45, 0x4b, 0x41, 0x55, 0x50,
	0x44, 0x41, 0x54, 0x45, 0x5f, 0x4e, 0x54, 0x46, 0x10, 0x03, 0x12, 0x13, 0x0a, 0x0f, 0x45, 0x52,
	0x4b, 0x5f, 0x45, 0x55, 0x52, 0x45, 0x4b, 0x41, 0x5f, 0x53, 0x59, 0x4e, 0x43, 0x10, 0x04, 0x12,
	0x19, 0x0a, 0x15, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x52, 0x45,
	0x47, 0x49, 0x53, 0x54, 0x5f, 0x52, 0x45, 0x51, 0x10, 0x0a, 0x12, 0x19, 0x0a, 0x15, 0x45, 0x52,
	0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x52, 0x45, 0x47, 0x49, 0x53, 0x54, 0x5f,
	0x41, 0x43, 0x4b, 0x10, 0x0b, 0x12, 0x1d, 0x0a, 0x19, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52,
	0x56, 0x49, 0x43, 0x45, 0x52, 0x45, 0x47, 0x49, 0x53, 0x54, 0x5f, 0x43, 0x4f, 0x4e, 0x46, 0x49,
	0x52, 0x4d, 0x10, 0x0c, 0x12, 0x17, 0x0a, 0x13, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56,
	0x49, 0x43, 0x45, 0x42, 0x49, 0x4e, 0x44, 0x5f, 0x52, 0x45, 0x51, 0x10, 0x0d, 0x12, 0x17, 0x0a,
	0x13, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x42, 0x49, 0x4e, 0x44,
	0x5f, 0x41, 0x43, 0x4b, 0x10, 0x0e, 0x12, 0x1c, 0x0a, 0x18, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45,
	0x52, 0x56, 0x49, 0x43, 0x45, 0x53, 0x55, 0x42, 0x53, 0x43, 0x52, 0x49, 0x42, 0x45, 0x5f, 0x52,
	0x45, 0x51, 0x10, 0x0f, 0x12, 0x1c, 0x0a, 0x18, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56,
	0x49, 0x43, 0x45, 0x53, 0x55, 0x42, 0x53, 0x43, 0x52, 0x49, 0x42, 0x45, 0x5f, 0x41, 0x43, 0x4b,
	0x10, 0x10, 0x12, 0x1b, 0x0a, 0x17, 0x45, 0x52, 0x4b, 0x5f, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43,
	0x45, 0x53, 0x48, 0x55, 0x54, 0x44, 0x4f, 0x57, 0x4e, 0x5f, 0x4e, 0x54, 0x46, 0x10, 0x11, 0x12,
	0x14, 0x0a, 0x10, 0x53, 0x56, 0x52, 0x5f, 0x4c, 0x49, 0x56, 0x45, 0x54, 0x49, 0x43, 0x4b, 0x5f,
	0x4e, 0x54, 0x46, 0x10, 0x31, 0x12, 0x1e, 0x0a, 0x1a, 0x53, 0x56, 0x52, 0x5f, 0x53, 0x45, 0x52,
	0x56, 0x49, 0x43, 0x45, 0x42, 0x49, 0x4e, 0x44, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x5f,
	0x52, 0x45, 0x51, 0x10, 0x32, 0x12, 0x1e, 0x0a, 0x1a, 0x53, 0x56, 0x52, 0x5f, 0x53, 0x45, 0x52,
	0x56, 0x49, 0x43, 0x45, 0x42, 0x49, 0x4e, 0x44, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x5f,
	0x41, 0x43, 0x4b, 0x10, 0x33, 0x12, 0x14, 0x0a, 0x10, 0x52, 0x45, 0x53, 0x5f, 0x55, 0x53, 0x45,
	0x52, 0x53, 0x4c, 0x4f, 0x54, 0x5f, 0x52, 0x45, 0x51, 0x10, 0x64, 0x12, 0x14, 0x0a, 0x10, 0x52,
	0x45, 0x53, 0x5f, 0x55, 0x53, 0x45, 0x52, 0x53, 0x4c, 0x4f, 0x54, 0x5f, 0x41, 0x43, 0x4b, 0x10,
	0x65, 0x12, 0x19, 0x0a, 0x15, 0x52, 0x45, 0x53, 0x5f, 0x50, 0x52, 0x4f, 0x58, 0x59, 0x55, 0x53,
	0x45, 0x52, 0x53, 0x4c, 0x4f, 0x54, 0x5f, 0x52, 0x45, 0x51, 0x10, 0x66, 0x12, 0x19, 0x0a, 0x15,
	0x52, 0x45, 0x53, 0x5f, 0x50, 0x52, 0x4f, 0x58, 0x59, 0x55, 0x53, 0x45, 0x52, 0x53, 0x4c, 0x4f,
	0x54, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0x67, 0x12, 0x18, 0x0a, 0x14, 0x52, 0x45, 0x53, 0x5f, 0x53,
	0x59, 0x4e, 0x43, 0x47, 0x41, 0x54, 0x45, 0x53, 0x4c, 0x4f, 0x54, 0x5f, 0x4e, 0x54, 0x46, 0x10,
	0x68, 0x2a, 0x6e, 0x0a, 0x10, 0x47, 0x41, 0x54, 0x45, 0x48, 0x4f, 0x4d, 0x45, 0x5f, 0x50, 0x52,
	0x4f, 0x54, 0x59, 0x50, 0x45, 0x12, 0x11, 0x0a, 0x0d, 0x55, 0x4e, 0x49, 0x56, 0x45, 0x52, 0x53,
	0x41, 0x4c, 0x5f, 0x32, 0x30, 0x30, 0x10, 0x00, 0x12, 0x15, 0x0a, 0x10, 0x47, 0x48, 0x53, 0x5f,
	0x55, 0x53, 0x45, 0x52, 0x49, 0x4e, 0x49, 0x54, 0x5f, 0x52, 0x45, 0x51, 0x10, 0xc9, 0x01, 0x12,
	0x17, 0x0a, 0x12, 0x47, 0x48, 0x53, 0x5f, 0x55, 0x53, 0x45, 0x52, 0x4c, 0x4f, 0x47, 0x4f, 0x55,
	0x54, 0x5f, 0x4e, 0x54, 0x46, 0x10, 0xca, 0x01, 0x12, 0x17, 0x0a, 0x12, 0x47, 0x48, 0x53, 0x5f,
	0x52, 0x4f, 0x4c, 0x45, 0x44, 0x45, 0x54, 0x41, 0x49, 0x4c, 0x5f, 0x41, 0x53, 0x4b, 0x10, 0xcb,
	0x01, 0x2a, 0xed, 0x01, 0x0a, 0x11, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x54, 0x45, 0x53, 0x54, 0x5f,
	0x50, 0x52, 0x4f, 0x54, 0x59, 0x50, 0x45, 0x12, 0x11, 0x0a, 0x0d, 0x55, 0x4e, 0x49, 0x56, 0x45,
	0x52, 0x53, 0x41, 0x4c, 0x5f, 0x33, 0x30, 0x30, 0x10, 0x00, 0x12, 0x20, 0x0a, 0x1b, 0x52, 0x4f,
	0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x5f, 0x43,
	0x4f, 0x4e, 0x46, 0x49, 0x47, 0x5f, 0x52, 0x45, 0x51, 0x10, 0xad, 0x02, 0x12, 0x20, 0x0a, 0x1b,
	0x52, 0x4f, 0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52, 0x4f, 0x42, 0x4f, 0x54,
	0x5f, 0x43, 0x4f, 0x4e, 0x46, 0x49, 0x47, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0xae, 0x02, 0x12, 0x1f,
	0x0a, 0x1a, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52, 0x4f, 0x42,
	0x4f, 0x54, 0x5f, 0x53, 0x54, 0x41, 0x52, 0x54, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0xaf, 0x02, 0x12,
	0x1e, 0x0a, 0x19, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52, 0x4f,
	0x42, 0x4f, 0x54, 0x5f, 0x53, 0x54, 0x4f, 0x50, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0xb0, 0x02, 0x12,
	0x1f, 0x0a, 0x1a, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52, 0x4f,
	0x42, 0x4f, 0x54, 0x5f, 0x53, 0x54, 0x41, 0x54, 0x45, 0x5f, 0x52, 0x50, 0x54, 0x10, 0xb1, 0x02,
	0x12, 0x1f, 0x0a, 0x1a, 0x52, 0x4f, 0x42, 0x4f, 0x54, 0x4f, 0x54, 0x45, 0x53, 0x54, 0x5f, 0x52,
	0x4f, 0x42, 0x4f, 0x54, 0x5f, 0x53, 0x54, 0x41, 0x54, 0x45, 0x5f, 0x41, 0x43, 0x4b, 0x10, 0xb2,
	0x02, 0x42, 0x54, 0x0a, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69,
	0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f,
	0x50, 0x01, 0x5a, 0x15, 0x67, 0x61, 0x6d, 0x65, 0x6c, 0x69, 0x62, 0x2f, 0x70, 0x72, 0x6f, 0x74,
	0x6f, 0x62, 0x75, 0x66, 0x2f, 0x67, 0x70, 0x72, 0x6f, 0xaa, 0x02, 0x1b, 0x63, 0x6f, 0x6d, 0x2e,
	0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f,
	0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_proto_iid_internal_proto_rawDescOnce sync.Once
	file_proto_iid_internal_proto_rawDescData = file_proto_iid_internal_proto_rawDesc
)

func file_proto_iid_internal_proto_rawDescGZIP() []byte {
	file_proto_iid_internal_proto_rawDescOnce.Do(func() {
		file_proto_iid_internal_proto_rawDescData = protoimpl.X.CompressGZIP(file_proto_iid_internal_proto_rawDescData)
	})
	return file_proto_iid_internal_proto_rawDescData
}

var file_proto_iid_internal_proto_enumTypes = make([]protoimpl.EnumInfo, 3)
var file_proto_iid_internal_proto_goTypes = []interface{}{
	(ERK_PROTYPE)(0),       // 0: PRO.ERK_PROTYPE
	(GATEHOME_PROTYPE)(0),  // 1: PRO.GATEHOME_PROTYPE
	(ROBOTTEST_PROTYPE)(0), // 2: PRO.ROBOTTEST_PROTYPE
}
var file_proto_iid_internal_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_proto_iid_internal_proto_init() }
func file_proto_iid_internal_proto_init() {
	if File_proto_iid_internal_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_proto_iid_internal_proto_rawDesc,
			NumEnums:      3,
			NumMessages:   0,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_proto_iid_internal_proto_goTypes,
		DependencyIndexes: file_proto_iid_internal_proto_depIdxs,
		EnumInfos:         file_proto_iid_internal_proto_enumTypes,
	}.Build()
	File_proto_iid_internal_proto = out.File
	file_proto_iid_internal_proto_rawDesc = nil
	file_proto_iid_internal_proto_goTypes = nil
	file_proto_iid_internal_proto_depIdxs = nil
}
