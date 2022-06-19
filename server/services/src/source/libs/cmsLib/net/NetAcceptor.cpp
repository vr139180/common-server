#include "cmsLib/net/NetAcceptor.h"
#include "cmsLib/net/NetDriverX.h"
#include "cmsLib/Log.h"

NetAcceptor::NetAcceptor( NetAcceptorEvent& cb):
event_cb_( cb),
acceptor_( 0),
max_sessions_( 0),
port_( 0),
is_initialied_( false)
{
}

NetAcceptor::~NetAcceptor()
{
	if( acceptor_ != 0)
		delete acceptor_;
	acceptor_ = 0;
}

bool NetAcceptor::begin_listen( const char* ip, int port, int maxnums)
{
	if( acceptor_ != 0)
		return false;

	try{
		acceptor_ =new boost::asio::ip::tcp::acceptor( NetDriverX::getInstance().get_ioservice());

		boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port);
		endpoint.address( boost::asio::ip::address_v4::from_string( ip));

		acceptor_->open( endpoint.protocol());

        boost::asio::socket_base::reuse_address reuse_addr_option(true);
        acceptor_->set_option(reuse_addr_option);

		acceptor_->bind( endpoint);

		if( maxnums <=0)
			maxnums =1;

		acceptor_->listen( maxnums);

		boost::asio::ip::tcp::endpoint le = acceptor_->local_endpoint();
		this->port_ = (int)le.port();

		max_sessions_ =maxnums;

		is_initialied_ =true;
	}
	catch(...){

		if( acceptor_ != 0)
		{
			delete acceptor_;
			acceptor_ =0;
		}

		return false;
	}

	try_accept();

	return true;
}

void NetAcceptor::end_listen()
{
	try{
		if( acceptor_ != 0)
		{
			boost::system::error_code err;
			acceptor_->cancel(err);
			acceptor_->close( err);
		}
	}
	catch(...){
	}

	is_initialied_ =false;
}

void NetAcceptor::try_accept()
{
	if( acceptor_ == 0 || !is_initialied_)
		return;

	NetSessionPtr empty_session =event_cb_.ask_free_netsession();

	if( empty_session.get() == 0)
	{
		refuse_socket_.reset( new boost::asio::ip::tcp::socket( NetDriverX::getInstance().get_ioservice()));
		acceptor_->async_accept( *refuse_socket_,
			boost::bind( &NetAcceptor::handle_refuse, this, boost::asio::placeholders::error) );
	}
	else
	{
		empty_session->attachToSocket( NetDriverX::getInstance().get_ioservice());

		acceptor_->async_accept( empty_session->get_socket(),
			boost::bind( &NetAcceptor::handle_accept, this, empty_session, boost::asio::placeholders::error));
	}
}

void NetAcceptor::handle_refuse( const boost::system::error_code& error)
{
	logError(out_net, "net acceptor, refuse client connect err:%d....", error.value());

	boost::system::error_code err;
	refuse_socket_->close( err);

	try_accept();
}

void NetAcceptor::handle_accept( NetSessionPtr session, const boost::system::error_code& error)
{
	bool bsucc =false;
	if( error.value() == 0)
		bsucc =true;
	if( !session->is_socket_valid())
		bsucc =false;

	event_cb_.accept_netsession( session, !bsucc, error.value());
	session->on_connectfrom_result(bsucc);

	//try to accept new connect request
	try_accept();
}
