#ifndef __BEFORESERVICEACTION_H__
#define __BEFORESERVICEACTION_H__

#include "testcase/core/CaseActionBase.h"

class LoginAction : public CaseActionBase
{
	typedef enum enum_LoginAction_State
	{
		LoginActionState_Idle =0,
		LoginActionState_lgs_conn,
		LoginActionState_lgs_loginreq,
		LoginActionState_gts_conn,
		LoginActionState_gts_conn_wait,
		LoginActionState_gts_loginreq,
	}LoginAction_State;

public:
	static std::string get_done_name(){ return "login";}

public:
	LoginAction();

	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

	virtual void time_out();

protected:
	virtual void build_from_xml_sub( tinyxml2::XMLElement* act);

private:
	LoginAction_State	la_state_;

	S_INT_32		proxy_index_;
	std::string		token_;
	int				userid_;
};

class LogoutAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "logout";}

public:
	LogoutAction();

	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

class SelChrAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "selchr";}

public:
	SelChrAction();

	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

class CreateChrAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "createchr";}

public:
	CreateChrAction();

	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

#endif	//__BEFORESERVICEACTION_H__
