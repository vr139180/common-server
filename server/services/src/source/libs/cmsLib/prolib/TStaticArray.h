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

#ifndef __TSTATICARRAY_H__
#define __TSTATICARRAY_H__

#include <cmsLib/prolib/prolibdef.h>

template< class T, int TARRAY_SIZE_MAX =1>
class TStaticArray
{
protected:
	struct _internal_item
	{
		_internal_item():is_used_(false){}

		void set_used( bool bused){ is_used_ =bused;}

		T	 item_;
		bool is_used_;
	};

public:
	TStaticArray();

	T& operator []( int index);

	bool is_used( int index);
	void set_used( int index, bool bused);
	int	 nums(){ return item_nums_;}
	int	 max(){ return TARRAY_SIZE_MAX;}

	void reset();

private:
	_internal_item	items_[TARRAY_SIZE_MAX];
	int				item_nums_;
};

template< class T, int TARRAY_SIZE_MAX>
TStaticArray< T, TARRAY_SIZE_MAX>::TStaticArray():
item_nums_(0)
{
}

template< class T, int TARRAY_SIZE_MAX>
T& TStaticArray< T, TARRAY_SIZE_MAX>::operator[]( int index)
{
	PROLIB_ASSERT( index < 0 || index >= TARRAY_SIZE_MAX);

	return items_[index].item_;
}

template< class T, int TARRAY_SIZE_MAX>
bool TStaticArray< T, TARRAY_SIZE_MAX>::is_used( int index)
{
	PROLIB_ASSERT( index < 0 || index >= TARRAY_SIZE_MAX);

	return items_[index].is_used_;
}

template< class T, int TARRAY_SIZE_MAX>
void TStaticArray< T, TARRAY_SIZE_MAX>::set_used( int index, bool bused)
{
	PROLIB_ASSERT( index < 0 || index >= TARRAY_SIZE_MAX);

	if( items_[index].is_used_ != bused)
	{
		bused? ++item_nums_:--item_nums_;
	}

	items_[index].set_used( bused);
}

template< class T, int TARRAY_SIZE_MAX>
void TStaticArray< T, TARRAY_SIZE_MAX>::reset()
{
	for( int ii =0; ii < TARRAY_SIZE_MAX; ++ii)
		items_[ii].set_used( false);

	item_nums_ =0;
}

#endif	//__TSTATICARRAY_H__

