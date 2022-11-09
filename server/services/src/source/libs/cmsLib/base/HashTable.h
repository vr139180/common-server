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
#ifndef HASHTABLE_H
#define HASHTABLE_H

// HashTable

template < class Type >
class HashTable
{
public:
    HashTable( s32 tableSize = 256 );
    ~HashTable();

    u32         Allocated() const;
    u32         Size() const;

    void        Set( const char* key, Type& value );
    Type*       Get( const char* key ) const;
    bool        Remove( const char* key, Type& tex );

    void        Clear();

    s32         Num() const;
    Type*       GetIndex( s32 idx ) const;
    void        RemoveContents();
    s32         GetSpread() const;
protected:
    u32         GetHash( const char* key ) const;
    u32         Hash( const char* key ) const;
protected:
    struct hashNode_s{
        hashNode_s( const char* key, Type value, hashNode_s* next ) : mKey(key), mValue(value), mNext(next) {}
        String      mKey;
        Type        mValue;
        hashNode_s* mNext;
    };

    hashNode_s**    mHeads;
    s32             mTableSize;
    s32             mNumEntries;
    s32             mTableSizeMask;
};

template <class Type>
HashTable<Type>::HashTable( s32 tableSize /* = 256  */ ) : mTableSize( tableSize )
{
    assert( ewMath::IsPowerOfTwo( tableSize ) && tableSize > 0 );

    mHeads = new hashNode_s* [mTableSize];

    memset( mHeads, 0, sizeof(hashNode_s*) * mTableSize );
    mNumEntries     = 0;
    mTableSizeMask  = mTableSize - 1;
}

template <class Type>
HashTable<Type>::~HashTable()
{
    Clear();
    delete[] mHeads;
}

template <class Type>
u32 HashTable<Type>::Allocated() const
{
    return (u32)(sizeof(mHeads) * mTableSize + sizeof(*mHeads) * mNumEntries);
}

template <class Type>
u32 HashTable<Type>::Size() const
{
    return (u32)(sizeof(HashTable<Type>) + Allocated());
}

template <class Type>
u32 HashTable<Type>::GetHash( const char* key ) const
{
    return ( Hash( key ) & mTableSizeMask );
}

template <class Type>
u32 HashTable<Type>::Hash( const char* key ) const
{
    u32 hash = 0;
    for ( s32 idx = 0; *key != L'\0'; ++idx )
    {
        hash += ( *key++ ) * ( idx + 119 );
    }

    return hash;
}

template <class Type>
void HashTable<Type>::Set( const char* key, Type& value )
{
    hashNode_s* node, **nextPtr;
    u32 hash = 0;
    s32 ret = 0;

    hash = GetHash( key );
    for ( nextPtr = &(mHeads[hash]), node = *nextPtr; node != NULL; nextPtr = &(node->mNext), node = *nextPtr )
    {
        ret = node->mKey.compare( key );
        if ( ret == 0 )
        {
            return;
        }

        if ( ret > 0 )
        {
            break;
        }
    }

    ++mNumEntries;

    (*nextPtr) = new hashNode_s( key, value, mHeads[hash] );
    (*nextPtr)->mNext = node;
}

template <class Type>
Type* HashTable<Type>::Get( const char* key ) const
{
    hashNode_s* node;
    u32 hash = 0;
    s32 ret = 0;

    hash = GetHash( key );
    for ( node = mHeads[hash]; node != NULL; node = node->mNext )
    {
        ret = node->mKey.compare( key );
        if ( ret == 0 )
        {
            return &node->mValue;
        }

        if ( ret > 0 )
        {
            break;
        }
    }

    return NULL;
}

template <class Type>
Type* HashTable<Type>::GetIndex( s32 idx ) const
{
    hashNode_s* node;
    s32 count;

    if ( idx < 0 || idx >= mNumEntries )
    {
        assert( 0 );
        return NULL;
    }

    count = 0;
    for ( s32 i = 0; i < mTableSize; ++i )
    {
        for ( node = mHeads[i]; node != NULL; node = node->mNext )
        {
            if ( count == idx )
            {
                return &node->mValue;
            }

            ++count;
        }
    }

    return NULL;
}

template <class Type>
void HashTable<Type>::Clear()
{
    hashNode_s*     node;
    hashNode_s*     next;

    for ( s32 i = 0; i < mTableSize; ++i )
    {
        next = mHeads[i];
        while ( next != NULL )
        {
            node = next;
            next = node->mNext;
            delete node;
        }

        mHeads[i] = NULL;
    }

    mNumEntries = 0;
}

template <class Type>
bool HashTable<Type>::Remove( const char* key, Type& tex )
{
    hashNode_s**    head;
    hashNode_s*     node;
    hashNode_s*     prev;

    u32 hash = GetHash( key );
    head = &mHeads[hash];
    if ( *head )
    {
        for ( prev = NULL, node = *head; node != NULL; prev = node, node = node->mNext )
        {
            if ( node->mKey == key )
            {
                if ( prev )
                {
                    prev->mNext = node->mNext;
                }
                else
                {
                    *head = node->mNext;
                }

                tex= node->mValue;

                delete node;
                --mNumEntries;

                return true;
            }
        }
    }

    return false;
}

template <class Type>
s32 HashTable<Type>::Num() const
{
    return mNumEntries;
}

template <class Type>
void HashTable<Type>::RemoveContents()
{
    hashNode_s* node;
    hashNode_s* next;

    for ( s32 idx = 0; idx < mTableSize; ++idx )
    {
        next = mHeads[idx];
        while ( next )
        {
            node = next;
            next = next->mNext;
            delete node->mValue;
            delete node;
        }

        mHeads[idx] = NULL;
    }

    mNumEntries = 0;
}

template <class Type>
s32 HashTable<Type>::GetSpread() const
{
    if ( !mNumEntries )
    {
        return 100;
    }

    s32 average = mNumEntries / mTableSize;
    s32 error, numItems, e;

    for ( s32 idx = 0; idx < mTableSize; ++idx )
    {
        numItems = 0;
        for ( hashNode_s* node = mHeads[idx]; node != NULL; node = node->mNext )
        {
            ++numItems;
        }

        e = abs( numItems - average );
        if ( e > 1 )
        {
            error += e - 1;
        }
    }

    return 100 - ( error * 100 / mNumEntries );
}

#endif
