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

#pragma once

#include <cmsLib/prolib/ProtocolFactory.h>
#include <gameLib/protobuf/Proto_all.h>
#include "robotclient/PhpResult.h"

#include "testcase/core/TestCaseWorkFlow.h"
#include "testcase/VirtualUser.h"

#define MAX_PACK_LEN	16*1024
#define SINGLE_PACK_LEN 1024*16

class TestCase
{
public:
	TestCase();
	virtual ~TestCase(void);

	void run_it( ULONGLONG now);
	void start_case( bool notify =false);
	void stop_case( bool notify =false);
	void end_case();

	void send_gts_protocol( BasicProtocol *p);
	void recv_gts_protocol();

	bool force_close();

	void set_userinfo(int id, const char* urladdr, const char* prefix);
	void set_openprefix(const char* prefix);
	int get_userid();

	void set_cancel(){ cancel_ =true;};

	void try_connect_to_gts() {
		if (is_connected_2_gts())
			return;

		need_connect_2_gts_ = true;
		is_connect_2_gts_failed_ = false;
		gts_connect_errcode_ = 0;
	}

	bool connect_2_gts(int& err);
	void disconnect_2_gts();

	bool is_connected_2_gts() {
		return gts_socket_ != INVALID_SOCKET;
	}

	bool is_connect_2_gts_failed() {
		return is_connect_2_gts_failed_;
	}
	S_INT_32 get_gts_errcode() { return gts_connect_errcode_; }

	void wait_socket_close();

	void init_clientproto(int serverid, std::string account);

	void heart_beat(S_INT_64 tnow);

protected:
	bool				cancel_;

	SOCKET				gts_socket_;
	char				gts_recv_buffer_[MAX_PACK_LEN];
	int					gts_data_len_;

	bool				need_connect_2_gts_;
	bool				is_connect_2_gts_failed_;
	S_INT_32			gts_connect_errcode_;

	TestCaseWorkFlow	workflow_;
	VirtualUser			vuser_;
	//Ë÷ÒýÎ»ÖÃ
	int					userid_;

	CProtocolHead		head_;
	
	ProtocolFactory		*data_;
};
