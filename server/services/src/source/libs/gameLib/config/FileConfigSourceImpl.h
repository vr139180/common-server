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

#ifndef __FILECONFIGSOURCEIMPL__H__
#define __FILECONFIGSOURCEIMPL__H__

#include <gameLib/config/IConfigContentSource.h>

/**
* @class FileConfigSourceImpl
* 
* @brief 配置模块数据源本地文件实现
**/
class FileConfigSourceImpl : public IConfigContentSource
{
public:
	FileConfigSourceImpl();
	~FileConfigSourceImpl();

public:
	//-----------------------implement IConfigContentSource------------------------
	/**
	* @see void IConfigContentSource::init_source( std::map< std::string, std::string> prop)
	* FileConfigSourceImpl支持的属性
	* dirbase:	配置文件根目录
	**/
	virtual bool init_source( std::map< std::string, std::string> prop);

	/**
	* @see void IConfigContentSource::uninit_source()
	**/
	virtual void uninit_source();

	/**
	* @see NS_STL::string IConfigContentSource::get_txtfilecontent( const char* path, const char* filename)
	**/
	virtual std::string get_txtfilecontent( const char* filename, bool deployfile =false);

	/**
	* @see void IConfigContentSource::get_filelist( const char* path, const char* ext, std::list< std::string>& ret)
	**/
	virtual void get_filelist( const char* path, const char* ext, std::list< std::string>& ret);

	/**
	* @see void IConfigContentSource::get_subdirlist( const char* path, std::list< std::string>& ret)
	**/
	virtual void get_subdirlist( const char* path, std::list< std::string>& ret);

protected:


private:
	//配置信息根目录
	std::string basedir_;
};

#endif	//__FILECONFIGSOURCEIMPL__H__
