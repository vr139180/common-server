#ifndef __FILEUTIL__H__
#define __FILEUTIL__H__

#include <string>
#include <list>

/**
* @class FileUtil
* 
* @brief 文件处理工具
**/
class FileUtil
{
private:
	FileUtil(){}

public:
	/**
	* 获取文件内容
	* @param path
	* @param file
	* @return
	**/
	static std::string get_filecontent( const char* path, const char* file);
	static std::string get_filecontent( const char* file);

	static void get_filelist( const char* path, const char* ext, std::list< std::string>& ret);

	/**
	* 检查目录是否存在
	**/
	static bool is_direxist( const char* path);
};

#endif	//__FILEUTIL__H__
