// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __BASICPROTOCOL__H__
#define __BASICPROTOCOL__H__

#include <cmsLib/core_type.h>
#include <cmsLib/common_result.h>

//#include <google/protobuf/message_lite.h>
//typedef google::protobuf::MessageLite BasicProtocol;

#include <google/protobuf/message.h>
typedef google::protobuf::Message BasicProtocol;

class CProtoHeadBase
{
public:
	//判断系统字节序
	static bool is_bigendian();

	S_UINT_16 get_msgid() { return msgid_; }
	void set_msgid(S_UINT_16 id) { msgid_ = id; }
	S_UINT_16 get_msglen() { return (S_UINT_16)(totle_len_ - head_len_); }
	S_UINT_16 get_headlen() { return head_len_; }
	S_UINT_32 get_totlelen() { return totle_len_; }

public:
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const bool data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_8 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_16 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_32 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_64 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_8 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_16 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_32 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_64 data);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_8 *data, const S_UINT_32 length, const S_UINT_32 maxLength);
	static S_INT_32 Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const char *data, const S_UINT_16 maxLength);

	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, bool& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_8& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_16& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_32& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_64& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_8& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_16& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_32& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_64& data);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_8* data, const S_UINT_32 length, const S_UINT_32 maxLength);
	static S_INT_32 Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, char* data, const S_UINT_16 maxLength);

protected:
	//协议长度 head+body
	S_UINT_32	totle_len_;
	//协议头长度
	S_INT_16	head_len_;
	//协议id
	S_UINT_16	msgid_;
};

//客户端通讯协议头
class CProtocolHead : public CProtoHeadBase
{
public:
	CProtocolHead():encryption_(true),version_(0),channel_(0)
		, seqno_(0), unpack_protocol_(true){}

	bool encode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen);
	bool encode_totlelen(S_UINT_8 *pbuf, S_UINT_32 maxlen, S_UINT_32 msglen);
	bool decode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen);

public:
	//是否加密
	bool		encryption_;
	//版本号
	S_INT_16	version_;
	//渠道编号
	S_INT_16	channel_;
	//协议序列号,
	S_INT_32	seqno_;
	//是否需要解析协议,bindevent设置，缺省为true, false主要用于转发情况
	bool		unpack_protocol_;
};

//服务器通讯协议头
class SProtocolHead : public CProtoHeadBase
{
public:
	SProtocolHead():from_type_(-1), to_type_(-1), circles_(0), token_giduid_(0), token_slottoken_(0),
		role_iid_(0), gameid_(0), unpack_protocol_(true){}

	bool encode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen);
	bool encode_totlelen(S_UINT_8 *pbuf, S_UINT_32 maxlen, S_UINT_32 msglen);
	bool decode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen);

	void sync_token(const SProtocolHead& head);
	void set_token_giduid(S_INT_64 gateiid, S_INT_64 userid);
	void set_token_gateid(S_INT_64 gateiid);
	void set_token_userid(S_INT_64 userid);
	S_INT_64 get_token_gateiid() const;
	S_INT_64 get_token_useriid() const;

	void set_token_slottoken(S_INT_32 slot, S_INT_64 token);
	S_INT_32 get_token_slot() const;
	S_INT_64 get_token_token() const;
	void set_token_token(S_INT_64 token);

	S_INT_64 get_role_iid() const { return role_iid_; }
	void set_role_iid(S_INT_64 id) { role_iid_ = id; }
	S_INT_64 get_gameid() const { return gameid_; }
	void set_gameid(S_INT_64 gid) { gameid_ = gid; }

	void inc_circles() { ++circles_; }
	bool circle_out(S_INT_8 maax) { return circles_ > maax; }

	bool is_same_session(const SProtocolHead& head) {
		return get_token_useriid() == head.get_token_useriid() && get_token_token() == head.get_token_token();
	}

	bool is_same_token(const SProtocolHead& head) {
		return get_token_token() == head.get_token_token();
	}

	SProtocolHead& operator = (const SProtocolHead& v);

public:
	static S_INT_64 build_token_giduid(S_INT_64 gateiid, S_INT_64 useriid);
	static S_INT_64 build_token_slottoken(S_INT_32 slot, S_INT_64 token);

public:
	//来自哪类服务器
	S_INT_8		from_type_;
	//发送给哪类服务器
	S_INT_8		to_type_;
	//负载均衡时重复定位cluster的回环次数
	S_INT_8		circles_;
	//token信息
	S_INT_64	token_giduid_;
	S_INT_64	token_slottoken_;
	S_INT_64	role_iid_;
	//编码后的gameservice+channelid
	S_INT_64	gameid_;
	//是否需要解析协议,bindevent设置，缺省为true, false主要用于转发情况
	bool		unpack_protocol_;
};

//协议
class NetProtocol
{
public:
	NetProtocol();
	NetProtocol(const SProtocolHead& h);
	NetProtocol(const SProtocolHead& h, BasicProtocol* msg);
	NetProtocol( BasicProtocol* msg);
	virtual ~NetProtocol();

	void set_head(const SProtocolHead& h) { head_ = h; }
	SProtocolHead& write_head() { return head_; }

	S_UINT_16 get_msg() { return head_.get_msgid(); }
	S_INT_64 get_useriid() { return head_.get_token_useriid(); }
	S_INT_64 get_roleiid() { return head_.get_role_iid(); }
	S_INT_64 get_gameid() { return head_.get_gameid(); }
	S_INT_8 get_to() { return head_.to_type_; }

	void inc_circle() { head_.inc_circles(); }
	bool circle_out(S_INT_8 maax) { return head_.circle_out(maax); }

	NetProtocol* clone();

public:
	SProtocolHead	head_;
	//包裹的协议
	BasicProtocol*	msg_;
	S_UINT_8*		msg_data_;
	S_UINT_16		data_len_;
};

#endif //__BASICPROTOCOL__H__

