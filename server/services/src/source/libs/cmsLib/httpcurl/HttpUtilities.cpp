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
#include <cmsLib/httpcurl/HttpUtilities.h>

const std::string HttpUtilities::TruncatePrefix( std::string& str, unsigned int pos, unsigned int skip)
{
	std::string res;
	if (str.length() < pos + skip)
		return res;
	res = str.substr(0, pos);
	str = str.substr(pos + skip);
	return res;
}

const std::string HttpUtilities::TruncateSuffix( std::string& str, unsigned int pos, unsigned int skip)
{
	std::string res;
	if (str.length() < pos + skip)
		return res;
	res = str.substr(pos + skip);
	str = str.substr(0,pos);
	return res;
}

const std::string HttpUtilities::GetNextToken( std::string& str, const std::string& delim)
{
	std::string result;
	size_t delimPos = str.find(delim.c_str());

	if (delimPos != std::string::npos)
	{
		result = TruncatePrefix(str, delimPos, delim.length());
	}
	else
	{
		result = str;
		str = "";
	}

	return result;
}

