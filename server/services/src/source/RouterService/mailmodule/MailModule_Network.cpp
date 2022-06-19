#include "mailmodule/MailModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include "RouterServiceApp.h"

USE_PROTOCOL_NAMESPACE

void MailModule::process_mail_msg(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
	if (proiid == MAIL_PROTYPE::MAIL_SYSTEMMAIL_REQ)
	{
		svrApp.send_protocal_to_mail_circle(p_msg.release());
	}
	else
	{
		S_INT_64 uid = 0;
		ProtoUtil::get_useriid_from_token(pro, uid);

		int mailhash = receiver_to_mailhash(uid);
		svrApp.send_protocal_to_mail(mailhash, p_msg.release());
	}
}
