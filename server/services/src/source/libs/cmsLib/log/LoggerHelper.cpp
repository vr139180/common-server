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

#include "cmsLib/log/LoggerHelper.h"

#include <stdio.h>

#include <stdarg.h>
#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>

namespace logging = boost::log;
namespace sinks = logging::sinks;
namespace expr = logging::expressions;
namespace attrs = boost::log::attributes;

//model分类功能日志
BOOST_LOG_ATTRIBUTE_KEYWORD( model_attr, "Model", std::string)

#define LOGMSG_LENGTH	512

LoggerHelper::LoggerHelper()
{
}

LoggerHelper::~LoggerHelper()
{
}

void LoggerHelper::InitLogger( const char* filename)
{
	regist_custom_sink();
	
	std::ifstream file( filename, std::ifstream::in);
	logging::init_from_stream(file);

	logging::add_common_attributes();
}

void LoggerHelper::Shutdown()
{
	logging::core::get()->flush();
}

void LoggerHelper::Debug( const char* model, const char* msg, ...)
{
	//BOOST_LOG_NAMED_SCOPE("xxx");
	BOOST_LOG_SCOPED_THREAD_TAG("Model", model);

	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(debug) << buf;
}

void LoggerHelper::Info( const char* model, const char* msg,...)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Model", model);

	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(info) << buf;
}

void LoggerHelper::Warn( const char* model, const char* msg,...)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Model", model);

	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(warning) << buf;
}

void LoggerHelper::Error( const char* model, const char* msg,...)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Model", model);

	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(error) << buf;
}

void LoggerHelper::Fatal( const char* model, const char* msg,...)
{
	BOOST_LOG_SCOPED_THREAD_TAG("Model", model);

	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	BOOST_LOG_TRIVIAL(fatal) << buf;
}
