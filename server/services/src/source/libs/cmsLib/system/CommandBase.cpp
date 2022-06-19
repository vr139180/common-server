

#include "cmsLib/system/CommandBase.h"

//-----------------------------------------------NetCommand----------------------------------------------

NetCommand::NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP fun, bool autorelease)
:CommandBase(autorelease), fun_( fun), pro_( d2), fun2_( 0)
{
}

NetCommand::NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP2 fun, int v, bool autorelease)
:CommandBase(autorelease), fun2_( fun), pro_( d2), fun_( 0), fun2_v(v)
{
}

NetCommand::~NetCommand()
{
	if( pro_ && delete_pro_)
		delete pro_;
	pro_ =0;
}

void NetCommand::run()
{
	if( fun_)
		fun_( pro_, delete_pro_);
	else if( fun2_)
		fun2_( pro_, delete_pro_, fun2_v);
}

//-----------------------------------------------NetCommandV----------------------------------------------

NetCommandV::NetCommandV( BasicProtocol *d2, NETCMD_FUN_MAP3 fun, bool autorelease)
:CommandBase(autorelease), fun_( fun), pro_( d2)
{
}

NetCommandV::~NetCommandV()
{
	if( pro_ && delete_pro_)
		delete pro_;
	pro_ =0;
}

void NetCommandV::run()
{
	if( fun_)
		fun_( pro_, delete_pro_, (void*)0);
}
