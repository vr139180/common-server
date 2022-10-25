#ifndef __LINKTOBASE_H__
#define __LINKTOBASE_H__

#include <cmsLib/net/NetConnector.h>
#include <cmsLib/net/NetSession.h>
#include <cmsLib/net/NetSessionBindEvent.h>
#include <cmsLib/prolib/BasicProtocol.h>

class LinkToBase : public NetSessionBindEvent
{
public:
	LinkToBase();
	virtual ~LinkToBase();

	bool is_connected();
	bool is_connecting();

	bool is_authed(){ return is_authed_; }
	void set_authed( bool v){ is_authed_ =v; }
	
	void connect_to( const char* ip, int port);

	void send_protocol( NetProtocol* pro){ session_->send_protocol( pro); }

	virtual std::string get_ext_bykey(const char* key) = 0;
	virtual void force_linkclose() = 0;
	//初始化协议头
	virtual void init_protocolhead() = 0;
	virtual const SProtocolHead& get_protocolhead() = 0;

	void force_close();

	void heart_beat(){
		if( session_.get() != 0)
			session_->heart_beat();
	}

public:
	//------------------------NetSessionBindEvent-----------------------------------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread() = 0;
	virtual void on_recv_protocol_netthread(NetProtocol* pro) = 0;

private:
	//physical link success
	bool								is_connected_;
	bool								is_authed_;
	//connector
	NetConnector						connector_;
	//bind session
	boost::shared_ptr<NetSession>		session_;
};

#endif	//__LINKTOBASE_H__
