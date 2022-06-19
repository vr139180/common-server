#ifndef __NETLINKFROMBASE_H__
#define __NETLINKFROMBASE_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

template<typename T>
class NetLinkFromBase
{
public:
	NetLinkFromBase();
	virtual ~NetLinkFromBase();

	void reset();

	void force_close();

	bool is_valid(){ return session_ != 0;}
	virtual void registinfo_tolog( bool bregist =true){}

	void set_session(T* s){ session_ =s; }
	T* get_session(){ return session_; }

	void set_iid(S_INT_64 id) { iid_ = id; }
	virtual S_INT_64 get_iid() { return iid_; }
	//void set_token(S_INT_64 token) { token_ = token; }
	virtual S_INT_64 get_token() { return token_; }
	void set_exts(boost::unordered_map<std::string, std::string>& e);
	void set_linkbase_info(S_INT_64 id, S_INT_64 token, const google::protobuf::Map<std::string, std::string>& e);
	void set_linkbase_info(S_INT_64 id, S_INT_64 token);

	virtual void force_linkclose() = 0;

	std::string get_ext_bykey(const char* key);

	void send_protocol( BasicProtocol* pro){
		std::auto_ptr<BasicProtocol> p_msg( pro);
		if( session_ == 0)
			return;

		session_->send_protocol( p_msg.release());
	}

	void heart_beat(){
		if( session_ != 0)
			session_->heart_beat();
	}

public:
	//------------------------------------NetSessionBindEvent-----------------------------
	virtual void on_cant_connectedto() {}
	virtual void on_connectedto_done() {}

	virtual void on_connect_lost_netthread() = 0;
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro) = 0;

protected:
	T*	session_;
	S_INT_64 iid_;
	S_INT_64 token_;

	boost::unordered_map<std::string, std::string> exts_;
};

template<typename T>
NetLinkFromBase<T>::NetLinkFromBase():session_(0)
{
}

template<typename T>
NetLinkFromBase<T>::~NetLinkFromBase()
{}

template<typename T>
void NetLinkFromBase<T>::set_linkbase_info(S_INT_64 id, S_INT_64 token)
{
	this->iid_ = id;
	this->token_ = token;
}

template<typename T>
void NetLinkFromBase<T>::set_linkbase_info(S_INT_64 id, S_INT_64 token,
	const google::protobuf::Map<std::string, std::string>& e)
{
	this->iid_ = id;
	this->token_ = token;

	if (e.size() > 0)
	{
		boost::unordered_map<std::string, std::string> ets( e.begin(), e.end());
		this->set_exts(ets);
	}
}

template<typename T>
void NetLinkFromBase<T>::set_exts(boost::unordered_map<std::string, std::string>& e)
{
	exts_.clear();
	exts_ = e;
}

template<typename T>
std::string NetLinkFromBase<T>::get_ext_bykey(const char* key)
{
	boost::unordered_map<std::string, std::string>::iterator fiter = exts_.find(key);
	if (fiter == exts_.end())
		return "";
	return fiter->second;
}

template<typename T>
void NetLinkFromBase<T>::reset()
{
	session_ = 0;
	exts_.clear();
}

template<typename T>
void NetLinkFromBase<T>::force_close()
{
	if (session_ != 0)
		session_->force_close();
}

#endif	//__NETLINKFROMBASE_H__
