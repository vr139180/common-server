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

#include <cmsLib/net/NetSession.h>
#include <cmsLib/Log.h>
#include <cmsLib/base/Profile.h>

NetSession::NetSession( ThreadLock* lock, ThreadLock* queuelock, NetSessionBindEvent* cb):
lock_(lock),
queue_lock_( queuelock),
lock_delete_( false),
bind_event_cb_( cb),
is_initialized( false)
{
	if( lock_ == 0)
	{
		lock_ =new ThreadLock();
		queue_lock_ =new ThreadLock();

		lock_delete_ =true;
	}

	//only use to do reset
	is_initialized =true;

	this->reset_nomutex();
}

NetSession::~NetSession()
{
	{
		ThreadLockWrapper guard( *lock_);

		this->reset_nomutex();
	}

	if( lock_delete_ && lock_)
		delete lock_;
	lock_ =0;

	if( lock_delete_ && queue_lock_)
		delete queue_lock_;
	queue_lock_ =0;
}

std::string NetSession::get_ip()
{
	if( socket_.get() != 0)
		return socket_->remote_endpoint().address().to_string();
	else
		return "";
}

void NetSession::attachToSocket( boost::asio::io_service& ios)
{
	ThreadLockWrapper guard( *lock_);

	//only use to do reset
	is_initialized =true;

	this->reset_nomutex();

	this->io_service_ =&ios;
	assert( io_service_);

	socket_.reset( new boost::asio::ip::tcp::socket( ios));
}

void NetSession::force_reset()
{
	ThreadLockWrapper guard( *lock_);

	this->reset_nomutex();
}

void NetSession::reset_nomutex()
{
	if( !is_initialized)
		return;

	is_initialized =false;

	is_sending_ =false;
	send_buff_pos_ =0;

	recv_buff_pos_ =0;

	if( socket_ != 0 && socket_->is_open())
	{
		boost::system::error_code err;
		socket_->close( err);
	}

	socket_.reset();
	io_service_ =0;

	this->freeSendQueue();
}

void NetSession::freeSendQueue()
{
	ThreadLockWrapper guard( *queue_lock_);

	for (NetProtocolQueueIt_t it = send_queue_.begin(); it != send_queue_.end(); ++it)
	{
		NetProtocol* pMessage = (*it);
		if (pMessage != NULL)
			delete pMessage;
	}

	send_queue_.clear();
}

void NetSession::on_connectto_result(bool success)
{
	dealwith_connect_result(success);

	if (bind_event_cb_)
	{
		if (success)
			bind_event_cb_->on_connectedto_done();
		else
			bind_event_cb_->on_cant_connectedto();
	}
}

void NetSession::on_connectfrom_result(bool success)
{
	dealwith_connect_result(success);
}

void NetSession::dealwith_connect_result(bool success)
{
	std::list<NetProtocol*> recvs;
	{
		ThreadLockWrapper guard(*lock_);

		if (success)
		{
			is_initialized = true;

			boost::asio::socket_base::keep_alive option(true);
			boost::system::error_code err;
			socket_->set_option(option, err);

			try_read_nomutext(0, recvs);
		}
		else
		{
			//only use to do reset
			is_initialized = true;

			this->reset_nomutex();
			return;
		}
	}

	if (success)
	{
		std::list<NetProtocol*>::iterator iter = recvs.begin(), eiter = recvs.end();
		for (; iter != eiter; ++iter)
		{
			NetProtocol* pr = (*iter);
			if (bind_event_cb_)
				bind_event_cb_->on_recv_protocol_netthread( pr);
			else
				delete pr;
		}
		recvs.clear();
	}
}

void NetSession::on_connect_lost_netthread()
{
	{
		//first release resources
		ThreadLockWrapper guard( *lock_);

		if( is_initialized)
			this->reset_nomutex();
	}

	if( bind_event_cb_)
		bind_event_cb_->on_connect_lost_netthread();
}

bool NetSession::try_read_nomutext( int alreadyrecvsize, std::list<NetProtocol*>& readpro)
{
	if( !is_initialized)
		return true;

	//move tail point to free space begin position
	recv_buff_pos_ += alreadyrecvsize;

	int len =analy_package_nomutex( readpro);
	if( len < 0)
		return false;

	if( len > 0)
	{
		S_UINT_8 *pdata =recv_buff_ + len;
		int movelen =recv_buff_pos_ - len;
		if( movelen > 0)
			memmove( recv_buff_, pdata, movelen);

		recv_buff_pos_ = movelen;
	}

	int freespace = NET_RECVBUFF_SIZE_MAX - recv_buff_pos_;
	if( freespace > NET_SINGLE_PACKAGE_SIZE_MAX)
		freespace =NET_SINGLE_PACKAGE_SIZE_MAX;

	S_UINT_8* pbuf =recv_buff_ + recv_buff_pos_;
	
	socket_->async_read_some(
		boost::asio::buffer( pbuf, freespace),
		boost::bind( &NetSession::handle_read, this, boost::placeholders::_1, boost::placeholders::_2));

	return true;
}

