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

#ifndef __ICONFIGCONTENTSOURCE__H__
#define __ICONFIGCONTENTSOURCE__H__

#include <string>
#include <list>
#include <map>

class IConfigContentSource
{
public:
	/**
	* 初始化数据源
	* @param prop
	**/
	virtual bool init_source( std::map< std::string, std::string> prop) =0;
	virtual void uninit_source() =0;
	
	/**
	* 根据指定的路径和文件名获取文本文件内容
	*
	* @param filename
	* @return
	**/
	virtual std::string get_txtfilecontent( const char* filename, bool deployfile =false) =0;

	/**
	* 从指定的路径和指定的扩展名获取文件名称列表
	* @param path
	* @param ext
	* @param ret
	**/
	virtual void get_filelist( const char* path, const char* ext, std::list< std::string>& ret) =0;

	/**
	* 从指定的路径获取子目录名称列表
	* @param path
	* @param ret
	**/
	virtual void get_subdirlist( const char* path, std::list< std::string>& ret) =0;

};

#endif	//__ICONFIGCONTENTSOURCE__H__
