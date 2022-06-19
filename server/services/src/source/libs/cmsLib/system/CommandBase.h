#ifndef __COMMANDBASE__H__
#define __COMMANDBASE__H__

//#include <cmsLib/prolib/core_type.h>
#include "cmsLib/prolib/BasicProtocol.h"

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

typedef boost::function< void( BasicProtocol*, bool&)>			NETCMD_FUN_MAP;
typedef boost::function< void( BasicProtocol*, bool&, int)>		NETCMD_FUN_MAP2;
typedef boost::function< void( BasicProtocol*, bool&, void*)>	NETCMD_FUN_MAP3;

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
	NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP fun, bool autorelease =true);
	NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP2 fun, int v, bool autorelease =true);
	virtual ~NetCommand();

	virtual void run();

private:
	NETCMD_FUN_MAP	fun_;
	NETCMD_FUN_MAP2	fun2_;
	int fun2_v;
	BasicProtocol*	pro_;
};

class NetCommandV : public CommandBase
{
public:
	NetCommandV( BasicProtocol *d2, NETCMD_FUN_MAP3 fun, bool autorelease =true);
	virtual ~NetCommandV();

	virtual void run();

private:
	NETCMD_FUN_MAP3	fun_;
	BasicProtocol*	pro_;
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
