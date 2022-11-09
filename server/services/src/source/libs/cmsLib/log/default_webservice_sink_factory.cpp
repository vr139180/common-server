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

//#include <boost/log/detail/setup_config.hpp>

#ifndef __DEFAULT_WEBSERVICE_SINK_FACTORY_H__
#define __DEFAULT_WEBSERVICE_SINK_FACTORY_H__

#include <cstddef>
#include <ios>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <algorithm>
#include <boost/type.hpp>
#include <boost/limits.hpp>
#include <boost/cstdint.hpp>
#include <boost/bind/bind.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
//#include <boost/core/null_deleter.hpp>
#include <boost/optional/optional.hpp>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/type_traits/conditional.hpp>
//#include <boost/type_traits/is_unsigned.hpp>
//#include <boost/type_traits/integral_constant.hpp>
//#include <boost/spirit/home/qi/numeric/numeric_utils.hpp>
//#include <boost/log/detail/code_conversion.hpp>
#include <boost/log/detail/singleton.hpp>
#include <boost/log/detail/default_attribute_names.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions/filter.hpp>
#include <boost/log/expressions/formatter.hpp>
#include <boost/log/utility/string_literal.hpp>
#include <boost/log/utility/functional/nop.hpp>
#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

//#include <boost/log/sinks/frontend_requirements.hpp>
//#include <boost/date_time/date_defs.hpp>
//#include <boost/log/sinks/auto_newline_mode.hpp>

#if !defined(BOOST_LOG_NO_THREADS)
#include <boost/log/detail/locks.hpp>
#include <boost/log/detail/light_rw_mutex.hpp>
#endif

#include <boost/log/detail/header.hpp>

#include <cmsLib/log/WSSinkBackend.h>
#include <cmsLib/util/ShareUtil.h>

namespace logging = boost::log;
namespace sinks = logging::sinks;

namespace boost {

BOOST_LOG_OPEN_NAMESPACE

template< typename CharT >
class default_webservice_sink_factory : public logging::sink_factory< CharT >
{
public:
	typedef default_webservice_sink_factory< CharT > this_type;
	typedef logging::sink_factory< CharT > base_type;
	typedef typename base_type::char_type char_type;
	typedef typename base_type::string_type string_type;
	typedef typename base_type::settings_section settings_section;

public:
	//! The function constructs a sink that writes log records to a text file
	shared_ptr< sinks::sink > create_sink(settings_section const& params) BOOST_OVERRIDE
	{
		typedef WSSinkBackend backend_t;
		shared_ptr< backend_t > backend = boost::make_shared< backend_t >();

		if (optional<string_type> models = params["Models"])
		{
			std::list<std::string> mds;
			ShareUtil::splitstr2str(models.get().c_str(), ";", mds);
			for (std::list<std::string>::iterator iter = mds.begin(); iter != mds.end(); ++iter)
			{
				std::string sname = (*iter);
				std::string murl = ShareUtil::str_format("Model_%s_inf", sname.c_str());
				if (optional<string_type> uri = params[murl.c_str()])
				{
					std::string uu = uri.get();
					backend->add_model_url(sname, uu);
				}
			}
		}
		// Auto flush
		/*
		if (optional< string_type > auto_flush_param = params["AutoFlush"])
		{
			if (auto_flush_param.has_value())
			{
				if (auto_flush_param.get() == "true")
					backend->auto_flush(true);
				else
					backend->auto_flush(false);
			}
			else
				backend->auto_flush(false);
		}
		*/

		return this_type::init_sink(backend, params);
	}

protected:
	//! Sink backend character selection function
	template< typename InitializerT >
	static shared_ptr< sinks::sink > select_backend_character_type(settings_section const& params, InitializerT initializer)
	{
#if defined(BOOST_LOG_USE_CHAR) && defined(BOOST_LOG_USE_WCHAR_T)
		if (optional< string_type > wide_param = params["Wide"])
		{
			string_type v = wide_param->get();
			if( v == "true")
				return initializer(params, type< wchar_t >());
		}

		return initializer(params, type< char >());
#elif defined(BOOST_LOG_USE_CHAR)
		return initializer(params, type< char >());
#elif defined(BOOST_LOG_USE_WCHAR_T)
		return initializer(params, type< wchar_t >());
#endif
	}

	//! The function initializes common parameters of a formatting sink and returns the constructed sink
	template< typename BackendT >
	static shared_ptr< sinks::sink > init_sink(shared_ptr< BackendT > const& backend, settings_section const& params)
	{
		typedef BackendT backend_t;

		// Filter
		filter filt;
		if (optional< string_type > filter_param = params["Filter"])
		{
			filt = parse_filter(filter_param.get());
		}

		shared_ptr< sinks::basic_sink_frontend > p;

#if !defined(BOOST_LOG_NO_THREADS)
		// Asynchronous. TODO: make it more flexible.
		bool async = false;
		if (optional< string_type > async_param = params["Asynchronous"])
		{
			if( async_param.get() == "true")
				async = true;
		}

		// Construct the frontend, considering Asynchronous parameter
		if (!async)
		{
			p = boost::make_shared< sinks::synchronous_sink< backend_t > >(backend);
		}
		else
		{
			p = boost::make_shared< sinks::asynchronous_sink< backend_t > >(backend);
			p->set_exception_handler(nop());
		}
#else
		// When multithreading is disabled we always use the unlocked sink frontend
		p = boost::make_shared< sinks::unlocked_sink< backend_t > >(backend)
#endif

		p->set_filter(filt);

		return p;
	}
};

BOOST_LOG_CLOSE_NAMESPACE // namespace log

} // namespace boost

#include <cmsLib/log/LoggerHelper.h>

void LoggerHelper::regist_custom_sink()
{
	logging::register_sink_factory("WebService", boost::make_shared<logging::default_webservice_sink_factory<char> >());
}

#endif //__DEFAULT_WEBSERVICE_SINK_FACTORY_H__
