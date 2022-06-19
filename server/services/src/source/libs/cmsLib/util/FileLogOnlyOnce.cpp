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
