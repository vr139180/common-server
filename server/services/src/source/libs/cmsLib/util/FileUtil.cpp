#include "cmsLib/util/FileUtil.h"

#include <cmsLib/Log.h>

#include <boost/filesystem.hpp>
#include <fstream>

std::string FileUtil::get_filecontent( const char* path, const char* file)
{
	if( !boost::filesystem::is_directory( path))
		return "";

	boost::filesystem::path p( path);
	p /= file;

	if( !boost::filesystem::exists( p))
		return "";

	std::string ret ="";

	std::ifstream ff;
	ff.open( p.string().c_str());

	char buf[1024*100];
	while( !ff.eof())
	{
		ff.read( &buf[0], 1024*100 -1);
		std::streamsize cnt =ff.gcount();
		buf[cnt] ='\0';

		ret += buf;
	}

	return ret;
}

std::string FileUtil::get_filecontent( const char* file)
{
	boost::filesystem::path p( file);

	if( !boost::filesystem::exists( p))
	{
		logError( out_boot, "file:%s don't exist", file);
		return "";
	}

	std::string ret ="";

	std::ifstream ff;
	ff.open( p.string().c_str());

	char buf[1024*100];
	while( !ff.eof())
	{
		ff.read( &buf[0], 1024*100 -1);
		std::streamsize cnt =ff.gcount();
		buf[cnt] ='\0';

		ret += buf;
	}

	return ret;
}

bool FileUtil::is_direxist( const char* path)
{
	if( path == 0)
		return false;

	if( !boost::filesystem::is_directory( path))
		return false;

	return true;
}

void FileUtil::get_filelist( const char* path, const char* ext, std::list< std::string>& ret)
{
	if( !is_direxist( path))
		return;

	std::string fext ="";
	if( ext != 0)
		fext =ext;

	boost::filesystem::directory_iterator eiter;
	for( boost::filesystem::directory_iterator iter( path); iter != eiter; ++iter)
	{
		if( boost::filesystem::is_directory( *iter))
			continue;

		std::string fname =iter->path().filename().string();
		std::string sext =iter->path().extension().string();
		
		if( fext != "" && fext != sext)
			continue;

		ret.push_back( fname);
	}
}