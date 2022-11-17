// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::mail_user_active()
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_UserOnline_active *act = new Mail_UserOnline_active();
	act->set_last_mailiid(0);
	act->set_firstactive(true);
	send_to_gts(act);
}

void CommandTestImpl::on_mail_newmail_ntf(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_NewMail_ntf *ack = dynamic_cast<Mail_NewMail_ntf*>(pro);
	CString str1;
	str1.Format("mail- you have new mail arrived \r\n");
	*pRetMsg += str1;
}

void CommandTestImpl::mail_new_systemmail(const char* title, const char* content)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_SystemMail_req *req = new Mail_SystemMail_req();
	req->set_sender_iid(0);
	req->set_attachment(0);
	req->set_attachinfo("");
	req->set_title(title);
	req->set_contents(content);

	send_to_gts(req);
}

void CommandTestImpl::on_mail_new_systemmail_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_SystemMail_ack *ack = dynamic_cast<Mail_SystemMail_ack*>(pro);

	CString str1;
	str1.Format("mail- new system mail result:%d \r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::mail_new_usermail(const char* title, const char* content)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_NewMail_req *req = new Mail_NewMail_req();
	req->set_stype(2);
	req->set_receiver_iid(this->role_iid_);
	req->set_sender_iid(this->role_iid_);
	req->set_attachment(0);
	req->set_attachinfo("");
	req->set_title(title);
	req->set_contents(content);

	send_to_gts(req);
}

void CommandTestImpl::on_mail_new_usermail_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_NewMail_ack *ack = dynamic_cast<Mail_NewMail_ack*>(pro);

	CString str1;
	str1.Format("mail- new user mail result:%d \r\n", ack->result());
	*pRetMsg += str1;
	if (ack->result() == 0)
	{
		const MailNormalItem& mail = ack->mail();
		str1.Format("	mail:%lld title:%s\r\n", mail.iid(), mail.title().c_str());
		*pRetMsg += str1;
	}
}

void CommandTestImpl::mail_getmail(S_INT_64 mailiid, bool next)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_MailGet_req *req = new Mail_MailGet_req();
	req->set_mailiid(mailiid);
	req->set_next(next);
	req->set_num(20);

	send_to_gts(req);
}

void CommandTestImpl::on_mail_getmail_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_MailGet_ack *ack = dynamic_cast<Mail_MailGet_ack*>(pro);
	
	CString str1;
	str1.Format("mail- query user mail totle:%d return:%d \r\n", ack->totle(), ack->mails_size());
	*pRetMsg += str1;

	for (int ii = 0; ii < ack->mails_size(); ++ii)
	{
		const MailNormalItem& mi = ack->mails(ii);
		str1.Format("\tmail:%lld title:%s\r\n", mi.iid(), mi.title().c_str());
		*pRetMsg += str1;
	}
}

void CommandTestImpl::mail_read(S_INT_64 mailiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_ReadMail_req *req = new Mail_ReadMail_req();
	req->set_mail_iid(mailiid);

	send_to_gts(req);
}

void CommandTestImpl::on_mail_read_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_ReadMail_ack *ack = dynamic_cast<Mail_ReadMail_ack*>(pro);

	CString str1;
	str1.Format("mail- read user mail result:%d \r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::mail_delete(S_INT_64 mailiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Mail_DeleteMail_req *req = new Mail_DeleteMail_req();
	req->set_mail_iid(mailiid);

	send_to_gts(req);
}
void CommandTestImpl::on_mail_delete_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Mail_DeleteMail_ack *ack = dynamic_cast<Mail_DeleteMail_ack*>(pro);

	CString str1;
	str1.Format("mail- delete user mail result:%d \r\n", ack->result());
	*pRetMsg += str1;
}