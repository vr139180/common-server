#ifndef __COMMANDBASE__H__
#define __COMMANDBASE__H__

#include "cmsLib/prolib/BasicProtocol.h"

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

typedef boost::function< void( NetProtocol*, bool&)>			NETCMD_FUN_MAP;
typedef boost::function< void( NetProtocol*, bool&, void*)>		NETCMD_FUN_MAP3;

class CommandBase
{
public:
	CommandBase( bool d):delete_pro_(d){}
	CommandBase():delete_pro_(true){}

	virtual ~CommandBase(){}

	virtual void run() =0;

public:
	bool	delete_pro_;
};

class NetCommand : public CommandBase
{
public:
	NetCommand(NetProtocol *d2, NETCMD_FUN_MAP fun, bool autorelease =true);
	virtual ~NetCommand();

	virtual void run();

private:
	NETCMD_FUN_MAP	fun_;
	NetProtocol*	pro_;
};

class NetCommandV : public CommandBase
{
public:
	NetCommandV(NetProtocol *d2, NETCMD_FUN_MAP3 fun, bool autorelease =true);
	virtual ~NetCommandV();

	virtual void run();

private:
	NETCMD_FUN_MAP3	fun_;
	NetProtocol*	pro_;
};

template< class T>
class SystemCommand : public CommandBase
{
public:
	SystemCommand( boost::function< void( T*)> f);
	virtual ~SystemCommand();

	virtual void run();

private:
	boost::function< void( T*)>	fun_;
};

template< class T>
SystemCommand<T>::SystemCommand( boost::function< void( T*)> f):fun_( f)
{
}

template< class T>
SystemCommand<T>::~SystemCommand()
{
}

template< class T>
void SystemCommand<T>::run()
{
	fun_( (T*)0);
}

template< class T>
class SystemCommand2 : public CommandBase
{
public:
	SystemCommand2( boost::function< void( T)> f, T v);
	virtual ~SystemCommand2();

	virtual void run();

private:
	boost::function< void( T)>	fun_;
	T	param_;
};

template< class T>
SystemCommand2<T>::SystemCommand2( boost::function< void( T)> f, T v):fun_( f), param_(v)
{
}

template< class T>
SystemCommand2<T>::~SystemCommand2()
{
}

template< class T>
void SystemCommand2<T>::run()
{
	fun_( param_);
}

//------------------------------------------------
template< class T>
class SystemCommand3 : public CommandBase
{
public:
	SystemCommand3(boost::function< void(T*)> f, T* v);
	virtual ~SystemCommand3();

	virtual void run();

private:
	boost::function< void(T*)>	fun_;
	T*	param_;
};

template< class T>
SystemCommand3<T>::SystemCommand3(boost::function< void(T*)> f, T* v) :fun_(f), param_(v)
{
}

template< class T>
SystemCommand3<T>::~SystemCommand3()
{
}

template< class T>
void SystemCommand3<T>::run()
{
	fun_(param_);
}

#endif //__COMMANDBASE__H__
