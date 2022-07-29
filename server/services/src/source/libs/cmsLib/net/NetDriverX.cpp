#include "cmsLib/net/NetDriverX.h"

#include <boost/thread.hpp>

#include <cmsLib/Log.h>

NetDriverX::NetDriverX():
bInitialized_(false),
index_next_ioservice_(0),
thread_nums_(0)
{
}

NetDriverX::~NetDriverX()
{
}

NetDriverX& NetDriverX::getInstance()
{
	static NetDriverX s_netdriver_;
	return s_netdriver_;
}

bool NetDriverX::initNetDriver( int threadNums)
{
	if( bInitialized_)
		return false;

	thread_nums_ =threadNums;
	if( thread_nums_ <= 0)
		thread_nums_ =1;

	alloc_ioservices();

	//start threads
	start_netthreads();

	bInitialized_ =true;

	return bInitialized_;
}

void NetDriverX::uninitNetDriver()
{
	if( !bInitialized_)
		return;

	//ioservice stop
	for( u32 i = 0; i < io_services_.size(); i++)
		io_services_[i]->stop();

	for( u32 i = 0; i < threads_.size(); i++)
		threads_[i]->join();

	bInitialized_ =false;
}

void NetDriverX::alloc_ioservices()
{
	for( u32 i = 0; i < thread_nums_; i++)
	{
		IOServicePtr_t ioService( new boost::asio::io_service );
		WorkPtr_t work( new boost::asio::io_service::work(*ioService) );

		io_services_.push_back( ioService);
		io_works_.push_back( work);
	}
}

void NetDriverX::start_netthreads()
{
	for( u32 i = 0; i < io_services_.size(); i++ )
	{
		boost::shared_ptr<boost::thread> thread( 
			new boost::thread( boost::bind( &boost::asio::io_service::run, io_services_[i])));
		threads_.push_back(thread);
		//logDebug(out_runtime, "netdriver- netthread id:0x%x", thread->get_id());
	}
}
