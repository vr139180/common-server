#include "cmsLib/net/LinkToBase.h"
#include "cmsLib/net/NetDriverX.h"

LinkToBase::LinkToBase():
is_connected_( false),
is_authed_( false)
{
	session_.reset( new NetSession( 0, 0, this));
	connector_.set_netsession( session_.get());
}

LinkToBase::~LinkToBase()
{
}

bool LinkToBase::is_connected()
{
	return is_connected_;
}

bool LinkToBase::is_connecting()
{
	return connector_.is_connecting();
}

void LinkToBase::connect_to( const char* ip, int port)
{
	if( is_connected() || is_connecting())
		return;

	is_authed_ =false;
	is_connected_ =false;

	connector_.connect_to( ip, port);
}

void LinkToBase::force_close()
{
	session_->force_reset();
	connector_.force_close();

	is_connected_ =false;
	is_authed_ =false;
}

void LinkToBase::on_cant_connectedto()
{
	is_connected_ = false;
}

void LinkToBase::on_connectedto_done()
{
	is_connected_ = true;
}