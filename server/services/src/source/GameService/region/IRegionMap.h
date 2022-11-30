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

#ifndef __IREGIONMAP_H__
#define __IREGIONMAP_H__

//regionmap功能接口定义
//后续可以提供多种实现
class IRegionMap
{
public:
	virtual ~IRegionMap();

	virtual void init_region() = 0;
};

#endif //__IREGIONMAP_H__