#include "player/GamePlayerCtrl.h"

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/eureka/EurekaClusterClient.h>

#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void GamePlayerCtrl::chat_globalmsg_notify(NetProtocol* pro)
{
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		NetProtocol* msg = 0;
		if (ii < (channel_nums_ - 1))
		{
			msg = pro->clone();
		}
		else
			msg = pro;

		NETCMD_FUN_MAP fun = boost::bind(&PlayerChannel::NetProcessMessage, &(all_channels_[ii]),
			boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(msg, fun);
		all_channels_[ii].regist_netcmd(pcmd);
	}
}
