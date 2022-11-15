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

#ifndef __PROTOCOLFACTORY__H__
#define __PROTOCOLFACTORY__H__

#include <boost/unordered_map.hpp>
#include <string>
#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

/**
* @class ProtocolFactory
* 
* @brief 通讯协议类工厂、所有定义的通讯协议必须在该类中注册。
* 改类不能保存自己的中间状态
**/
class ProtocolFactory
{
private:
	ProtocolFactory();

public:
	static ProtocolFactory* instance();
	~ProtocolFactory();

	virtual void init_factory();
	void uinit();

public:
	S_UINT_16 proto_to_iid( BasicProtocol* pro);
	BasicProtocol* iid_to_proto(S_UINT_16 proiid, void *pdata, S_UINT_16 len);

protected:
	void release();

	template<typename T>
	void regist_protocal(S_UINT_16 proiid);

private:
	boost::unordered_map<S_UINT_16, const BasicProtocol*>	iid_2_proto;
	//boost::unordered_map<const std::string, S_UINT_16>		name_2_iid;
	boost::unordered_map<const google::protobuf::Descriptor*, S_UINT_16>	desc_2_iid;
};

template<typename T>
void ProtocolFactory::regist_protocal(S_UINT_16 proiid)
{
	const BasicProtocol* pro = &(T::default_instance());

	iid_2_proto[proiid] = pro;
	//name_2_iid[pro->GetTypeName()] = proiid;
	desc_2_iid[pro->GetDescriptor()] = proiid;
}

#endif //__PROTOCOLFACTORY__H__
