#include "cmsLib/system/CommandBase.h"

//-----------------------------------------------NetCommand----------------------------------------------

NetCommand::NetCommand(NetProtocol *d2, NETCMD_FUN_MAP fun, bool autorelease)
:CommandBase(autorelease), fun_( fun), pro_( d2)
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
}

//-----------------------------------------------NetCommandV----------------------------------------------

NetCommandV::NetCommandV(NetProtocol *d2, NETCMD_FUN_MAP3 fun, bool autorelease)
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

//-----------------------------------------------NetCommandV2----------------------------------------------

NetCommandV2::NetCommandV2(NetProtocol *d2, NETCMD_FUN_MAP2 fun, bool autorelease)
	:CommandBase(autorelease), fun_(fun), pro_(d2)
{
}

NetCommandV2::~NetCommandV2()
{
	if (pro_ && delete_pro_)
		delete pro_;
	pro_ = 0;
}

void NetCommandV2::run()
{
	if (fun_)
		fun_(pro_, delete_pro_, 0);
}
