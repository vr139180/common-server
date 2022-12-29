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
#ifndef __HTTP_UTILITIES_H__
#define __HTTP_UTILITIES_H__

#include <string>

class HttpUtilities
{
public:
	struct MemoryStruct
	{
		char *memory;
		size_t size;
	};

	/** \internal */
	static const std::string TruncatePrefix(std::string& str, unsigned int pos, unsigned int skip = 0);

	/** \internal */
	static const std::string TruncateSuffix(std::string& str, unsigned int pos, unsigned int skip = 0);

	/** \internal */
	static const std::string GetNextToken(std::string& str, const std::string& delim);
};

#endif //__HTTP_UTILITIES_H__