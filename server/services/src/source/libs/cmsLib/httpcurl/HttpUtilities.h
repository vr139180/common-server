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