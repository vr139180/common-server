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

#include <cmsLib/util/FileLogOnlyOnce.h>

#include <cmsLib/base/OSSystem.h>

FileLogOnlyOnce::FileLogOnlyOnce( const char* filename)
{
	boost::filesystem::path p(OSSystem::work_path2_.c_str());
	p /= filename;

	log_.open( p.string().c_str(), std::ofstream::out|std::ofstream::trunc);
}

FileLogOnlyOnce::~FileLogOnlyOnce()
{
	log_.close();
}

void FileLogOnlyOnce::write_line( const char* format, ...)
{
	if( !log_.is_open())
		return;

	char buf[512] ={0};

	va_list vaList;

	va_start( vaList, format);
	::vsnprintf( buf, sizeof(buf)-1, format, vaList);
	va_end( vaList);

	log_ << buf << std::endl;
}

void FileLogOnlyOnce::write_noline( const char* format, ...)
{
	if( !log_.is_open())
		return;

	char buf[512] ={0};

	va_list vaList;

	va_start( vaList, format);
	::vsnprintf( buf, sizeof(buf)-1, format, vaList);
	va_end( vaList);

	log_ << buf;
}
