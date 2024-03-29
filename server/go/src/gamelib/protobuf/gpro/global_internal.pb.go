// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.0
// source: global_internal.proto

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

//系统邮件信息
type MailSystemItem struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Iid       int64  `protobuf:"varint,1,opt,name=iid,proto3" json:"iid,omitempty"`
	SenderIid int64  `protobuf:"varint,2,opt,name=sender_iid,json=senderIid,proto3" json:"sender_iid,omitempty"`
	Title     string `protobuf:"bytes,3,opt,name=title,proto3" json:"title,omitempty"`
	Contents  string `protobuf:"bytes,4,opt,name=contents,proto3" json:"contents,omitempty"`
	//0:无附件 1:有附件
	Attachment int32 `protobuf:"varint,5,opt,name=attachment,proto3" json:"attachment,omitempty"`
	//附件json字符串
	Attachinfo string `protobuf:"bytes,6,opt,name=attachinfo,proto3" json:"attachinfo,omitempty"`
	//创建时间second
	Createtime int32 `protobuf:"varint,7,opt,name=createtime,proto3" json:"createtime,omitempty"`
}

func (x *MailSystemItem) Reset() {
	*x = MailSystemItem{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MailSystemItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MailSystemItem) ProtoMessage() {}

func (x *MailSystemItem) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MailSystemItem.ProtoReflect.Descriptor instead.
func (*MailSystemItem) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{0}
}

func (x *MailSystemItem) GetIid() int64 {
	if x != nil {
		return x.Iid
	}
	return 0
}

func (x *MailSystemItem) GetSenderIid() int64 {
	if x != nil {
		return x.SenderIid
	}
	return 0
}

func (x *MailSystemItem) GetTitle() string {
	if x != nil {
		return x.Title
	}
	return ""
}

func (x *MailSystemItem) GetContents() string {
	if x != nil {
		return x.Contents
	}
	return ""
}

func (x *MailSystemItem) GetAttachment() int32 {
	if x != nil {
		return x.Attachment
	}
	return 0
}

func (x *MailSystemItem) GetAttachinfo() string {
	if x != nil {
		return x.Attachinfo
	}
	return ""
}

func (x *MailSystemItem) GetCreatetime() int32 {
	if x != nil {
		return x.Createtime
	}
	return 0
}

type MailSystemItems struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Mails []*MailSystemItem `protobuf:"bytes,1,rep,name=mails,proto3" json:"mails,omitempty"`
}

func (x *MailSystemItems) Reset() {
	*x = MailSystemItems{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MailSystemItems) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MailSystemItems) ProtoMessage() {}

func (x *MailSystemItems) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MailSystemItems.ProtoReflect.Descriptor instead.
func (*MailSystemItems) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{1}
}

func (x *MailSystemItems) GetMails() []*MailSystemItem {
	if x != nil {
		return x.Mails
	}
	return nil
}

//邮件信息
type MailNormalItem struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Iid int64 `protobuf:"varint,1,opt,name=iid,proto3" json:"iid,omitempty"`
	//0系统邮件 1工会邮件
	Stype       int32 `protobuf:"varint,2,opt,name=stype,proto3" json:"stype,omitempty"`
	SenderIid   int64 `protobuf:"varint,3,opt,name=sender_iid,json=senderIid,proto3" json:"sender_iid,omitempty"`
	ReceiverIid int64 `protobuf:"varint,4,opt,name=receiver_iid,json=receiverIid,proto3" json:"receiver_iid,omitempty"`
	//0:无附件 1:有附件
	Attachment int32 `protobuf:"varint,5,opt,name=attachment,proto3" json:"attachment,omitempty"`
	//附件json字符串
	Attachinfo string `protobuf:"bytes,6,opt,name=attachinfo,proto3" json:"attachinfo,omitempty"`
	Title      string `protobuf:"bytes,7,opt,name=title,proto3" json:"title,omitempty"`
	Contents   string `protobuf:"bytes,8,opt,name=contents,proto3" json:"contents,omitempty"`
	//创建时间second
	Createtime int32 `protobuf:"varint,9,opt,name=createtime,proto3" json:"createtime,omitempty"`
	Readtime   int32 `protobuf:"varint,10,opt,name=readtime,proto3" json:"readtime,omitempty"`
}

func (x *MailNormalItem) Reset() {
	*x = MailNormalItem{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MailNormalItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MailNormalItem) ProtoMessage() {}

func (x *MailNormalItem) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MailNormalItem.ProtoReflect.Descriptor instead.
func (*MailNormalItem) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{2}
}

