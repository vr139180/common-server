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

#include "gameLib/config/FileConfigSourceImpl.h"

#include <cmsLib/util/FileUtil.h>
#include <cmsLib/Log.h>

FileConfigSourceImpl::FileConfigSourceImpl(void)
{
}

FileConfigSourceImpl::~FileConfigSourceImpl(void)
{
}

bool FileConfigSourceImpl::init_source( std::map< std::string, std::string> prop)
{
	//设置配置文件根目录
	basedir_ =prop["dirbase"];
	if( basedir_.size() == 0)
		return false;

	char ec =basedir_[basedir_.size()-1];
	if( ec != '/' && ec != '\\' )
		basedir_ += "/";

	if( !FileUtil::is_direxist( basedir_.c_str()))
		return false;

	return true;
}

void FileConfigSourceImpl::uninit_source()
{

}

std::string FileConfigSourceImpl::get_txtfilecontent( const char* filename, bool deployfile)
{
	std::string p =basedir_;

	p += filename;

	return FileUtil::get_filecontent( p.c_str());
}

void FileConfigSourceImpl::get_filelist( const char* path, const char* ext, std::list< std::string>& ret)
{
	std::string p =basedir_;
	p += path;

	if( !FileUtil::is_direxist( p.c_str()))
	{
		logError(out_runtime, "direction:%s don't exist", path);
		return;
	}

	ret.clear();

	FileUtil::get_filelist( p.c_str(), ext, ret);
}

void FileConfigSourceImpl::get_subdirlist( const char* path, std::list< std::string>& ret)
{

}
