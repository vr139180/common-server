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

#include <cmsLib/prolib/BasicProtocol.h>

//-----------------------------------CProtocolHead--------------------------------
bool CProtocolHead::encode_totlelen(S_UINT_8 *pbuf, S_UINT_32 maxlen, S_UINT_32 msglen)
{
	totle_len_ += msglen;

	S_UINT_32 offset = 0;
	if (Encode(pbuf, maxlen, offset, totle_len_) != SC_OK)
		return false;

	return true;
}

bool CProtocolHead::encode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen)
{
	S_UINT_8 *pdata = pbuf;

	S_UINT_32 offset = sizeof(S_UINT_32);
	if (Encode(pdata, maxlen, offset, msgid_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, encryption_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, version_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, channel_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, seqno_) != SC_OK)
		return false;

	this->head_len_ = offset;
	this->totle_len_ = this->head_len_;

	return true;
}

bool CProtocolHead::decode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen)
{
	this->totle_len_ = (S_INT_32)maxlen;

	S_UINT_8 *pdata = pbuf;
	S_UINT_32 offset = sizeof(S_UINT_32);
	if (Decode(pdata, maxlen, offset, this->msgid_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->encryption_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->version_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->channel_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->seqno_) != SC_OK)
		return false;

	this->head_len_ = (S_INT_16)offset;

	return true;
}

//-----------------------------------SProtocolHead--------------------------------
//14:slot 50:timestamp
#define USERTOKEN_MASK_ZERO_H	0b0000000000000011111111111111111111111111111111111111111111111111
#define USERTOKEN_MASK_ZERO_L	0b1111111111111100000000000000000000000000000000000000000000000000

#define MAKE_USERTOKEN( SLOT, TOKEN, USERTOKEN) { USERTOKEN = ((SLOT<<50)&USERTOKEN_MASK_ZERO_L)|TOKEN;}
#define PARSE_USERTOKEN( USERTOKEN, SLOT, TOKEN) { SLOT = (int)((USERTOKEN&USERTOKEN_MASK_ZERO_L)>>50); TOKEN = (USERTOKEN&USERTOKEN_MASK_ZERO_H);}
#define PARSE_USERTOKEN_SLOT( USERTOKEN, SLOT) { SLOT = (int)((USERTOKEN&USERTOKEN_MASK_ZERO_L)>>50);}
#define PARSE_USERTOKEN_TOKEN( USERTOKEN, TOKEN) { TOKEN = (USERTOKEN&USERTOKEN_MASK_ZERO_H);}

//20:gate 43:useriid
#define USERGATE_MASK_ZERO_H	0b0000000000000000000001111111111111111111111111111111111111111111
#define USERGATE_MASK_ZERO_L	0b1111111111111111111110000000000000000000000000000000000000000000

#define MAKE_USERGATE( GATEID, USERID, USERGATE) { USERGATE = ((GATEID<<43)&USERGATE_MASK_ZERO_L)|USERID;}
#define PARSE_USERGATE( USERGATE, GATEID, USERID) { GATEID = (USERGATE&USERGATE_MASK_ZERO_L)>>43; USERID = (USERGATE&USERGATE_MASK_ZERO_H);}
#define PARSE_USERGATE_GATEID( USERGATE, GATEID) { GATEID = (USERGATE&USERGATE_MASK_ZERO_L)>>43;}
#define PARSE_USERGATE_ROLEID( USERGATE, USERID) { USERID = (USERGATE&USERGATE_MASK_ZERO_H);}

S_INT_64 SProtocolHead::build_token_giduid(S_INT_64 gateiid, S_INT_64 useriid)
{
	S_INT_64 r = 0;
	MAKE_USERGATE(gateiid, useriid, r);
	return r;
}

S_INT_64 SProtocolHead::build_token_slottoken(S_INT_32 slot, S_INT_64 token)
{
	S_INT_64 r = 0;
	MAKE_USERTOKEN((S_INT_64)slot, token, r);
	return r;
}

void SProtocolHead::sync_token(const SProtocolHead& head)
{
	this->token_giduid_ = head.token_giduid_;
	this->token_slottoken_ = head.token_slottoken_;
	this->role_iid_ = head.role_iid_;
}

void SProtocolHead::set_token_giduid(S_INT_64 gateiid, S_INT_64 useriid)
{
	MAKE_USERGATE(gateiid, useriid, token_giduid_);
}

void SProtocolHead::set_token_gateid(S_INT_64 gateiid)
{
	S_INT_64 uid = get_token_useriid();
	set_token_giduid(gateiid, uid);
}

