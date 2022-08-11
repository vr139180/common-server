#pragma once

#include <cmsLib/prolib/ProtocolFactory.h>
#include <gameLib/protobuf/Proto_all.h>

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

	void send_lgs_protocol( BasicProtocol *p);
	void recv_lgs_protocol();

	void send_gts_protocol( BasicProtocol *p);
	void recv_gts_protocol();

	bool force_close();

	void set_userrange( int id, int range, int svrno);
	void set_userid();
	int get_userid();

	void set_cancel(){ cancel_ =true;};

	void set_delay( ULONGLONG t){ delay_connect_ =t;};

	bool connect_2_lgs( int& err);
	bool connect_2_gts( int& err);

	void disconnect_2_lgs();
	void disconnect_2_gts();

	void reconnect_2_lgs();
	void reconnect_2_gts();

	bool is_connected_2_lgs(){
		return lgs_socket_ != INVALID_SOCKET;
	}

	bool is_connected_2_gts(){
		return gts_socket_ != INVALID_SOCKET;
	}

	bool is_connect_2_gts_failed(){
		return is_connect_2_gts_failed_;
	}

	bool is_connect_2_lgs_failed(){
		return is_connect_2_lgs_failed_;
	}

	void wait_socket_close();

	void set_ip( const char* ip, int port){
		lgs_ip_ =ip;
		lgs_port_ =port;
	}

public:
	NS_STL::string		lgs_ip_;
	S_INT_32			lgs_port_;

protected:
	bool				cancel_;

	int					userid_;
	int					userrange_;
	int					cur_user_pos_;
	int					svrno_;

	SOCKET				lgs_socket_;
	char				lgs_recv_buffer_[MAX_PACK_LEN];
	int					lgs_data_len_;

	SOCKET				gts_socket_;
	char				gts_recv_buffer_[MAX_PACK_LEN];
	int					gts_data_len_;

	bool				need_connect_2_lgs_;
	bool				need_connect_2_gts_;
	bool				is_connect_2_gts_failed_;
	bool				is_connect_2_lgs_failed_;
	ULONGLONG			start_connect_t_;
	ULONGLONG			delay_connect_;

	TestCaseWorkFlow	workflow_;
	VirtualUser			vuser_;
	
	ProtocolFactory		*data_;
};
