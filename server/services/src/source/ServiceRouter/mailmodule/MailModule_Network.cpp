#include "mailmodule/MailModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "ServiceRouterApp.h"

USE_PROTOCOL_NAMESPACE

void MailModule::process_mail_msg(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == MAIL_PROTYPE::MAIL_SYSTEMMAIL_REQ)
	{
		svrApp.send_protocal_to_mail_circle(p_msg.release());
	}
	else
	{
		S_INT_64 uid = 0;

		int mailhash = receiver_to_mailhash(uid);
		svrApp.send_protocal_to_mail(mailhash, p_msg.release());
	}
}
