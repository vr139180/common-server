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

#include <cmsLib/net/NetSessionBindEvent.h>

#include <cmsLib/prolib/ProtocolFactory.h>

NetProtocol* NetSessionBindEvent::unpack_protocol(S_UINT_8 *pbuf, S_UINT_32 prolen)
{
	NetProtocol* pro = new NetProtocol();
	std::unique_ptr<NetProtocol> ptr(pro);

	S_UINT_8 *pdata = pbuf;

	SProtocolHead& head = pro->head_;
	if (!head.decode_head(pdata, prolen))
		return 0;

	head.unpack_protocol_ = this->is_need_unpack_protocol(head.get_msgid());

	pdata = pbuf + (S_UINT_32)head.get_headlen();

	if (head.unpack_protocol_)
	{
		ProtocolFactory* fact = ProtocolFactory::instance();
		try {
			BasicProtocol* bp = fact->iid_to_proto(head.get_msgid(), pdata, head.get_msglen());
			if (bp == 0)
				return 0;

			pro->msg_ = bp;
		}
		catch (...) {
			return 0;
		}
	}
	else
	{
		//不解析协议，保存单独的内存块
		pro->data_len_ = head.get_msglen();
		pro->msg_data_ = new S_UINT_8[pro->data_len_];
		memcpy(pro->msg_data_, pdata, pro->data_len_);
	}

	return ptr.release();
}

bool NetSessionBindEvent::pack_protocol(S_UINT_8 *pbuf, S_UINT_32 maxlen, NetProtocol* pro, S_UINT_32& prolen)
{
	prolen = 0;

	SProtocolHead& head = pro->head_;
	S_UINT_8 *pdata = pbuf;

	if (head.unpack_protocol_)
	{
		if (pro->msg_ == 0)
			return false;

		ProtocolFactory* fact = ProtocolFactory::instance();
		S_UINT_16 proiid = fact->proto_to_iid(pro->msg_);
		if (proiid == 0)
			return false;
		//设置协议id
		head.set_msgid( proiid);
	}
	
	if (!head.encode_head(pdata, maxlen))
		return false;

	if (head.unpack_protocol_)
	{
		S_UINT_32 len2 = (S_UINT_32)pro->msg_->ByteSizeLong();

		if (head.get_headlen() + len2 > maxlen)
			return false;

		try {
			pdata = pbuf + head.get_headlen();
			if (!pro->msg_->SerializeToArray(pdata, len2))
				return false;
		}
		catch (...) {
			return false;
		}

		head.encode_totlelen(pbuf, maxlen, len2);
	}
	else
	{
		if (pro->msg_data_ == 0 && pro->data_len_ > 0)
			return false;

		S_UINT_32 len2 = (S_UINT_32)pro->data_len_;
		if (head.get_headlen() + len2 > maxlen)
			return false;

		pdata = pbuf + head.get_headlen();
		memcpy(pdata, pro->msg_data_, len2);

		head.encode_totlelen(pbuf, maxlen, len2);
	}

	//写长度
	prolen = head.get_totlelen();

	return true;
}