#include "StdAfx.h"

#include "VirtualUser.h"

#include "testcase/core/TestCaseWorkFlow.h"

VirtualUser::VirtualUser():
	userid_( NO_INITVALUE),
	chrid_( NO_INITVALUE),
	exit_playerid_( NO_INITVALUE)
{
	context_.init_scriptcontext();

	context_.regist_2_context<VirtualUser>( "vuser", this);
	context_.regist_2_context<CaseConditionResult>( "result", &cond_result_);
}

VirtualUser::~VirtualUser()
{
	context_.uninit_scriptcontext();
}

void VirtualUser::init_before_start()
{
	userid_ =NO_INITVALUE;
	exit_playerid_ =NO_INITVALUE;
	chrid_ =NO_INITVALUE;

	login_time_ =NO_INITVALUE;
}

int VirtualUser::logintime()
{
	if( login_time_ == NO_INITVALUE)
		return 0;

	return (int)(TestCaseWorkFlow::GetMillisecondTime() - login_time_);
}

bool VirtualUser::islogin()
{
	return userid_ != NO_INITVALUE;
}

bool VirtualUser::has_chr()
{
	return exit_playerid_ != NO_INITVALUE;
}

bool VirtualUser::is_ready()
{
	return chrid_ != NO_INITVALUE;
}