void SProtocolHead::set_token_userid(S_INT_64 userid)
{
	S_INT_64 gid = get_token_gateiid();
	set_token_giduid(gid, userid);
}

void SProtocolHead::set_token_token(S_INT_64 token)
{
	S_INT_32 slot = get_token_slot();
	set_token_slottoken(slot, token);
}

S_INT_64 SProtocolHead::get_token_gateiid() const
{
	S_INT_64 gid = 0;
	PARSE_USERGATE_GATEID(token_giduid_, gid);
	return gid;
}

S_INT_64 SProtocolHead::get_token_useriid() const
{
	S_INT_64 uid = 0;
	PARSE_USERGATE_ROLEID(token_giduid_, uid);
	return uid;
}

void SProtocolHead::set_token_slottoken(S_INT_32 slot, S_INT_64 token)
{
	MAKE_USERTOKEN((S_INT_64)slot, token, token_slottoken_);
}

S_INT_32 SProtocolHead::get_token_slot() const
{
	S_INT_32 slot = 0;
	PARSE_USERTOKEN_SLOT(token_slottoken_, slot);
	return slot;
}

S_INT_64 SProtocolHead::get_token_token() const
{
	S_INT_64 token = 0;
	PARSE_USERTOKEN_TOKEN(token_slottoken_, token);
	return token;
}

bool SProtocolHead::encode_totlelen(S_UINT_8 *pbuf, S_UINT_32 maxlen, S_UINT_32 msglen)
{
	totle_len_ += msglen;

	S_UINT_32 offset = 0;
	if (Encode(pbuf, maxlen, offset, totle_len_) != SC_OK)
		return false;

	return true;
}

bool SProtocolHead::encode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen)
{
	S_UINT_8 *pdata = pbuf;

	S_UINT_32 offset = sizeof(S_UINT_32);
	if (Encode(pdata, maxlen, offset, msgid_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, router_balance_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, from_type_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, to_type_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, token_giduid_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, token_slottoken_) != SC_OK)
		return false;
	if (Encode(pdata, maxlen, offset, role_iid_) != SC_OK)
		return false;

	this->head_len_ = offset;
	this->totle_len_ = this->head_len_;

	return true;
}

bool SProtocolHead::decode_head(S_UINT_8 *pbuf, S_UINT_32 maxlen)
{
	//过滤整体长度,指向shead头
	this->totle_len_ = (S_INT_32)maxlen;

	S_UINT_8 *pdata = pbuf;
	S_UINT_32 offset = sizeof(S_UINT_32);
	if (Decode(pdata, maxlen, offset, this->msgid_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->router_balance_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->from_type_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->to_type_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->token_giduid_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->token_slottoken_) != SC_OK)
		return false;
	if (Decode(pdata, maxlen, offset, this->role_iid_) != SC_OK)
		return false;

	this->head_len_ = (S_INT_16)offset;

	return true;
}

//-----------------------------------NetProtocol--------------------------------

NetProtocol::NetProtocol():msg_(0), msg_data_(0), data_len_(0)
{
}

NetProtocol::NetProtocol(const SProtocolHead& h, BasicProtocol* msg) : head_(h)
,msg_(msg), msg_data_(0), data_len_(0)
{
	head_.unpack_protocol_ = true;
}

NetProtocol::NetProtocol(const SProtocolHead& h) : head_(h)
, msg_(0), msg_data_(0), data_len_(0)
{

}

NetProtocol::NetProtocol(BasicProtocol* msg): msg_(msg), msg_data_(0), data_len_(0)
{
	head_.unpack_protocol_ = true;
}

NetProtocol* NetProtocol::clone()
{
	NetProtocol* newpro = new NetProtocol(this->head_);
	if (head_.unpack_protocol_)
	{
		if (this->msg_ != 0)
		{
			newpro->msg_ = this->msg_->New();
			newpro->msg_->CopyFrom( *msg_);
		}
	}
	else
	{
		if (this->data_len_ > 0 && this->msg_data_ != 0)
		{
			newpro->data_len_ = this->data_len_;
			newpro->msg_data_ = new S_UINT_8[newpro->data_len_];
			memcpy(newpro->msg_data_, this->msg_data_, newpro->data_len_);
		}
	}

	return newpro;
}

NetProtocol::~NetProtocol()
{
	if (msg_ != 0)
	{
		delete msg_;
		msg_ = 0;
	}

	if (msg_data_ != 0)
	{
		delete[] msg_data_;
		msg_data_ = 0;
	}
}
