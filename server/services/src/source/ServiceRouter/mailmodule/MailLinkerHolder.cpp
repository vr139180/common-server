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

#include "mailmodule/MailLinkerHolder.h"

MailLinkerHolder::MailLinkerHolder():base()
, cur_mail_service_(0)
{
}

void MailLinkerHolder::uninit_holder()
{
	mails_vector_.clear();
	hash_service_.clear();

	base::uninit_holder();
}

void MailLinkerHolder::return_freelink(MailServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	//hash services
	int cid = link->get_mailhash();
	if (cid != -1)
	{
		//针对多个相同mailhash的mail服务切换时造成的
		boost::unordered_map<int, MailServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			MailServiceLinkFrom* p = fiter->second;
			if (p == link)
				hash_service_.erase(fiter);
		}
	}
	
	std::vector<MailServiceLinkFrom*>::iterator fiter2 = std::find(mails_vector_.begin(), mails_vector_.end(), link);
	if (fiter2 != mails_vector_.end())
		mails_vector_.erase(fiter2);

	online_links_.erase(link->get_iid());
	free_links_.insert(link);
}

MailServiceLinkFrom* MailLinkerHolder::regist_onlinelink(MailServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_[link->get_iid()] = link;

	bool need_add = false;
	int cid = link->get_mailhash();
	if (cid != -1)
	{
		//针对多个相同mailhash的mail服务切换时造成的
		boost::unordered_map<int, MailServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			MailServiceLinkFrom* p = fiter->second;
			//iid大的表示后续启动的，替换前次启动的，每个mailhash服务只能存在一个
			if (link->get_iid() > p->get_iid())
			{
				hash_service_[cid] = link;
				need_add = true;
			}
		}
		else
		{
			hash_service_[cid] = link;
			need_add = true;
		}
	}

	if (need_add)
	{
		std::vector<MailServiceLinkFrom*>::iterator fiter2 = std::find(mails_vector_.begin(), mails_vector_.end(), link);
		if (fiter2 == mails_vector_.end())
			mails_vector_.push_back(link);
	}

	return link;
}

void MailLinkerHolder::send_mth_protocol(int chathash, BasicProtocol* pro)
{
	ThreadLockWrapper guard(lock_);

	boost::unordered_map<int, MailServiceLinkFrom*>::iterator fiter = hash_service_.find(chathash);
	if (fiter == hash_service_.end())
	{
		delete pro;
		return;
	}

	MailServiceLinkFrom* p = fiter->second;
	p->send_netprotocol(pro);
}

void MailLinkerHolder::send_mth_protocol_circle(BasicProtocol* pro)
{
	ThreadLockWrapper guard(lock_);

	if (mails_vector_.size() == 0)
	{
		delete pro;
		return;
	}

	if (cur_mail_service_ >= mails_vector_.size())
		cur_mail_service_ = 0;

	MailServiceLinkFrom* p = mails_vector_[cur_mail_service_];
	p->send_netprotocol(pro);

	++cur_mail_service_;
}