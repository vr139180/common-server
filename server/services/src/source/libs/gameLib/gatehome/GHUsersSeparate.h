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

#ifndef __GHUSERSSEPARATE_H__
#define __GHUSERSSEPARATE_H__

#include <vector>
#include <boost/scoped_array.hpp>

template<typename T>
class GHUsersSeparate
{
public:
	GHUsersSeparate();
	virtual ~GHUsersSeparate();

	//init separate
	void init_separate(int maxnum, int piecenum);
	void uninit();
	//获取指定的piece的data
	//pieceind [0,max_piece_)
	void get_piece_data(int pieceind, std::vector<T*>& datas);
	//slot [0,total_num_0
	T* get_data_of_slot(int slot);
	//result: -1 error, [0,max_piece_)
	int get_pieceindex_from_slot(int slot);
	int get_max_piece() { return max_piece_; }
	//one peice of the max slot members 
	int get_piece_slotnum() { return piece_num_; }

private:
	int get_piece_nums();

protected:
	int total_num_;
	int piece_num_;
	int max_piece_;

	boost::scoped_array<T>	user_datas_;
};

template<typename T>
GHUsersSeparate<T>::GHUsersSeparate():total_num_(0), piece_num_(0)
{
}

template<typename T>
GHUsersSeparate<T>::~GHUsersSeparate()
{
}

template<typename T>
void GHUsersSeparate<T>::uninit()
{
	user_datas_.reset(0);
}

template<typename T>
void GHUsersSeparate<T>::init_separate(int maxnum, int piecenum)
{
	this->total_num_ = maxnum;
	this->piece_num_ = piecenum;
	this->max_piece_ = get_piece_nums();

	user_datas_.reset(new T[total_num_]);
	for (int ind = 0; ind < total_num_; ++ind)
	{
		user_datas_[ind].set_userslot(ind);
	}
}

template<typename T>
int GHUsersSeparate<T>::get_piece_nums()
{
	return total_num_%piece_num_ ? (total_num_ / piece_num_ + 1) : (total_num_ / piece_num_);
}

template<typename T>
void GHUsersSeparate<T>::get_piece_data(int pieceind, std::vector<T*>& datas)
{
	assert(pieceind >= 0 || pieceind < max_piece_);

	int bind = pieceind * piece_num_;
	for (; bind < total_num_ && bind < ((pieceind+1)*piece_num_); ++bind)
	{
		datas.push_back( &user_datas_[bind]);
	}
}

template<typename T>
int GHUsersSeparate<T>::get_pieceindex_from_slot(int slot)
{
	if (slot < 0 || slot >= total_num_)
		return -1;

	return slot/piece_num_;
}

template<typename T>
T* GHUsersSeparate<T>::get_data_of_slot(int slot)
{
	if (slot < 0 || slot >= total_num_)
		return 0;

	return &user_datas_[slot];
}

#endif //__GHUSERSSEPARATE_H__
