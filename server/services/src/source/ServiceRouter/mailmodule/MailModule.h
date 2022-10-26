#ifndef __MAILMODULE_H__
#define __MAILMODULE_H__

#include <boost/atomic.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include "config/RouterConfig.h"

class MailModule
{
private:
	MailModule();
public:
	static MailModule& instance();

	void init_mailmodule(S_INT_64 myiid);

	void process_mail_msg(S_UINT_16 proiid, BasicProtocol* pro);

protected:

	//receiver hash µ½mail·þÎñ
	int receiver_to_mailhash(S_INT_64 receiver);

private:
	MailHashPlot			mhash_plot_;
};

#endif //__MAILMODULE_H__
