#include "StdAfx.h"

#include "testcase/BeforeServiceAction.h"

#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/Log.h>
#include <gameLib/protobuf/Proto_all.h>

#include "TestCase.h"

USE_PROTOCOL_NAMESPACE;

//--------------------------------------LoginAction--------------------------------------
LoginAction::LoginAction():CaseActionBase(),
	la_state_( LoginActionState_Idle)
{
}

void LoginAction::build_from_xml_sub( tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);
}

void LoginAction::init_before_start()
{
	CaseActionBase::init_before_start();

	la_state_ =LoginActionState_Idle;
	userid_ =NO_INITVALUE;
}

void LoginAction::start_action()
{
	CaseActionBase::start_action();

	//must finish under 10s
	open_timeout( 10*1000);

	wf_->get_testcase()->reconnect_2_lgs();

	la_state_ =LoginActionState_lgs_conn;
}

void LoginAction::end_action()
{
	CaseActionBase::end_action();
}

void LoginAction::time_out()
{
	if(la_state_ == LoginActionState_lgs_loginreq){
		error_step_ = "lgs time out";
		error_code_ = -1;
	}
	else if(la_state_ == LoginActionState_gts_loginreq){
		error_step_ = "gts time out";
		error_code_ = -1;
	}
}

void LoginAction::update_do( ULONGLONG now)
{
	if( la_state_ == LoginActionState_lgs_conn)
	{
		if( wf_->get_testcase()->is_connected_2_lgs())
		{
			PRO::User_Login_req *req = new PRO::User_Login_req();

			wf_->send_lgs_protocol( req);

			la_state_ =LoginActionState_lgs_loginreq;
		}
		else if(wf_->get_testcase()->is_connect_2_lgs_failed()){
			error_code_ = -1;
			error_step_ = "can't connect to lgs";
			this->end_action();
		}
	}
	else if( la_state_ == LoginActionState_gts_conn)
	{
		wf_->get_testcase()->reconnect_2_gts();
		la_state_ =LoginActionState_gts_conn_wait;
	}
	else if( la_state_ == LoginActionState_gts_conn_wait)
	{
		if( wf_->get_testcase()->is_connected_2_gts())
		{
			PRO::User_ProxyLogin_req* req =new PRO::User_ProxyLogin_req();
			//req->proxy_index_ = proxy_index_;
			//req->userid_ = userid_;
			//req->token_ = token_.c_str();

			wf_->send_gts_protocol( req);

			la_state_ =LoginActionState_gts_loginreq;
		}
		else if( wf_->get_testcase()->is_connect_2_gts_failed())
		{
			error_code_ = -1;
			error_step_ = "can't connect to gts";
			this->end_action();
		}
	}
}

void LoginAction::handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
	if( la_state_ == LoginActionState_lgs_loginreq)
	{
		/*
		if( pro->iid_ == PLAYER_LOGIN_ACK)
		{
			handle =true;

			Player_Login_ack *lack =dynamic_cast<Player_Login_ack*>( pro);
			if( lack->result_ == 0)
			{
				userid_ =lack->userid_;
				token_ =lack->token_.c_str();
				proxy_index_ =lack->proxy_index_;

				vuser_->gts_ip_ =lack->ip_.c_str();
				vuser_->gts_port_ =lack->port_;

				la_state_ =LoginActionState_gts_conn;
			}
			else
            {
                error_code_ = lack->result_;
				error_step_ = "lgs_login";
				end_action();
            }

			wf_->get_testcase()->disconnect_2_lgs();
		}
		*/
	}
	else if( la_state_ == LoginActionState_gts_loginreq)
	{
		/*
		if( pro->iid_ == PLAYER_PROXYLOGIN_ACK)
		{
			handle =true;

			Player_ProxyLogin_ack* ack =dynamic_cast<Player_ProxyLogin_ack*>( pro);
			if( ack->result_ == 0)
			{
				vuser_->userid_ =ack->userid_;
				vuser_->exit_playerid_ =NO_INITVALUE;
				vuser_->login_time_ =TestCaseWorkFlow::GetMillisecondTime();

				for( int ii =0; ii < ack->playerlist_.size(); ++ii)
				{
					PlayerInfo& pi =ack->playerlist_[ii];

					vuser_->chrs_.push_back( pi);

					vuser_->exit_playerid_ =pi.playerid_;

					vuser_->faction_ = pi.faction_;
				}
			}
			else
			{
				error_code_ = ack->result_;
				error_step_ = "gts_login";
			}
			
			end_action();
		}
		*/
	}
}

//--------------------------------------LogoutAction--------------------------------------
LogoutAction::LogoutAction():CaseActionBase()
{
}

void LogoutAction::build_from_xml_sub( tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);
}

void LogoutAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void LogoutAction::start_action()
{
	CaseActionBase::start_action();

	wf_->get_testcase()->disconnect_2_gts();
	vuser_->init_before_start();
}

void LogoutAction::end_action()
{
	CaseActionBase::end_action();
}

void LogoutAction::update_do( ULONGLONG now)
{
	end_action();
}

void LogoutAction::handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
}

//--------------------------------------CreateChrAction--------------------------------------
CreateChrAction::CreateChrAction():CaseActionBase()
{
}

void CreateChrAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);
}

void CreateChrAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void CreateChrAction::start_action()
{
	CaseActionBase::start_action();

	/*
	Client_CreatePlayer_Req* req =new Client_CreatePlayer_Req();

	std::stringstream ss;
	ss << "robot_";
	ss << vuser_->user_name_;

	req->playername_ = ss.str().c_str();
	req->gender_ =0;
	req->icon_ =0;
	req->faction_ =get_params_int( "faction", 1);

	wf_->send_gts_protocol( req);
	*/
}

void CreateChrAction::end_action()
{
	CaseActionBase::end_action();
}

void CreateChrAction::update_do( ULONGLONG now)
{
}

void CreateChrAction::handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
	/*
	if( pro->iid_ == CLIENT_CREATEPLAYER_ACK)
	{
		handle =true;

		Client_CreatePlayer_Ack* ack =dynamic_cast<Client_CreatePlayer_Ack*>(pro);

		if( ack->result_ == 0)
		{
			for( int ii =0; ii < ack->playerlist_.size(); ++ii)
			{
				PlayerInfo& pi =ack->playerlist_[ii];

				vuser_->chrs_.push_back( pi);

				vuser_->exit_playerid_ =pi.playerid_;
			}
		}
        else
        {
            error_code_ = ack->result_;
			error_step_ = "create_player";
        }

		end_action();
	}
	*/
}

//--------------------------------------SelChrAction--------------------------------------
SelChrAction::SelChrAction():CaseActionBase()
{
}

void SelChrAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);
}

void SelChrAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void SelChrAction::start_action()
{
	CaseActionBase::start_action();

	//Client_PlayerSel_Req* req =new Client_PlayerSel_Req();
	//req->playerid_ =vuser_->exit_playerid_;

	//wf_->send_gts_protocol( req);
}

void SelChrAction::end_action()
{
	CaseActionBase::end_action();
}

void SelChrAction::update_do( ULONGLONG now)
{
}

void SelChrAction::handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
	end_action();
}
