#ifndef __MAILLINKERHOLDER_H__
#define __MAILLINKERHOLDER_H__

#include <gameLib/commons/LinkFromHolder.h>
#include "network/MailServiceLinkFrom.h"

class MailLinkerHolder : public LinkFromHolder<MailServiceLinkFrom>
{
	typedef LinkFromHolder<MailServiceLinkFrom> base;
public:
	MailLinkerHolder();

	virtual void uninit_holder();

	virtual void return_freelink(MailServiceLinkFrom* link);
	virtual MailServiceLinkFrom* regist_onlinelink(MailServiceLinkFrom* link);

	void send_mth_protocol(int mailhash, BasicProtocol* pro);
	void send_mth_protocol_circle(BasicProtocol* pro);

protected:
	boost::unordered_map<int, MailServiceLinkFrom*>	hash_service_;
	std::vector<MailServiceLinkFrom*>	mails_vector_;
	int cur_mail_service_;
};

#endif //__CHATLINKERHOLDER_H__
