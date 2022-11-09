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

#ifndef __SINGLETONT__H__
#define __SINGLETONT__H__

/**
* @class SingletonT
* 
* @brief 单例类
**/
template< typename T>
class SingletonT
{
private:
	struct object_creator{
		object_creator(){ SingletonT<T>::instance();}

		inline void do_nothing()const{}
	};

	static object_creator creator_;

public:
	SingletonT();

	inline static T& instance();
};

template< typename T>
SingletonT<T>::SingletonT()
{
}

template< typename T>
T& SingletonT<T>::instance()
{
	static T ins;

	//确保在main入口之前执行
	creator_.do_nothing();

	return ins;
}

template <typename T> typename SingletonT<T>::object_creator SingletonT<T>::creator_;

#endif	//__SINGLETONT__H__
