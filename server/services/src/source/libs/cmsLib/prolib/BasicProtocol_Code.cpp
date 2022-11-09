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

#define ENCODE(buf, size, offset, data, type)\
	if ((NULL == buf) || (0 == size))\
	{\
		return E_INVALIDARGUMENT;\
	}\
	if (offset + sizeof(type) > size)\
	{\
		return E_BUFFERLENGTH;\
	}\
	S_UINT_8 *p = (S_UINT_8*)&data;\
	if (CProtoHeadBase::is_bigendian())\
	{\
	for(S_UINT_32 i = 0; i < sizeof(type); ++i)\
		{\
			buf[offset++] = p[i];\
		}\
	}\
	else\
	{\
	for(S_UINT_32 i = 0; i < sizeof(type); ++i)\
		{\
			buf[offset++] = p[sizeof(type)-i-1];\
		}\
	}\
	return SC_OK;


#define DECODE(buf, size, offset, data, type)\
	if ((NULL == buf) || (0 == size))\
	{\
		return E_INVALIDARGUMENT;\
	}\
	if (offset + sizeof(type) > size)\
	{\
		return E_BUFFERLENGTH;\
	}\
	S_UINT_8 *p = (S_UINT_8*)&data;\
	if (CProtoHeadBase::is_bigendian())\
	{\
	for(S_UINT_32 i = 0; i < sizeof(type); ++i)\
		{\
			p[i] = buf[offset++];\
		}\
	} \
	else\
	{\
	for (S_UINT_32 i = 0; i < sizeof(type); ++i)\
		{\
			p[sizeof(type)-i-1] = buf[offset++];\
		}\
	}\
	return SC_OK;

class CEndianCheck
{
public:
	CEndianCheck()
	{
		union
		{
			S_UINT_16	s1;
			S_UINT_8	c2[2];
		} un;
		un.s1 = 0x0102;
		m_bBigEndian = (un.c2[0] == 0x01);
	}

	inline bool IsBigEndian()
	{
		return m_bBigEndian;
	}

	bool m_bBigEndian;
};

static CEndianCheck s_endiancheck;

bool CProtoHeadBase::is_bigendian()
{
	return s_endiancheck.IsBigEndian();
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const bool data)
{
	S_UINT_8 v = (data ? 1 : 0);
	return Encode(buf, size, offset, v);
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_8 data)
{
	ENCODE(buf, size, offset, data, S_INT_8)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_16 data)
{
	ENCODE(buf, size, offset, data, S_INT_16)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_32 data)
{
	ENCODE(buf, size, offset, data, S_INT_32)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_INT_64 data)
{
	ENCODE(buf, size, offset, data, S_INT_64)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_8 data)
{
	ENCODE(buf, size, offset, data, S_UINT_8)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_16 data)
{
	ENCODE(buf, size, offset, data, S_UINT_16)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_32 data)
{
	ENCODE(buf, size, offset, data, S_UINT_32)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_64 data)
{
	ENCODE(buf, size, offset, data, S_UINT_64)
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const S_UINT_8 *data, const S_UINT_32 length, const S_UINT_32 maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if ((NULL == data) || (0 == length))
	{
		return SC_OK;
	}
	if (offset + length > size)
	{
		return E_BUFFERLENGTH;
	}
	if (length > maxLength)
	{
		return E_MAXLENGTH;
	}

	memcpy(buf + offset, data, length);
	offset += length;

	return SC_OK;
}

S_INT_32 CProtoHeadBase::Encode(S_UINT_8* buf, const S_UINT_32 size, S_UINT_32& offset, const char *data, const S_UINT_16 maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if (NULL == data)
	{
		return E_INVALIDARGUMENT;
	}
	if (0 == maxLength)
	{
		return E_MAXLENGTH;
	}

	S_INT_32 ret = SC_OK;

	S_UINT_16 len = (S_UINT_16)strlen(data) + 1;
	ret = Encode(buf, size, offset, len);
	if (0 > ret)
	{
		return ret;
	}
	ret = Encode(buf, size, offset, (S_UINT_8*)data, len, (S_UINT_32)maxLength);
	if (0 > ret)
	{
		return ret;
	}

	return SC_OK;
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, bool& data)
{
	S_INT_32 ret = SC_OK;

	S_UINT_8 v = 0;
	ret = Decode(buf, size, offset, v);
	if (0 > ret)
	{
		return ret;
	}

	data = (0 != v);

	return SC_OK;
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_8& data)
{
	DECODE(buf, size, offset, data, S_INT_8)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_16& data)
{
	DECODE(buf, size, offset, data, S_INT_16)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_32& data)
{
	DECODE(buf, size, offset, data, S_INT_32)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_INT_64& data)
{
	DECODE(buf, size, offset, data, S_INT_64)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_8& data)
{
	DECODE(buf, size, offset, data, S_UINT_8)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_16& data)
{
	DECODE(buf, size, offset, data, S_UINT_16)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_32& data)
{
	DECODE(buf, size, offset, data, S_UINT_32)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_64& data)
{
	DECODE(buf, size, offset, data, S_UINT_64)
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, S_UINT_8* data, const S_UINT_32 length, const S_UINT_32 maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if ((NULL == data) || (0 == length))
	{
		return SC_OK;
	}
	if (offset + length > size)
	{
		return E_BUFFERLENGTH;
	}
	if (length > maxLength)
	{
		return E_MAXLENGTH;
	}

	memcpy(data, buf + offset, length);
	offset += length;

	return SC_OK;
}

S_INT_32 CProtoHeadBase::Decode(S_UINT_8 *buf, const S_UINT_32 size, S_UINT_32& offset, char* data, const S_UINT_16 maxLength)
{
	if (0 == maxLength)
	{
		return E_MAXLENGTH;
	}

	S_INT_32 ret = SC_OK;

	S_UINT_16 len = 0;
	ret = Decode(buf, size, offset, len);
	if (0 > ret)
	{
		return ret;
	}
	if (0 == len)
	{
		data[0] = '\0';
		return SC_OK;
	}
	ret = Decode(buf, size, offset, (S_UINT_8*)data, (S_UINT_32)len, (S_UINT_32)maxLength);
	if (0 > ret)
	{
		return ret;
	}
	data[len - 1] = '\0';

	return SC_OK;
}
