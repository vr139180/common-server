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

#ifndef __PROTOUTIL_H__
#define __PROTOUTIL_H__

#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/commons/GLoc3D.h>

class ProtoUtil
{
private:
	ProtoUtil() {}

public:
	static bool get_location_from_msg(BasicProtocol* msg, GLoc3D& loc);
	static bool set_location_to_msg(BasicProtocol* msg, const GLoc3D& loc);
};

#endif //__PROTOUTIL_H__