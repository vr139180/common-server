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

#ifndef __NETDRIVERX_H__
#define __NETDRIVERX_H__

#include "cmsLib/net/MutexAllocator.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class NetDriverX
{
public:
	typedef boost::shared_ptr<boost::asio::io_service>			IOServicePtr_t;
	typedef boost::shared_ptr<boost::asio::io_service::work>    WorkPtr_t;

public:
	virtual ~NetDriverX();
	static NetDriverX& getInstance();

	bool initNetDriver( int threadNums);
	void uninitNetDriver();

	boost::asio::io_service& get_ioservice()
	{
		if( index_next_ioservice_ >= io_services_.size())
			index_next_ioservice_ = 0;

		return *io_services_[index_next_ioservice_++];
	}

protected:

private:
	NetDriverX();

	void alloc_ioservices();
	void start_netthreads();

private:
	//init flags
	bool							bInitialized_;

	//ioservice static array
	u32								thread_nums_;
	std::vector<boost::shared_ptr<boost::thread> >	threads_;

	std::vector<IOServicePtr_t>     io_services_;
	std::vector<WorkPtr_t>          io_works_;
	u32                             index_next_ioservice_;
};

#endif	//__NETDRIVERX_H__
