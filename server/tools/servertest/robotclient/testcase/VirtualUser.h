#ifndef __VIRTUALUSER_H__
#define __VIRTUALUSER_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <cmsLib/lua/ScriptContext.h>

#include <gameLib/protobuf/Proto_all.h>

#include <map>
#include <string>
#include <list>

USE_PROTOCOL_NAMESPACE

class CaseConditionResult
{
public:
	CaseConditionResult():succ(false){}

	void reset(){ succ =false; }
public:
	bool succ;
};

class VirtualUser
{
public:
	static void InitScriptBind( lua_State* l);

	VirtualUser();
	virtual ~VirtualUser();

	void init_before_start();

	bool run_script( const char* cmd);

public:
	//script support function or property
	int  logintime();
	bool islogin();
	bool has_chr();
	bool is_ready();

public:
	ULONGLONG		login_time_;

	S_INT_32		userid_;
	S_INT_32		exit_playerid_;
	S_UINT_8		faction_;
	S_INT_32		chrid_;
	std::string		chrname_;

public:
	std::string		user_name_;
	std::string		user_pwd_;
	std::string		gts_ip_;
	S_INT_16		gts_port_;

protected:
	ScriptContext		context_;
	CaseConditionResult	cond_result_;
};

#endif	//__VIRTUALUSER_H__
