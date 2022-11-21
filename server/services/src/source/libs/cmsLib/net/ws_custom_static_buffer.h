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

#ifndef __WS_CUSTOM_STATIC_BUFFER_H__
#define __WS_CUSTOM_STATIC_BUFFER_H__

#include <boost/beast/core.hpp>
using namespace boost::beast;

template<std::size_t N>
class ws_custom_static_buffer : public flat_static_buffer_base
{
	char *buf_;

public:
	
	/// Constructor
	ws_custom_static_buffer( char* pbuf)
		: flat_static_buffer_base( pbuf, N), buf_( pbuf)
	{
	}

	/// Returns the @ref flat_static_buffer_base portion of this object
	flat_static_buffer_base&
		base()
	{
		return *this;
	}

	/// Returns the @ref flat_static_buffer_base portion of this object
	flat_static_buffer_base const&
		base() const
	{
		return *this;
	}

	/// Return the maximum sum of the input and output sequence sizes.
	std::size_t constexpr
		max_size() const
	{
		return N;
	}

	/// Return the maximum sum of input and output sizes that can be held without an allocation.
	std::size_t constexpr
		capacity() const
	{
		return N;
	}
};

#endif //__WS_CUSTOM_STATIC_BUFFER_H__
