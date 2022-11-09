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

#ifndef __FILELOGONLYONCE_H__
#define __FILELOGONLYONCE_H__

#include <boost/filesystem.hpp>
#include <fstream>

class FileLogOnlyOnce
{
public:
	FileLogOnlyOnce( const char* filename);
	~FileLogOnlyOnce();

	void write_line( const char* format, ...);
	void write_noline( const char* format, ...);

private:
	std::ofstream	log_;
};

#endif	//__FILELOGONLYONCE_H__