func (x *MailNormalItem) GetIid() int64 {
	if x != nil {
		return x.Iid
	}
	return 0
}

func (x *MailNormalItem) GetStype() int32 {
	if x != nil {
		return x.Stype
	}
	return 0
}

func (x *MailNormalItem) GetSenderIid() int64 {
	if x != nil {
		return x.SenderIid
	}
	return 0
}

func (x *MailNormalItem) GetReceiverIid() int64 {
	if x != nil {
		return x.ReceiverIid
	}
	return 0
}

func (x *MailNormalItem) GetAttachment() int32 {
	if x != nil {
		return x.Attachment
	}
	return 0
}

func (x *MailNormalItem) GetAttachinfo() string {
	if x != nil {
		return x.Attachinfo
	}
	return ""
}

func (x *MailNormalItem) GetTitle() string {
	if x != nil {
		return x.Title
	}
	return ""
}

func (x *MailNormalItem) GetContents() string {
	if x != nil {
		return x.Contents
	}
	return ""
}

func (x *MailNormalItem) GetCreatetime() int32 {
	if x != nil {
		return x.Createtime
	}
	return 0
}

func (x *MailNormalItem) GetReadtime() int32 {
	if x != nil {
		return x.Readtime
	}
	return 0
}

type MailNormalItems struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	//从小到大排列
	Mails []*MailNormalItem `protobuf:"bytes,1,rep,name=mails,proto3" json:"mails,omitempty"`
}

func (x *MailNormalItems) Reset() {
	*x = MailNormalItems{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *MailNormalItems) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*MailNormalItems) ProtoMessage() {}

func (x *MailNormalItems) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use MailNormalItems.ProtoReflect.Descriptor instead.
func (*MailNormalItems) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{3}
}

func (x *MailNormalItems) GetMails() []*MailNormalItem {
	if x != nil {
		return x.Mails
	}
	return nil
}

type FriendInviteItem struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Iid int64 `protobuf:"varint,1,opt,name=iid,proto3" json:"iid,omitempty"`
	//邀请人
	FromIid int64 `protobuf:"varint,2,opt,name=from_iid,json=fromIid,proto3" json:"from_iid,omitempty"`
	//邀请人昵称
	Nickname string `protobuf:"bytes,3,opt,name=nickname,proto3" json:"nickname,omitempty"`
	//邀请时间
	Invitetime int32 `protobuf:"varint,4,opt,name=invitetime,proto3" json:"invitetime,omitempty"`
}

func (x *FriendInviteItem) Reset() {
	*x = FriendInviteItem{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[4]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *FriendInviteItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*FriendInviteItem) ProtoMessage() {}

func (x *FriendInviteItem) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[4]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use FriendInviteItem.ProtoReflect.Descriptor instead.
func (*FriendInviteItem) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{4}
}

func (x *FriendInviteItem) GetIid() int64 {
	if x != nil {
		return x.Iid
	}
	return 0
}

func (x *FriendInviteItem) GetFromIid() int64 {
	if x != nil {
		return x.FromIid
	}
	return 0
}

func (x *FriendInviteItem) GetNickname() string {
	if x != nil {
		return x.Nickname
	}
	return ""
}

func (x *FriendInviteItem) GetInvitetime() int32 {
	if x != nil {
		return x.Invitetime
	}
	return 0
}

type FriendRelation struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Iid       int64  `protobuf:"varint,1,opt,name=iid,proto3" json:"iid,omitempty"`
	Friendiid int64  `protobuf:"varint,2,opt,name=friendiid,proto3" json:"friendiid,omitempty"`
	Nickname  string `protobuf:"bytes,3,opt,name=nickname,proto3" json:"nickname,omitempty"`
	//最后更新时间
	Online bool `protobuf:"varint,4,opt,name=online,proto3" json:"online,omitempty"`
}

func (x *FriendRelation) Reset() {
	*x = FriendRelation{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[5]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *FriendRelation) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*FriendRelation) ProtoMessage() {}

func (x *FriendRelation) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[5]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use FriendRelation.ProtoReflect.Descriptor instead.
func (*FriendRelation) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{5}
}

func (x *FriendRelation) GetIid() int64 {
	if x != nil {
		return x.Iid
	}
	return 0
}

func (x *FriendRelation) GetFriendiid() int64 {
	if x != nil {
		return x.Friendiid
	}
	return 0
}

func (x *FriendRelation) GetNickname() string {
	if x != nil {
		return x.Nickname
	}
	return ""
}

