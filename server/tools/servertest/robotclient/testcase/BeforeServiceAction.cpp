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

#include "StdAfx.h"

#include "testcase/BeforeServiceAction.h"

#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/Log.h>
#include <gameLib/protobuf/Proto_all.h>

#include "TestCase.h"

USE_PROTOCOL_NAMESPACE;

//--------------------------------------ServerConnectAction--------------------------------------
ServerConnectAction::ServerConnectAction() :CaseActionBase()
{
}

void ServerConnectAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub(act);

	time_out_step_ = get_params_int("timeout", 0);
}

void ServerConnectAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void ServerConnectAction::start_action()
{
	CaseActionBase::start_action();

	if (time_out_step_ > 0)
		open_timeout(time_out_step_);

	vuser_->gts_connect_result(false);

	vuser_->random_svrinfo();
	wf_->get_testcase()->try_connect_to_gts();
}

void ServerConnectAction::end_action()
{
	CaseActionBase::end_action();
}

void ServerConnectAction::time_out()
{
	error_code_ = -1;
}

void ServerConnectAction::update_do(ULONGLONG now)
{
	if (wf_->get_testcase()->is_connected_2_gts())
	{
		vuser_->gts_connect_result(true);
		this->end_action();
		return;
	}
	else if (wf_->get_testcase()->is_connect_2_gts_failed())
	{
		vuser_->gts_connect_result(false);
		error_code_ = wf_->get_testcase()->get_gts_errcode();
		this->end_action();
		return;
	}
}

//--------------------------------------LoginAction--------------------------------------
LoginAction::LoginAction():CaseActionBase(),
	la_state_( LoginActionState_Idle)
{
}

void LoginAction::build_from_xml_sub( tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);

	time_out_step_ = get_params_int("timeout", 0);
}

void LoginAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

S_UINT_32 LoginAction::get_req_protoid()
{
	return 0;
}

void LoginAction::start_action()
{
	CaseActionBase::start_action();

	//must finish under 10s
	if (time_out_step_ > 0)
		open_timeout(time_out_step_);

	wf_->send_gts_protocol(0);
}

void LoginAction::end_action()
{
	CaseActionBase::end_action();
}

void LoginAction::time_out()
{
	error_code_ = -1;
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
