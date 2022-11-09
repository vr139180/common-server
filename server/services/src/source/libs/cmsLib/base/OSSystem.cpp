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

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/Log.h>

#if defined(EW_PLATFORM_WINDOWS)
    #include <cmsLib/os/win32/OSDriverWin32.h>
#elif defined(EW_PLATFORM_LINUX)
    #include <cmsLib/os/linux/OSDriverLinux.h>
#else
    #error Error: unsupport platform
#endif

OSDriver*		OSSystem::mOS =0;
std::string		OSSystem::work_path2_ ="";
std::string		OSSystem::script_path_ ="";
std::string		OSSystem::mOSType ="";

void GetPath( std::string& str)
{
	int pos = (int)str.length();
	while( ( pos > 0 ) && ( str[ pos - 1 ] != '/' ) && ( str[ pos - 1 ] != '\\' ) ) {
		pos--;
	}
	str = str.substr( 0, pos - 1  );
}

bool OSSystem::init()
{
	if( mOS)
	{
		logError(out_runtime, "%s : Init failed\n", EW_FUNC );
		return false;
	}

#if defined(EW_PLATFORM_WINDOWS)
	mOS = new OSDriverWin32();
	mOSType = "Windows";
#elif defined(EW_PLATFORM_LINUX)
	mOS = new OSDriverLinux();
	mOSType = "Linux";
#else
	return false;
#endif

	if( !mOS)
	{
		logError(out_runtime, "%s : Alloc OSDriver failed\n", EW_FUNC );
		return false;
	}

	mOS->UpdateNowTick();
	srand( (unsigned int)mOS->GetTicks());

	//path construct

	work_path2_ = mOS->ProcessName();
    GetPath(work_path2_);
	if ( ( work_path2_[work_path2_.size() - 1] == '/' 
		|| work_path2_[work_path2_.size() - 1] == '\\' ))
	{
		work_path2_[work_path2_.size() - 1] = '\0';
	}

    script_path_ = work_path2_;
    GetPath(script_path_);

	script_path_ += "\\datas\\script";

	return true;
}

void OSSystem::uninit()
{
	if( mOS)
	{
		delete mOS;
		mOS = NULL;
	}
}
