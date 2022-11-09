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

#ifndef __OSSYSTEM_H__
#define __OSSYSTEM_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/OSDriver.h>

class OSSystem
{
private:
	OSSystem(){}

public:
	static bool	init();
	static void	uninit();

public:
	static OSDriver*        mOS;
	static std::string      mOSType;

	static std::string      work_path2_;
	static std::string		script_path_;
};

#endif // __OSSYSTEM_H__
