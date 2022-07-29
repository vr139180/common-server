#include "friends/FriendModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include "RouterServiceApp.h"

USE_PROTOCOL_NAMESPACE

void FriendModule::process_friend_msg(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	S_INT_64 uid = 0;
	ProtoUtil::get_useriid_from_token(pro, uid);

	logDebug(out_runtime, "resend msg:%d to friend service\r\n", proiid);

	int frdhash = user_to_frdhash(uid);
	svrApp.send_protocal_to_friend(frdhash, p_msg.release());
}
