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
