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

package protocolx

import (
	"bytes"
	"encoding/binary"

	"google.golang.org/protobuf/proto"
)

type ProtoHeadBase struct {
	TotleLen uint32
	HeadLen  int16
	MsgId    uint16
}

func (p *ProtoHeadBase) GetMsgLen() uint32 {
	return p.TotleLen - uint32(p.HeadLen)
}

func (p ProtoHeadBase) DecodeBool(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val bool) {
	succ = false

	if (*offset)+1 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	*offset += 1

	val = (pdata[0] > 0)

	succ = true
	return
}

func (p ProtoHeadBase) EncodeBool(pdata *bytes.Buffer, offset *uint32, val bool) (succ bool) {
	succ = false

	var ival int8 = 0
	if val {
		ival = 1
	}

	binary.Write(pdata, binary.BigEndian, ival)

	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) DecodeUint16(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val uint16) {
	succ = false

	if (*offset)+2 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = binary.BigEndian.Uint16(pdata)
	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) EncodeUint16(pdata *bytes.Buffer, offset *uint32, val uint16) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) DecodeUint32(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val uint32) {
	succ = false

	if (*offset)+4 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = binary.BigEndian.Uint32(pdata)
	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) EncodeUint32(pdata *bytes.Buffer, offset *uint32, val uint32) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint8(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int8) {
	succ = false

	if (*offset)+1 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int8(pdata[0])
	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint8(pdata *bytes.Buffer, offset *uint32, val int8) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint16(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int16) {
	succ = false

	if (*offset)+2 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int16(binary.BigEndian.Uint16(pdata))
	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint16(pdata *bytes.Buffer, offset *uint32, val int16) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint32(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int32) {
	succ = false

	if (*offset)+4 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int32(binary.BigEndian.Uint32(pdata))
	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint32(pdata *bytes.Buffer, offset *uint32, val int32) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint64(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int64) {
	succ = false

	if (*offset)+8 > maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int64(binary.BigEndian.Uint64(pdata))
	*offset += 8

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint64(pdata *bytes.Buffer, offset *uint32, val int64) (succ bool) {
	succ = false

	binary.Write(pdata, binary.BigEndian, val)

	*offset += 8

	succ = true
	return
}

//----------------------------------------------UserToken--------------------------------------
type UserToken struct {
	TokenGidRid    int64
	TokenSlotToken int64
}

func NewUserToken(gidrid int64, slottoken int64) (token *UserToken) {
	token = new(UserToken)
	token.TokenGidRid = gidrid
	token.TokenSlotToken = slottoken

	return
}

func (p *UserToken) SyncToken(token *UserToken) {
	p.TokenGidRid = token.TokenGidRid
	p.TokenSlotToken = token.TokenSlotToken
}

//20:gate 43:userid
//0b0000000000000000000001111111111111111111111111111111111111111111
const USERGATE_MASK_ZERO_H = uint64(0x7FFFFFFFFFF)

//0b1111111111111111111110000000000000000000000000000000000000000000
const USERGATE_MASK_ZERO_L = uint64(0xFFFFF80000000000)

//14：slot 50:timestamp
//0b0000000000000011111111111111111111111111111111111111111111111111
const USERTOKEN_MASK_ZERO_H = uint64(0x3FFFFFFFFFFFF)

//0b1111111111111100000000000000000000000000000000000000000000000000
const USERTOKEN_MASK_ZERO_L = uint64(0xFFFC000000000000)

func (p *UserToken) BuildTokenGidRid(gateiid int64, useriid int64) int64 {
	giid := (uint64(gateiid) << 43) & USERGATE_MASK_ZERO_L
	uiid := uint64(useriid)
	return int64(giid | uiid)
}

func (p *UserToken) BuildTokenSlotToken(slot int32, token int64) int64 {
	siid := (uint64(slot) << 50) & USERTOKEN_MASK_ZERO_L
	tiid := uint64(token)
	return int64(siid | tiid)
}

func (p *UserToken) SetTokenGidRid(gateiid int64, useriid int64) {
	p.TokenGidRid = p.BuildTokenGidRid(gateiid, useriid)
}

func (p *UserToken) GetTokenGateIid() int64 {
	grd := uint64(p.TokenGidRid)
	return int64((grd & USERGATE_MASK_ZERO_L) >> 43)
}

func (p *UserToken) GetTokenRoleIid() int64 {
	grd := uint64(p.TokenGidRid)
	return int64(grd & USERGATE_MASK_ZERO_H)
}

func (p *UserToken) SetTokenSlotToken(slot int32, token int64) {
	p.TokenSlotToken = p.BuildTokenSlotToken(slot, token)
}

func (p *UserToken) GetTokenSlot() int32 {
	grd := uint64(p.TokenSlotToken)
	return int32((grd & USERTOKEN_MASK_ZERO_L) >> 50)
}

func (p *UserToken) GetTokenToken() int64 {
	grd := uint64(p.TokenSlotToken)
	return int64(grd & USERTOKEN_MASK_ZERO_H)
}

//----------------------------------------------SProtocolHead--------------------------------------
type SProtocolHead struct {
	ProtoHeadBase

	FromType int8
	ToType   int8
	//token信息
	Token          UserToken
	RoleId         int64
	GameId         int64
	UnpackProtocol bool
}

func (p *SProtocolHead) InitHead() {
	p.FromType = -1
	p.ToType = -1
	p.Token = UserToken{TokenGidRid: 0, TokenSlotToken: 0}
	p.RoleId = 0
	p.GameId = 0
	p.UnpackProtocol = true
}

func (p *SProtocolHead) EncodeHead(pdata *bytes.Buffer) bool {

	var offset uint32 = 0

	//占位长度
	p.EncodeUint32(pdata, &offset, 0)

	p.EncodeUint16(pdata, &offset, p.MsgId)
	p.Encodeint8(pdata, &offset, p.FromType)
	p.Encodeint8(pdata, &offset, p.ToType)
	p.Encodeint64(pdata, &offset, p.Token.TokenGidRid)
	p.Encodeint64(pdata, &offset, p.Token.TokenSlotToken)
	p.Encodeint64(pdata, &offset, p.RoleId)
	p.Encodeint64(pdata, &offset, p.GameId)

	p.HeadLen = int16(offset)
	p.TotleLen = offset

	return true
}

func (p *SProtocolHead) EncodeTotleLen(pdata *bytes.Buffer, msglen uint32) bool {
	p.TotleLen += msglen
	pbuf := pdata.Bytes()

	binary.BigEndian.PutUint32(pbuf, p.TotleLen)

	return true
}

func (p *SProtocolHead) DecodeHead(pbuf []byte, maxlen uint32) bool {
	p.TotleLen = maxlen

	pdata := pbuf[0:]
	var offset uint32 = 4

	var succ bool = false
	succ, p.MsgId = p.DecodeUint16(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.FromType = p.Decodeint8(pdata, maxlen, &offset)
	if !succ {
		return false
	}
	succ, p.ToType = p.Decodeint8(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	pToken := &p.Token
	succ, pToken.TokenGidRid = p.Decodeint64(pdata, maxlen, &offset)
	if !succ {
		return false
	}
	succ, pToken.TokenSlotToken = p.Decodeint64(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.RoleId = p.Decodeint64(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.GameId = p.Decodeint64(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	p.HeadLen = int16(offset)
	return true
}

//----------------------------------------------NetProtocol--------------------------------------
type NetProtocol struct {
	Head    SProtocolHead
	Msg     proto.Message
	MsgData []byte
	DataLen int32
}

func NewNetProtocol() (pro *NetProtocol) {
	pro = new(NetProtocol)

	pro.Msg = nil
	pro.MsgData = nil
	pro.DataLen = 0

	pro.WriteHead().InitHead()

	return
}

func NewNetProtocolByMsg(msg proto.Message) (pro *NetProtocol) {
	pro = new(NetProtocol)
	pro.Msg = msg
	pro.MsgData = nil
	pro.DataLen = 0
	pro.WriteHead().InitHead()

	return
}

func NewNetProtocolByHeadMsg(msg proto.Message, head *SProtocolHead) (pro *NetProtocol) {
	pro = new(NetProtocol)
	pro.Msg = msg
	pro.MsgData = nil
	pro.DataLen = 0

	pro.SetHead(head)

	return
}

func NewNetProtocolByData(dat []byte, len int32) (pro *NetProtocol) {
	pro = new(NetProtocol)
	pro.Msg = nil
	pro.MsgData = dat
	pro.DataLen = len

	return
}

func (np *NetProtocol) GetMsgId() uint16 {
	return np.Head.MsgId
}

func (np *NetProtocol) WriteHead() *SProtocolHead {
	return &np.Head
}

func (np *NetProtocol) SetHead(head *SProtocolHead) {
	np.Head = *head
}

func (np *NetProtocol) GetTokenRoleIid() int64 {
	return np.Head.Token.GetTokenRoleIid()
}

func (np *NetProtocol) GetUserToken() UserToken {
	return np.Head.Token
}
