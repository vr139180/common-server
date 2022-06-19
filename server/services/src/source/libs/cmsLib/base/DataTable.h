#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#include <cmsLib/prolib/core_type.h>

#include <boost/unordered_map.hpp>

// DataMapTable
template < class T ,int MAX_SIZE>
class DataMapTable
{
protected:
    T           datas_[MAX_SIZE];

	int			data_vec_keys_[MAX_SIZE];
	boost::unordered::unordered_map<int,int> data_keys_;

public:
    //Get Max Size
    int         get_max_size() { return MAX_SIZE; };
    const int   get_max_size() const { return MAX_SIZE; };

    void        reset();
    // Check Data by key
    T*          has_data_bykey( int key );
    const T*    has_data_bykey( int key ) const;
    // Get Data By Key, if don't have  will insert this key
    T*          get_data_bykey( int key );

    // Ease the Key of Data
    void        remove_data_bykey( int key );
    // Check Data by index, if keys is NO_INITVALUE, return null
    T*          has_data_byindex( int index );
    const T*    has_data_byindex( int index ) const;

public:
    DataMapTable();
    ~DataMapTable();
};

template<class T ,int MAX_SIZE>
DataMapTable<T, MAX_SIZE>::DataMapTable()
{
    reset();
}

template<class T ,int MAX_SIZE>
DataMapTable<T, MAX_SIZE>::~DataMapTable()
{
    reset();
}

template<class T ,int MAX_SIZE>
void DataMapTable<T, MAX_SIZE>::reset()
{
    for( int ii =0; ii < MAX_SIZE; ++ii)
	{
        datas_[ii].reset();
		data_vec_keys_[ii] =NO_INITVALUE;
	}

	boost::unordered::unordered_map<int,int> emp;
	data_keys_.swap( emp);
}

template<class T ,int MAX_SIZE>
T* DataMapTable<T, MAX_SIZE>::has_data_bykey( int key)
{
    if( key == NO_INITVALUE)
        return 0;

	boost::unordered::unordered_map<int,int>::iterator fiter =data_keys_.find( key);
	if( fiter == data_keys_.end())
		return 0;

	return &datas_[fiter->second];
}

template<class T ,int MAX_SIZE>
const T* DataMapTable<T, MAX_SIZE>::has_data_bykey( int key) const
{
	if( key == NO_INITVALUE)
		return 0;

	boost::unordered::unordered_map<int,int>::const_iterator fiter =data_keys_.find( key);
	if( fiter == data_keys_.end())
		return 0;

	return &datas_[fiter->second];
}

template<class T ,int MAX_SIZE>
T* DataMapTable<T, MAX_SIZE>::has_data_byindex( int index)
{
	if( index < 0 || index >= MAX_SIZE)
		return 0;

	if (data_vec_keys_[index] != NO_INITVALUE)
		return &(datas_[index]);

	return 0;
}

template<class T ,int MAX_SIZE>
const T* DataMapTable<T, MAX_SIZE>::has_data_byindex( int index) const
{
	if( index < 0 || index >= MAX_SIZE)
		return 0;

	if (data_vec_keys_[index] != NO_INITVALUE)
		return &(datas_[index]);

    return 0;
}

template<class T ,int MAX_SIZE>
T* DataMapTable<T, MAX_SIZE>::get_data_bykey( int key)
{
    if( key == NO_INITVALUE)
        return 0;

    //key alwardy exit
    int index = NO_INITVALUE;
    //empty key
    int eindex = NO_INITVALUE;

	boost::unordered::unordered_map<int,int>::iterator fiter =data_keys_.find( key);
	if( fiter != data_keys_.end())
		index =fiter->second;

	if( index == NO_INITVALUE)
	{
		//get one free slot
		for( int ii =0; ii < MAX_SIZE; ++ii)
		{
			if( data_vec_keys_[ii] == NO_INITVALUE)
			{
				eindex =ii;
				break;
			}
		}

		if( eindex != NO_INITVALUE)
		{
			index =eindex;
			data_keys_[key] =eindex;
			//same time save to vector in order to search quickly
			data_vec_keys_[index] =key;
		}
	}

    //k don't exit and there are not empty slot to use
    if( index == NO_INITVALUE)
        return 0;

    return &(datas_[index]);
}

template<class T ,int MAX_SIZE>
void DataMapTable<T, MAX_SIZE>::remove_data_bykey( int key)
{
    if( key == NO_INITVALUE)
        return;

	boost::unordered::unordered_map<int,int>::iterator fiter =data_keys_.find( key);
	if( fiter == data_keys_.end())
		return;

	int ind =fiter->second;
	datas_[ind].reset();

	data_vec_keys_[ind] =NO_INITVALUE;
	data_keys_.erase( fiter);
}

// DataVecTable
template < class T ,int MAX_SIZE>
class DataVecTable
{
protected:
    T           datas_[MAX_SIZE];
    int         data_size_;

public:
    void        reset();

    int         get_size();

    //Get Max Size
    int         get_max_size() { return MAX_SIZE; };
    const int   get_max_size() const { return MAX_SIZE; };

    // Remove the Index of Data
    //void        remove_data_byindex( int index );
    // Check Data by index, if keys is NO_INITVALUE, return null
    T*          has_data_byindex( int index );
    const T*    has_data_byindex( int index ) const;
    T*          get_data_from_end();
public:
    DataVecTable();
    ~DataVecTable();
};

template<class T ,int MAX_SIZE>
DataVecTable<T, MAX_SIZE>::DataVecTable()
{
    reset();
}

template<class T ,int MAX_SIZE>
DataVecTable<T, MAX_SIZE>::~DataVecTable()
{
    reset();
}

template<class T ,int MAX_SIZE>
void DataVecTable<T, MAX_SIZE>::reset()
{
    for( int ii =0; ii < MAX_SIZE; ++ii)
        datas_[ii].reset();
    data_size_= 0;
}

template<class T ,int MAX_SIZE>
int DataVecTable<T, MAX_SIZE>::get_size()
{
    return data_size_;
}

template<class T ,int MAX_SIZE>
T* DataVecTable<T, MAX_SIZE>::has_data_byindex( int index )
{
    if ( index >=0 && index < data_size_ )
    {
        return &(datas_[index]);
    }
    return 0;
}

template<class T ,int MAX_SIZE>
const T* DataVecTable<T, MAX_SIZE>::has_data_byindex( int index ) const
{
    if ( index >=0 && index < data_size_ )
    {
        return &(datas_[index]);
    }
    return 0;
}

template<class T ,int MAX_SIZE>
T* DataVecTable<T, MAX_SIZE>::get_data_from_end()
{
    if ( data_size_ < MAX_SIZE )
    {
        data_size_++;
        return &(datas_[data_size_-1]);
    }
    return 0;
}

#endif  //__DATA_TABLE_H__
