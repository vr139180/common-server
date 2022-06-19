#ifndef __REDISCLIENT_H__
#define __REDISCLIENT_H__

#include <sw/redis++/redis++.h>
#include <boost/json.hpp>
#include <boost/unordered_map.hpp>
#include <google/protobuf/message.h>

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/redis/RedisProtoBufThreadCache.h>

using namespace sw::redis;

class RedisClient;

class IRedisSerializer
{
	friend class RedisClient;

protected:
	virtual bool to_json(std::string& val) = 0;
	virtual bool from_json(boost::json::value& root) = 0;
};

class RedisClient
{
public:
	RedisClient();
	virtual ~RedisClient();

	/**
	* ≥ı ºªØ¡¥Ω”
	**/
	void init_redis(std::string ip, int port, std::string pwd, int db, 
		int sockettimeout = 100, int pool_size =1);

public:
	//hash object
	template<typename T>
	bool get_hashobject(const char* hkey, const char* field, T& obj);
	template<typename T>
	bool get_hashallobj(const char* hkey, boost::unordered_map<std::string,T*>& vals);

	template<class T>
	bool set_hashobject(const char* hkey, const char* field, T& obj);

	bool del_hashmember(const char* hkey, const char* field);
	bool set_hashmember(const char* hkey, const char* field, const char* val);
	bool set_hashmember_ul(const char* hkey, const char* field, S_INT_64 val);

	template<typename T>
	bool set_hashmember(const char* hkey, std::initializer_list<T> il);
	bool get_hashmember(const char* hkey, const char* field, std::string& val);
	bool get_hashmember_l(const char* hkey, const char* field, long& val);
	bool get_hashmember_ul(const char* hkey, const char* field, S_INT_64& val);

	bool inc_hashmemeber(const char* hkey, const char* field, S_INT_64& val, S_INT_64 incnum = 1);

public:
	//protobuf hash object
	bool set_hashobject(const char* hkey, const char* field, 
		google::protobuf::Message* obj, RedisProtoBufThreadCache* cache);
	bool get_hashobject(const char* hkey, const char*field, google::protobuf::Message* obj);

public:
	//zset object
	bool add_zset(const char* hset, const char* key, S_INT_64 val, UpdateType flag = UpdateType::ALWAYS);
	bool del_zsetmember(const char* hset, const char* key);
	bool getbyscore_zset(const char* hset, S_INT_64 minval, std::vector<std::string>& keys);
	bool get_zscore(const char* hset, const char* member, S_INT_64& val);

public:
	//set object
	bool add_set(const char* key, const char* member);
	bool del_setmember(const char* key, const char* member);
	bool pop_one(const char* key, std::string& member);

	int get_set_size(const char* key);

public:
	//key
	bool del(const char* key);

	bool pexpire(const char* key, S_TIMESTAMP ttl);
	bool persist(const char* key);

	//ttl == 0 no expire, million second
	template<typename T>
	bool add(const char* key, T& obj, S_TIMESTAMP ttl, UpdateType flag = UpdateType::ALWAYS);

	template<typename T>
	bool get(const char* key, T& obj);

	bool get(const char* key, google::protobuf::Message* obj, S_TIMESTAMP ttl = 0);
	//ttl == 0 no expire, million second
	bool add(const char* key, google::protobuf::Message* obj, RedisProtoBufThreadCache* cache, S_TIMESTAMP ttl = 0);

public:
	bool evalStrs(const char* sc, std::initializer_list<StringView> keys, std::initializer_list<StringView> vals,
		std::vector<std::string>& rets);

protected:
	template<typename T>
	bool json_to_object(const char* str, T& obj);

public:
	S_INT_64 gen_uuid_from_redis(const char* hkey, const char* uuidkey);
	S_INT_64 gen_dbid_from_redis(const char* hkey, const char* fieldfmt, const char* tablename);
	std::string build_rediskey(const char* fmt, ...);

protected:
	sw::redis::Redis* redis_;
};

template<typename T>
bool RedisClient::json_to_object(const char* str, T& obj)
{
	boost::json::parse_options opt;
	opt.allow_comments = true;
	opt.allow_trailing_commas = true;

	boost::json::error_code ec;
	boost::json::value root = boost::json::parse(str, ec, {}, opt);
	if (ec.failed())
		return false;

	IRedisSerializer* iis = reinterpret_cast<IRedisSerializer*>(&obj);
	return iis->from_json(root);
}

template<typename T>
bool RedisClient::get_hashobject(const char* hkey, const char* field, T& obj)
{
	try
	{
		OptionalString str = redis_->hget(hkey, field);
		if (!str)
			return false;

		return json_to_object<T>(str->c_str(), obj);
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
bool RedisClient::get_hashallobj(const char* hkey, boost::unordered_map<std::string, T*>& vals)
{
	try {
		std::vector< std::pair<std::string, std::string>> vecs;
		redis_->hgetall(hkey, std::back_inserter(vecs));
		for (int ii = 0; ii < vecs.size(); ++ii)
		{
			std::pair<std::string, std::string>& v = vecs[ii];
			std::string& n = v.first;
			std::string& xv = v.second;

			T* val = new T();
			IRedisSerializer* iis = reinterpret_cast<IRedisSerializer*>(val);
			json_to_object<T>( xv.c_str(), *val);

			vals[n] = val;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

template<class T>
bool RedisClient::set_hashobject(const char* hkey, const char* field, T& obj)
{
	try
	{
		IRedisSerializer* iis = reinterpret_cast<IRedisSerializer*>(&obj);
		std::string str = "";
		if (!iis->to_json( str))
			return false;

		redis_->hset(hkey, field, str);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
bool RedisClient::set_hashmember(const char* hkey, std::initializer_list<T> il)
{
	try
	{
		redis_->hmset(hkey, il);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
bool RedisClient::add(const char* key, T& obj, S_TIMESTAMP ttl, UpdateType flag)
{
	try
	{
		IRedisSerializer* iis = reinterpret_cast<IRedisSerializer*>(&obj);
		std::string str = "";
		if (!iis->to_json(str))
			return false;

		std::chrono::milliseconds t(ttl);
		return redis_->set(key, str, t, flag);
	}
	catch (...)
	{
		return false;
	}
}

template<typename T>
bool RedisClient::get(const char* key, T& obj)
{
	try
	{
		OptionalString str = redis_->get(key);
		if (!str)
			return false;

		return json_to_object<T>(str->c_str(), obj);
	}
	catch (...)
	{
		return false;
	}
}


#endif //__REDISCLIENT_H__
