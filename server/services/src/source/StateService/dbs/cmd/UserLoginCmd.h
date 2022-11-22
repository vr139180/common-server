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

#ifndef __USERLOGINCMD_H__
#define __USERLOGINCMD_H__

#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "dbs/cmd/BaseDBCmd.h"

class UserLoginCmd : public BaseDBCmd
{
public:
	UserLoginCmd( const char* uname, const char* pwd, StateService* p);

	void reuse_cmd( const SProtocolHead& h);

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection);
	//run in logic thread
	virtual void run();

	virtual bool reused() { return true; }

protected:
	//input parameters
	std::string		username_;
	std::string		pwd_;
	SProtocolHead	head_;

	//output
	//0:成功 1:账号被禁用 2:账号不存在 3:验证错误 4:系统错误 5:登陆排队中
	S_INT_32	result_;
	std::string user_pwd_;
	S_INT_64	user_iid_;
	S_INT_32	user_state_;
};

#endif //__USERLOGINCMD_H__
