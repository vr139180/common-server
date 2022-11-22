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

#include "cmsLib/redis/RedisClient.h"
#include <cmsLib/Log.h>

RedisClient::RedisClient():
	redis_( 0)
{
}

RedisClient::~RedisClient()
{
	if (redis_ != 0) {
		delete redis_;
		redis_ = 0;
	}
}

void RedisClient::init_redis(std::string ip, int port, std::string pwd, int db, 
	int sockettimeout, int pool_size)
{
	ConnectionOptions connection_options;
	connection_options.host = ip;
	connection_options.port = port;
	connection_options.password = pwd;
	connection_options.db = db;
	connection_options.keep_alive = true;
	connection_options.socket_timeout = std::chrono::milliseconds(sockettimeout);
	connection_options.connect_timeout = std::chrono::milliseconds(100);

	ConnectionPoolOptions pool_options;
	pool_options.size = pool_size;
	pool_options.wait_timeout = std::chrono::milliseconds(100);

	this->redis_ = new Redis( connection_options, pool_options);
	try {
		redis_->ping();
	}
	catch (...) {
		logError(out_runtime, "######## redis connected failed.......");
	}
}

S_INT_64 RedisClient::gen_uuid_from_redis(const char* hkey, const char* uuidkey)
{
	try {
		long long vx = redis_->hincrby(hkey, uuidkey, 1);

		return (S_INT_64)vx;
	}
	catch (...) {
		return -1;
	}
}

S_INT_64 RedisClient::gen_dbid_from_redis(const char* hkey, const char* fieldfmt, const char* tablename)
{
	try {
		std::string fs = build_rediskey(fieldfmt, tablename);
		long long vx = redis_->hincrby(hkey, fs.c_str(), 1);

		return (S_INT_64)vx;
	}
	catch (...) {
		return -1;
	}
}

std::string RedisClient::build_rediskey(const char* fmt, ...)
{
	char buf[128] = { 0 };

	va_list vaList;

	va_start(vaList, fmt);
	::vsnprintf(buf, 127, fmt, vaList);
	va_end(vaList);

	return buf;
}

bool RedisClient::has_hashmember(const char* hkey, const char* field)
{
	try {
		return redis_->hexists(hkey, field);
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::del_hashmember(const char* hkey, const char* field)
{
	try {
		redis_->hdel(hkey, field);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::set_hashmember(const char* hkey, const char* field, const char* val)
{
	try {
		redis_->hset(hkey, field, val);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::set_hashmember_ul(const char* hkey, const char* field, S_INT_64 val)
{
	try {
		char buf[10] = { 0, };
		sprintf(buf, "%lld", val);
		redis_->hset(hkey, field, buf);

		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::get_hashmember(const char* hkey, const char* field, std::string& val)
{
	try {
		OptionalString str = redis_->hget(hkey, field);
		if (!str)
			return false;

		val = str.value();
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::get_hashmember_l(const char* hkey, const char* field, long& val)
{
	std::string sval = "";
	if (get_hashmember(hkey, field, sval))
	{
		if (sval.size() == 0)
			return false;

		val = atol(sval.c_str());
		return true;
	}

	return false;
}

bool RedisClient::get_hashmember_ul(const char* hkey, const char* field, S_INT_64& val)
{
	std::string sval = "";
	if (get_hashmember(hkey, field, sval))
	{
		if (sval.size() == 0)
			return false;

		val = atoll(sval.c_str());
		return true;
	}

	return false;
}

bool RedisClient::inc_hashmemeber(const char* hkey, const char* field, S_INT_64& val, S_INT_64 incnum)
{
	try {
		val = redis_->hincrby(hkey, field, incnum);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::add_zset(const char* hset, const char* key, S_INT_64 val, UpdateType flag)
{
	try {
		redis_->zadd(hset, key, val, flag);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::getbyscore_zset(const char* hset, S_INT_64 maxval, std::vector<std::string>& keys)
{
	try {
		//redis_->zrangebyscore(hset, LeftBoundedInterval<double>(minval, BoundType::RIGHT_OPEN), std::back_inserter(keys));
		redis_->zrangebyscore(hset, RightBoundedInterval<double>(maxval, BoundType::LEFT_OPEN), std::back_inserter(keys));
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::del_zsetmember(const char* hset, const char* key)
{
	try {
		redis_->zrem(hset, key);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::get_zscore(const char* hset, const char* member, S_INT_64& val)
{
	try {
		OptionalDouble d = redis_->zscore(hset, member);
		if (d)
		{
			val = (S_INT_64)d.value();
			return true;
		}
		return false;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::del(const char* key)
{
	try {
		redis_->del(key);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::pexpire(const char* key, S_TIMESTAMP ttl)
{
	try {
		std::chrono::milliseconds t(ttl);
		return redis_->pexpire(key, t);
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::persist(const char* key)
{
	try {
		return redis_->persist(key);
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::add_set(const char* key, const char* member)
{
	try {
		redis_->sadd(key, member);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::del_setmember(const char* key, const char* member)
{
	try {
		redis_->srem(key, member);
		return true;
	}
	catch (...) {
		return false;
	}
}

int RedisClient::get_set_size(const char* key)
{
	try {
		long long v = redis_->scard(key);
		return (int)v;
	}
	catch (...) {
		return 0;
	}
}

bool RedisClient::pop_one(const char* key, std::string& member)
{
	try {
		OptionalString os = redis_->spop(key);
		if( os)
		{
			member = os->c_str();
			return true;
		}
		return false;
	}
	catch (...) {
		return false;
	}
}

bool RedisClient::evalStrs(const char* sc, std::initializer_list<StringView> keys, std::initializer_list<StringView> vals,
	std::vector<std::string>& rets)
{
	try {
		redis_->eval(sc, keys, vals, std::back_inserter(rets));
		return true;
	}
	catch (const Error &err) {
		logError(out_runtime, "redis eval error:%s", err.what());
		return false;
	}
}

bool RedisClient::set_hashobject(const char* hkey, const char* field,
	google::protobuf::Message* obj, RedisProtoBufThreadCache* cache)
{
	try
	{
		int cs = obj->ByteSizeLong();
		if (!obj->SerializeToArray(cache->data(), cs))
			return false;

		StringView sv(cache->data(), cs);

		redis_->hset(hkey, field, sv);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool RedisClient::get_hashobject(const char* hkey, const char*field, google::protobuf::Message* obj)
{
	try
	{
		OptionalString str = redis_->hget(hkey, field);
		if (!str)
			return false;

		return obj->ParseFromArray(str->c_str(), str->length());
	}
	catch (...)
	{
		logError(out_runtime, "redis get hashobject failed. key:%s field:%s", hkey, field);
		return false;
	}
}

bool RedisClient::get(const char* key, google::protobuf::Message* obj, S_TIMESTAMP ttl)
{
	try
	{
		OptionalString str = redis_->get(key);
		if (!str)
			return false;

		if (!obj->ParseFromArray(str->c_str(), str->length()))
			return false;

		if (ttl > 0)
		{
			std::chrono::milliseconds t(ttl);
			redis_->pexpire(key, t);
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool RedisClient::add(const char* key, google::protobuf::Message* obj, RedisProtoBufThreadCache* cache, S_TIMESTAMP ttl)
{
	try
	{
		int cs = obj->ByteSizeLong();
		if (!obj->SerializeToArray(cache->data(), cs))
			return false;

		StringView sv(cache->data(), cs);

		std::chrono::milliseconds t(ttl);

		return redis_->set(key, sv, t, UpdateType::ALWAYS);
	}
	catch (...)
	{
		return false;
	}
}