func (x *FriendRelation) GetOnline() bool {
	if x != nil {
		return x.Online
	}
	return false
}

//好友系统的用户数据
type FriendUserInfo struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Roleiid int64 `protobuf:"varint,1,opt,name=roleiid,proto3" json:"roleiid,omitempty"`
}

func (x *FriendUserInfo) Reset() {
	*x = FriendUserInfo{}
	if protoimpl.UnsafeEnabled {
		mi := &file_global_internal_proto_msgTypes[6]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *FriendUserInfo) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*FriendUserInfo) ProtoMessage() {}

func (x *FriendUserInfo) ProtoReflect() protoreflect.Message {
	mi := &file_global_internal_proto_msgTypes[6]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use FriendUserInfo.ProtoReflect.Descriptor instead.
func (*FriendUserInfo) Descriptor() ([]byte, []int) {
	return file_global_internal_proto_rawDescGZIP(), []int{6}
}

func (x *FriendUserInfo) GetRoleiid() int64 {
	if x != nil {
		return x.Roleiid
	}
	return 0
}

var File_global_internal_proto protoreflect.FileDescriptor

var file_global_internal_proto_rawDesc = []byte{
	0x0a, 0x15, 0x67, 0x6c, 0x6f, 0x62, 0x61, 0x6c, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61,
	0x6c, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x03, 0x50, 0x52, 0x4f, 0x22, 0xd3, 0x01, 0x0a,
	0x0e, 0x4d, 0x61, 0x69, 0x6c, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x49, 0x74, 0x65, 0x6d, 0x12,
	0x10, 0x0a, 0x03, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x03, 0x69, 0x69,
	0x64, 0x12, 0x1d, 0x0a, 0x0a, 0x73, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x5f, 0x69, 0x69, 0x64, 0x18,
	0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x09, 0x73, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x49, 0x69, 0x64,
	0x12, 0x14, 0x0a, 0x05, 0x74, 0x69, 0x74, 0x6c, 0x65, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52,
	0x05, 0x74, 0x69, 0x74, 0x6c, 0x65, 0x12, 0x1a, 0x0a, 0x08, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e,
	0x74, 0x73, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e,
	0x74, 0x73, 0x12, 0x1e, 0x0a, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6d, 0x65, 0x6e, 0x74,
	0x18, 0x05, 0x20, 0x01, 0x28, 0x05, 0x52, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6d, 0x65,
	0x6e, 0x74, 0x12, 0x1e, 0x0a, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x69, 0x6e, 0x66, 0x6f,
	0x18, 0x06, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x69, 0x6e,
	0x66, 0x6f, 0x12, 0x1e, 0x0a, 0x0a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6d, 0x65,
	0x18, 0x07, 0x20, 0x01, 0x28, 0x05, 0x52, 0x0a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x74, 0x69,
	0x6d, 0x65, 0x22, 0x3c, 0x0a, 0x0f, 0x4d, 0x61, 0x69, 0x6c, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d,
	0x49, 0x74, 0x65, 0x6d, 0x73, 0x12, 0x29, 0x0a, 0x05, 0x6d, 0x61, 0x69, 0x6c, 0x73, 0x18, 0x01,
	0x20, 0x03, 0x28, 0x0b, 0x32, 0x13, 0x2e, 0x50, 0x52, 0x4f, 0x2e, 0x4d, 0x61, 0x69, 0x6c, 0x53,
	0x79, 0x73, 0x74, 0x65, 0x6d, 0x49, 0x74, 0x65, 0x6d, 0x52, 0x05, 0x6d, 0x61, 0x69, 0x6c, 0x73,
	0x22, 0xa8, 0x02, 0x0a, 0x0e, 0x4d, 0x61, 0x69, 0x6c, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x49,
	0x74, 0x65, 0x6d, 0x12, 0x10, 0x0a, 0x03, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03,
	0x52, 0x03, 0x69, 0x69, 0x64, 0x12, 0x14, 0x0a, 0x05, 0x73, 0x74, 0x79, 0x70, 0x65, 0x18, 0x02,
	0x20, 0x01, 0x28, 0x05, 0x52, 0x05, 0x73, 0x74, 0x79, 0x70, 0x65, 0x12, 0x1d, 0x0a, 0x0a, 0x73,
	0x65, 0x6e, 0x64, 0x65, 0x72, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x03, 0x52,
	0x09, 0x73, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x49, 0x69, 0x64, 0x12, 0x21, 0x0a, 0x0c, 0x72, 0x65,
	0x63, 0x65, 0x69, 0x76, 0x65, 0x72, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x04, 0x20, 0x01, 0x28, 0x03,
	0x52, 0x0b, 0x72, 0x65, 0x63, 0x65, 0x69, 0x76, 0x65, 0x72, 0x49, 0x69, 0x64, 0x12, 0x1e, 0x0a,
	0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6d, 0x65, 0x6e, 0x74, 0x18, 0x05, 0x20, 0x01, 0x28,
	0x05, 0x52, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x6d, 0x65, 0x6e, 0x74, 0x12, 0x1e, 0x0a,
	0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x69, 0x6e, 0x66, 0x6f, 0x18, 0x06, 0x20, 0x01, 0x28,
	0x09, 0x52, 0x0a, 0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x69, 0x6e, 0x66, 0x6f, 0x12, 0x14, 0x0a,
	0x05, 0x74, 0x69, 0x74, 0x6c, 0x65, 0x18, 0x07, 0x20, 0x01, 0x28, 0x09, 0x52, 0x05, 0x74, 0x69,
	0x74, 0x6c, 0x65, 0x12, 0x1a, 0x0a, 0x08, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x73, 0x18,
	0x08, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x73, 0x12,
	0x1e, 0x0a, 0x0a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6d, 0x65, 0x18, 0x09, 0x20,
	0x01, 0x28, 0x05, 0x52, 0x0a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x74, 0x69, 0x6d, 0x65, 0x12,
	0x1a, 0x0a, 0x08, 0x72, 0x65, 0x61, 0x64, 0x74, 0x69, 0x6d, 0x65, 0x18, 0x0a, 0x20, 0x01, 0x28,
	0x05, 0x52, 0x08, 0x72, 0x65, 0x61, 0x64, 0x74, 0x69, 0x6d, 0x65, 0x22, 0x3c, 0x0a, 0x0f, 0x4d,
	0x61, 0x69, 0x6c, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x49, 0x74, 0x65, 0x6d, 0x73, 0x12, 0x29,
	0x0a, 0x05, 0x6d, 0x61, 0x69, 0x6c, 0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x13, 0x2e,
	0x50, 0x52, 0x4f, 0x2e, 0x4d, 0x61, 0x69, 0x6c, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x49, 0x74,
	0x65, 0x6d, 0x52, 0x05, 0x6d, 0x61, 0x69, 0x6c, 0x73, 0x22, 0x7b, 0x0a, 0x10, 0x46, 0x72, 0x69,
	0x65, 0x6e, 0x64, 0x49, 0x6e, 0x76, 0x69, 0x74, 0x65, 0x49, 0x74, 0x65, 0x6d, 0x12, 0x10, 0x0a,
	0x03, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x03, 0x69, 0x69, 0x64, 0x12,
	0x19, 0x0a, 0x08, 0x66, 0x72, 0x6f, 0x6d, 0x5f, 0x69, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28,
	0x03, 0x52, 0x07, 0x66, 0x72, 0x6f, 0x6d, 0x49, 0x69, 0x64, 0x12, 0x1a, 0x0a, 0x08, 0x6e, 0x69,
	0x63, 0x6b, 0x6e, 0x61, 0x6d, 0x65, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x6e, 0x69,
	0x63, 0x6b, 0x6e, 0x61, 0x6d, 0x65, 0x12, 0x1e, 0x0a, 0x0a, 0x69, 0x6e, 0x76, 0x69, 0x74, 0x65,
	0x74, 0x69, 0x6d, 0x65, 0x18, 0x04, 0x20, 0x01, 0x28, 0x05, 0x52, 0x0a, 0x69, 0x6e, 0x76, 0x69,
	0x74, 0x65, 0x74, 0x69, 0x6d, 0x65, 0x22, 0x74, 0x0a, 0x0e, 0x46, 0x72, 0x69, 0x65, 0x6e, 0x64,
	0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x12, 0x10, 0x0a, 0x03, 0x69, 0x69, 0x64, 0x18,
	0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x03, 0x69, 0x69, 0x64, 0x12, 0x1c, 0x0a, 0x09, 0x66, 0x72,
	0x69, 0x65, 0x6e, 0x64, 0x69, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x03, 0x52, 0x09, 0x66,
	0x72, 0x69, 0x65, 0x6e, 0x64, 0x69, 0x69, 0x64, 0x12, 0x1a, 0x0a, 0x08, 0x6e, 0x69, 0x63, 0x6b,
	0x6e, 0x61, 0x6d, 0x65, 0x18, 0x03, 0x20, 0x01, 0x28, 0x09, 0x52, 0x08, 0x6e, 0x69, 0x63, 0x6b,
	0x6e, 0x61, 0x6d, 0x65, 0x12, 0x16, 0x0a, 0x06, 0x6f, 0x6e, 0x6c, 0x69, 0x6e, 0x65, 0x18, 0x04,
	0x20, 0x01, 0x28, 0x08, 0x52, 0x06, 0x6f, 0x6e, 0x6c, 0x69, 0x6e, 0x65, 0x22, 0x2a, 0x0a, 0x0e,
	0x46, 0x72, 0x69, 0x65, 0x6e, 0x64, 0x55, 0x73, 0x65, 0x72, 0x49, 0x6e, 0x66, 0x6f, 0x12, 0x18,
	0x0a, 0x07, 0x72, 0x6f, 0x6c, 0x65, 0x69, 0x69, 0x64, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52,
	0x07, 0x72, 0x6f, 0x6c, 0x65, 0x69, 0x69, 0x64, 0x42, 0x54, 0x0a, 0x1b, 0x63, 0x6f, 0x6d, 0x2e,
	0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f,
	0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x50, 0x01, 0x5a, 0x15, 0x67, 0x61, 0x6d, 0x65, 0x6c,
	0x69, 0x62, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2f, 0x67, 0x70, 0x72, 0x6f,
	0xaa, 0x02, 0x1b, 0x63, 0x6f, 0x6d, 0x2e, 0x63, 0x6d, 0x73, 0x2e, 0x63, 0x6c, 0x69, 0x65, 0x6e,
	0x74, 0x2e, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x67, 0x70, 0x72, 0x6f, 0x62, 0x06,
	0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_global_internal_proto_rawDescOnce sync.Once
	file_global_internal_proto_rawDescData = file_global_internal_proto_rawDesc
)

func file_global_internal_proto_rawDescGZIP() []byte {
	file_global_internal_proto_rawDescOnce.Do(func() {
		file_global_internal_proto_rawDescData = protoimpl.X.CompressGZIP(file_global_internal_proto_rawDescData)
	})
	return file_global_internal_proto_rawDescData
}

var file_global_internal_proto_msgTypes = make([]protoimpl.MessageInfo, 7)
var file_global_internal_proto_goTypes = []interface{}{
	(*MailSystemItem)(nil),   // 0: PRO.MailSystemItem
	(*MailSystemItems)(nil),  // 1: PRO.MailSystemItems
	(*MailNormalItem)(nil),   // 2: PRO.MailNormalItem
	(*MailNormalItems)(nil),  // 3: PRO.MailNormalItems
	(*FriendInviteItem)(nil), // 4: PRO.FriendInviteItem
	(*FriendRelation)(nil),   // 5: PRO.FriendRelation
	(*FriendUserInfo)(nil),   // 6: PRO.FriendUserInfo
}
var file_global_internal_proto_depIdxs = []int32{
	0, // 0: PRO.MailSystemItems.mails:type_name -> PRO.MailSystemItem
	2, // 1: PRO.MailNormalItems.mails:type_name -> PRO.MailNormalItem
	2, // [2:2] is the sub-list for method output_type
	2, // [2:2] is the sub-list for method input_type
	2, // [2:2] is the sub-list for extension type_name
	2, // [2:2] is the sub-list for extension extendee
	0, // [0:2] is the sub-list for field type_name
}

func init() { file_global_internal_proto_init() }
func file_global_internal_proto_init() {
	if File_global_internal_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_global_internal_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MailSystemItem); i {
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
		file_global_internal_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MailSystemItems); i {
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
		file_global_internal_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MailNormalItem); i {
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
		file_global_internal_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*MailNormalItems); i {
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
		file_global_internal_proto_msgTypes[4].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*FriendInviteItem); i {
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
		file_global_internal_proto_msgTypes[5].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*FriendRelation); i {
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
		file_global_internal_proto_msgTypes[6].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*FriendUserInfo); i {
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
			RawDescriptor: file_global_internal_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   7,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_global_internal_proto_goTypes,
		DependencyIndexes: file_global_internal_proto_depIdxs,
		MessageInfos:      file_global_internal_proto_msgTypes,
	}.Build()
	File_global_internal_proto = out.File
	file_global_internal_proto_rawDesc = nil
	file_global_internal_proto_goTypes = nil
	file_global_internal_proto_depIdxs = nil
}
