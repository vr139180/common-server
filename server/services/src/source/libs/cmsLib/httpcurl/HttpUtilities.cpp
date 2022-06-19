#include <cmsLib/httpcurl/HttpUtilities.h>

const std::string HttpUtilities::TruncatePrefix( std::string& str, unsigned int pos, unsigned int skip)
{
	std::string res;
	if (str.length() < pos + skip)
		return res;
	res = str.substr(0,pos + skip);
	str = str.substr(pos + skip);
	return res;
}

const std::string HttpUtilities::TruncateSuffix( std::string& str, unsigned int pos, unsigned int skip)
{
	std::string res;
	if (str.length() < pos + skip)
		return res;
	res = str.substr(pos + skip);
	str = str.substr(0,pos + skip);
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

