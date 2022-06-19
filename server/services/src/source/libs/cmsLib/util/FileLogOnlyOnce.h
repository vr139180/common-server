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