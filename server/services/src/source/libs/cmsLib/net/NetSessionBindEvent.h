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

#ifndef __NETSESSIONBINDEVENT_H__
#define __NETSESSIONBINDEVENT_H__

#include <cmsLib/prolib/BasicProtocol.h>

class NetSessionBindEvent
{
public:
	//only for connect to client
	virtual void on_cant_connectedto() = 0;
	virtual void on_connectedto_done() = 0;

	virtual void on_connect_lost_netthread() =0;
	virtual void on_recv_protocol_netthread( NetProtocol* pro) =0;

	virtual NetProtocol* get_livekeep_msg() { return 0; }

	//Ω‚Œˆ–≠“È
	virtual NetProtocol* unpack_protocol(S_UINT_8 *pbuf, S_UINT_32 prolen);
	virtual bool pack_protocol(S_UINT_8 *pbuf, S_UINT_32 maxlen, NetProtocol* pro, S_UINT_32& prolen);
	virtual bool is_need_unpack_protocol(S_UINT_16 msgid) { return true; }
};

#endif	//__NETSESSIONBINDEVENT_H__
