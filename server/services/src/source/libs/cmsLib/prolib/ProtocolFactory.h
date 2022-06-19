/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2021
*/
#ifndef __PROTOCOLFACTORY__H__
#define __PROTOCOLFACTORY__H__

#include <boost/unordered_map.hpp>
#include <string>
#include <cmsLib/prolib/core_type.h>
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
	BasicProtocol* iid_to_proto(S_UINT_16 proiid, char *pdata, S_UINT_16 len);

protected:
	void release();

	template<typename T>
	void regist_protocal(S_UINT_16 proiid);

private:
	boost::unordered_map<S_UINT_16, const BasicProtocol*>	iid_2_proto;
	boost::unordered_map<const std::string, S_UINT_16>		name_2_iid;
};

template<typename T>
void ProtocolFactory::regist_protocal(S_UINT_16 proiid)
{
	const BasicProtocol* pro = &(T::default_instance());

	iid_2_proto[proiid] = pro;
	name_2_iid[pro->GetTypeName()] = proiid;
}

#endif //__PROTOCOLFACTORY__H__
