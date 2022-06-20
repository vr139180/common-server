#include "cmsLib/log/LoggerHelper.h"

#include <stdio.h>

#include <stdarg.h>
#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace sinks = logging::sinks;
namespace expr = logging::expressions;

#define LOGMSG_LENGTH	512

LoggerHelper::LoggerHelper()
{
}

LoggerHelper::~LoggerHelper()
{
}

void LoggerHelper::InitLogger( const char* filename)
{
	std::ifstream file( filename, std::ifstream::in);
	logging::init_from_stream(file);

	logging::add_common_attributes();
	
}

void LoggerHelper::Shutdown()
{
}

void LoggerHelper::Debug( const char* model, const char* msg, ...)
{
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(debug) << buf;
}

void LoggerHelper::Info( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(info) << buf;
}

void LoggerHelper::Warn( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(warning) << buf;
}

void LoggerHelper::Error( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(error) << buf;
}

void LoggerHelper::Fatal( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(fatal) << buf;
}
