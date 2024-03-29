﻿// Copyright 2021 common-server Authors
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

#include "cmsLib/prolib/ProtocolFactory.h"

ProtocolFactory* ProtocolFactory::instance()
{
	static ProtocolFactory s_instance;
	return &s_instance;
}

ProtocolFactory::ProtocolFactory()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
}

ProtocolFactory::~ProtocolFactory()
{
	google::protobuf::ShutdownProtobufLibrary();
}

void ProtocolFactory::uinit()
{
	this->release();
}

void ProtocolFactory::release()
{
	iid_2_proto.clear();
	//name_2_iid.clear();
	desc_2_iid.clear();
}

S_UINT_16 ProtocolFactory::proto_to_iid(BasicProtocol* pro)
{
	if (pro == 0)
		return 0;
	//boost::unordered_map<const std::string, S_UINT_16>::iterator fiter = name_2_iid.find(pro->GetTypeName());
	boost::unordered_map<const google::protobuf::Descriptor*, S_UINT_16>::iterator fiter = desc_2_iid.find(pro->GetDescriptor());
	if (fiter == desc_2_iid.end())
		return 0;
	return fiter->second;
}

BasicProtocol* ProtocolFactory::iid_to_proto(S_UINT_16 proiid, void *pdata, S_UINT_16 len)
{
	boost::unordered_map<S_UINT_16, const BasicProtocol*>::iterator fiter = iid_2_proto.find(proiid);
	if (fiter == iid_2_proto.end())
		return 0;

	const BasicProtocol* prox = fiter->second;
	if (prox == 0)
		return 0;

	BasicProtocol* pro = prox->New();
	if( pro == 0 || pro->ParseFromArray(pdata, len) == false)
	{
		if( pro != 0)
			delete pro;
		return 0;
	}

	return pro;
}
