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

#ifndef __USEROPERATE_H__
#define __USEROPERATE_H__

#include <cppconn/resultset.h>
#include <google/protobuf/message.h>

#include <cmsLib/redis/RedisClient.h>

#define DBVER_INIT	1

class UserOperate
{
public:
	virtual ~UserOperate(){}

public:
	//load from database
	virtual void load_from_database(sql::ResultSet& row) {}
	virtual google::protobuf::Message* get_data() { return 0; }
	virtual google::protobuf::Message* get_data_dels() { return 0; }

	void adjust_dels(S_INT_64 dbver);
	virtual void mark_data_dels_updated() {}

	virtual void reset_data() {}

	virtual bool is_modify() { return true; }

	template<typename T> T* clone_data();
	template<typename T> T* clone_datadels();
	template<typename T> T* clone_data(T* src);

	template<typename T> void data_copy_from(T* from);
	template<typename T> void datadels_copy_from(T* from);
	template<typename T> void data_copy_from(T* from, T* to);
};

template<typename T>
T* UserOperate::clone_data(T* src)
{
	T* ret = new T();
	ret->CopyFrom(*src);

	return ret;
}

template<typename T>
T* UserOperate::clone_data()
{
	T* ret = new T();
	T* from = dynamic_cast<T*>(get_data());
	ret->CopyFrom(*from);

	return ret;
}

template<typename T>
T* UserOperate::clone_datadels()
{
	T* ret = new T();
	T* from = dynamic_cast<T*>(get_data_dels());
	ret->CopyFrom(*from);

	return ret;
}

template<typename T>
void UserOperate::data_copy_from(T* from, T* to)
{
	to->CopyFrom(*from);
}

template<typename T> 
void UserOperate::data_copy_from(T* from)
{
	T* dat = dynamic_cast<T*>(get_data());
	dat->CopyFrom(*from);
}

template<typename T>
void UserOperate::datadels_copy_from(T* from)
{
	T* dat = dynamic_cast<T*>(get_data_dels());
	dat->CopyFrom(*from);
}

#endif //__USEROPERATE_H__