int NetSession::analy_package_nomutex( std::list<NetProtocol*>& readpro)
{
	int reduce_len =0;

	if( recv_buff_pos_ < sizeof(S_UINT_32))
		return reduce_len;

	int len =recv_buff_pos_;
	do
	{
		S_UINT_8 *pdata =recv_buff_ + reduce_len;

		if( len < sizeof(S_UINT_32))
			break;
		
		S_UINT_32 len2 = *((S_UINT_32*)pdata);
		{
			S_UINT_32 offset = 0;
			CProtoHeadBase::Decode(pdata, (S_UINT_32)len, offset, len2);
		}
		
		if( len2 > NET_SINGLE_PACKAGE_SIZE_MAX)
			return -1;

		len -= len2;
		if( len < 0)
			break;

		NetProtocol* protocol = bind_event_cb_->unpack_protocol(pdata, len2);

		if( protocol == 0)
		{
			std::list<NetProtocol*>::iterator iter =readpro.begin(), eiter =readpro.end();
			for( ; iter != eiter; ++iter)
				delete *iter;
			readpro.clear();

			return -1;
		}
		else
		{
			readpro.push_back( protocol);
		}

		reduce_len += (int)len2;
	}
	while( len > 0);

	return reduce_len;
}

void NetSession::try_write_nomutext( int alreadysendsize)
{
	if( is_sending_)
		return;

	fill_writebuffer_nomutex( alreadysendsize);

	if( send_buff_pos_ > 0)
	{
		is_sending_ =true;

		socket_->async_write_some(
			boost::asio::buffer( send_buff_, send_buff_pos_),
			boost::bind( &NetSession::handle_write, this, boost::placeholders::_1, boost::placeholders::_2));
	}
}

void NetSession::fill_writebuffer_nomutex( int alreadysendsize)
{
	if( alreadysendsize > 0)
	{
		if( alreadysendsize > send_buff_pos_)
			alreadysendsize =send_buff_pos_;

		int needmove = send_buff_pos_ - alreadysendsize;

		if( needmove > 0)
		{
			S_UINT_8 *pbuf =(send_buff_+alreadysendsize);
			memmove( send_buff_, pbuf, needmove);
		}

		send_buff_pos_ =needmove;
	}

	//try to fill other package
	int freespace = NET_SENDBUFF_SIZE_MAX - send_buff_pos_;
	if( freespace < NET_SINGLE_PACKAGE_SIZE_MAX)
		freespace =0;
	else
		freespace = NET_SINGLE_PACKAGE_SIZE_MAX;

	int len =0;
	while( (len =( freespace)) > 0 && send_queue_.size() > 0)
	{
		S_UINT_8 *pdata =(send_buff_ + send_buff_pos_);

		NetProtocol* msg =0;

		{
			ThreadLockWrapper guard( *queue_lock_);

			msg =send_queue_.front();
			send_queue_.pop_front();
		}

		if (msg == 0) continue;
		std::unique_ptr<NetProtocol> p_msg( msg);

		S_UINT_32 len2 = 0;
		if (!bind_event_cb_->pack_protocol(pdata, (S_UINT_32)len, msg, len2))
		{
			ThreadLockWrapper guard(*queue_lock_);
			send_queue_.push_front(p_msg.release());
			break;
		}

		if (len2 > NET_SINGLE_PACKAGE_SIZE_MAX) {
			logError(out_runtime, "NetSession::fill_writebuffer_nomutex msg->SerializeToArray( pbuf, len) failed, maxsize=%d prosize=%d", len, len2);
		}

		send_buff_pos_ += (S_INT_32)len2;

		freespace -= (S_INT_32)len2;
	}
}

void NetSession::handle_read( boost::system::error_code error, size_t bytes_transferred)
{
	if( error)
		on_connect_lost_netthread();
	else
	{
		std::list<NetProtocol*> recvs;
		bool read_err =false;
		{
			ThreadLockWrapper guard( *lock_);

			if( !is_initialized)
				return;

			read_err = (!try_read_nomutext( bytes_transferred, recvs));
		}

		if( read_err)
			on_connect_lost_netthread();
		else
		{
			std::list<NetProtocol*>::iterator iter =recvs.begin(), eiter =recvs.end();
			for( ; iter != eiter; ++iter)
			{
				NetProtocol* pr =(*iter);
				if( bind_event_cb_)
					bind_event_cb_->on_recv_protocol_netthread( pr);
				else
					delete pr;
			}
			recvs.clear();
		}
	}
}

void NetSession::handle_write( boost::system::error_code error, size_t bytes_transferred)
{
	if( error)
		on_connect_lost_netthread();
	else
	{
		ThreadLockWrapper guard( *lock_);

		is_sending_ =false;

		if( !is_initialized)
			return;

		try_write_nomutext( bytes_transferred);
	}
}

void NetSession::send_protocol(NetProtocol* message)
{
	std::unique_ptr<NetProtocol> pMsg( message);

	if( !is_initialized)
		return;

	//save to send queue
	{
		ThreadLockWrapper guard( *queue_lock_);

		send_queue_.push_back( pMsg.release());
	}

	ThreadLockWrapper guard( *lock_);

	if( !is_initialized)
		return;

	try_write_nomutext( 0);
}

void NetSession::heart_beat()
{
	if( !is_initialized)
		return;

	if( send_queue_.size() > 0)
		return;

	if (bind_event_cb_ != 0)
	{
		NetProtocol *msg = bind_event_cb_->get_livekeep_msg();
		if (msg != 0)
		{
			send_protocol(msg);
		}
	}
}
