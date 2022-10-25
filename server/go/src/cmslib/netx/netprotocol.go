package netx

import (
	"encoding/binary"

	"google.golang.org/protobuf/proto"
)

type ProtoHeadBase struct {
	TotleLen uint32
	HeadLen  int16
	MsgId    uint16
}

func (p ProtoHeadBase) DecodeBool(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val bool) {
	succ = false

	if (*offset)+1 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	*offset += 1

	val = (pdata[0] > 0)

	succ = true
	return
}

func (p ProtoHeadBase) EncodeBool(pbuf []byte, maxlen uint32, offset *uint32, val bool) (succ bool) {
	succ = false

	if (*offset)+1 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	if val {
		pdata[0] = 1
	} else {
		pdata[0] = 0
	}

	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) DecodeUint16(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val uint16) {
	succ = false

	if (*offset)+2 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = binary.BigEndian.Uint16(pdata)
	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) EncodeUint16(pbuf []byte, maxlen uint32, offset *uint32, val uint16) (succ bool) {
	succ = false

	if (*offset)+2 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	binary.BigEndian.PutUint16(pdata, val)

	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) DecodeUint32(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val uint32) {
	succ = false

	if (*offset)+4 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = binary.BigEndian.Uint32(pdata)
	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) EncodeUint32(pbuf []byte, maxlen uint32, offset *uint32, val uint32) (succ bool) {
	succ = false

	if (*offset)+4 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	binary.BigEndian.PutUint32(pdata, val)

	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint8(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int8) {
	succ = false

	if (*offset)+1 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int8(pdata[0])
	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint8(pbuf []byte, maxlen uint32, offset *uint32, val int8) (succ bool) {
	succ = false

	if (*offset)+1 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	pdata[0] = byte(val)

	*offset += 1

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint16(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int16) {
	succ = false

	if (*offset)+2 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int16(binary.BigEndian.Uint16(pdata))
	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint16(pbuf []byte, maxlen uint32, offset *uint32, val int16) (succ bool) {
	succ = false

	if (*offset)+2 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	binary.BigEndian.PutUint16(pdata, uint16(val))

	*offset += 2

	succ = true
	return
}

func (p ProtoHeadBase) Decodeint32(pbuf []byte, maxlen uint32, offset *uint32) (succ bool, val int32) {
	succ = false

	if (*offset)+4 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	val = int32(binary.BigEndian.Uint32(pdata))
	*offset += 4

	succ = true
	return
}

func (p ProtoHeadBase) Encodeint32(pbuf []byte, maxlen uint32, offset *uint32, val int32) (succ bool) {
	succ = false

	if (*offset)+4 < maxlen {
		return
	}

	pdata := pbuf[*offset:]
	binary.BigEndian.PutUint32(pdata, uint32(val))

	*offset += 4

	succ = true
	return
}

//----------------------------------------------CProtocolHead--------------------------------------
type CProtocolHead struct {
	*ProtoHeadBase

	Encryption     bool
	Version        int16
	Channel        int16
	SeqNo          int32
	UnpackProtocol bool
}

func (p *CProtocolHead) InitHead() {
	p.Encryption = false
	p.Version = 0
	p.Channel = 0
	p.SeqNo = 0
	p.UnpackProtocol = true
}

func (p *CProtocolHead) EncodeHead(pbuf []byte, maxlen uint32) bool {
	pdata := pbuf[0:]

	var offset uint32 = 4
	if !p.EncodeUint16(pdata, maxlen, &offset, p.MsgId) {
		return false
	}

	if !p.EncodeBool(pdata, maxlen, &offset, p.Encryption) {
		return false
	}

	if !p.Encodeint16(pdata, maxlen, &offset, p.Version) {
		return false
	}

	if !p.Encodeint16(pdata, maxlen, &offset, p.Channel) {
		return false
	}

	if !p.Encodeint32(pdata, maxlen, &offset, p.SeqNo) {
		return false
	}

	p.HeadLen = int16(offset)
	p.TotleLen = offset

	return true
}

func (p *CProtocolHead) EncodeTotleLen(pbuf []byte, maxlen uint32, msglen uint32) bool {
	p.TotleLen += msglen
	pdata := pbuf[0:]

	var offset uint32 = 0

	return p.EncodeUint32(pdata, maxlen, &offset, p.TotleLen)
}

func (p *CProtocolHead) DecodeHead(pbuf []byte, maxlen uint32) bool {
	p.TotleLen = maxlen

	pdata := pbuf[0:]
	var offset uint32 = 4

	var succ bool = false
	succ, p.MsgId = p.DecodeUint16(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.Encryption = p.DecodeBool(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.Version = p.Decodeint16(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.Channel = p.Decodeint16(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.SeqNo = p.Decodeint32(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	p.HeadLen = int16(offset)
	return true
}

//----------------------------------------------SProtocolHead--------------------------------------
type SProtocolHead struct {
	*ProtoHeadBase

	RouterBalance  bool
	HashKey        int32
	FromType       int8
	ToType         int8
	ToBroadCast    bool
	UnpackProtocol bool
}

func (p *SProtocolHead) InitHead() {
	p.RouterBalance = true
	p.HashKey = 0
	p.FromType = -1
	p.ToType = -1
	p.ToBroadCast = false
	p.UnpackProtocol = true
}

func (p *SProtocolHead) EncodeHead(pbuf []byte, maxlen uint32) bool {
	pdata := pbuf[0:]

	var offset uint32 = 4
	if !p.EncodeUint16(pdata, maxlen, &offset, p.MsgId) {
		return false
	}

	if !p.EncodeBool(pdata, maxlen, &offset, p.RouterBalance) {
		return false
	}

	if !p.Encodeint32(pdata, maxlen, &offset, p.HashKey) {
		return false
	}

	if !p.Encodeint8(pdata, maxlen, &offset, p.FromType) {
		return false
	}

	if !p.Encodeint8(pdata, maxlen, &offset, p.ToType) {
		return false
	}

	if !p.EncodeBool(pdata, maxlen, &offset, p.ToBroadCast) {
		return false
	}

	p.HeadLen = int16(offset)
	p.TotleLen = offset

	return true
}

func (p *SProtocolHead) EncodeTotleLen(pbuf []byte, maxlen uint32, msglen uint32) bool {
	p.TotleLen += msglen
	pdata := pbuf[0:]

	var offset uint32 = 0

	return p.EncodeUint32(pdata, maxlen, &offset, p.TotleLen)
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

	succ, p.RouterBalance = p.DecodeBool(pdata, maxlen, &offset)
	if !succ {
		return false
	}

	succ, p.HashKey = p.Decodeint32(pdata, maxlen, &offset)
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

	succ, p.ToBroadCast = p.DecodeBool(pdata, maxlen, &offset)
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

	return
}

func NewNetProtocolByMsg(msg proto.Message) (pro *NetProtocol) {
	pro = new(NetProtocol)
	pro.Msg = msg
	pro.MsgData = nil
	pro.DataLen = 0

	return
}

func NewNetProtocolByData(dat []byte, len int32) (pro *NetProtocol) {
	pro = new(NetProtocol)
	pro.Msg = nil
	pro.MsgData = dat
	pro.DataLen = len

	return
}
