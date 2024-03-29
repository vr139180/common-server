﻿// Copyright 2021 common-server Authors
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
