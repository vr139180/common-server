/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2021
*/
#ifndef __PRO_MACRO__H__
#define __PRO_MACRO__H__

#include "cmsLib/prolib/prolibdef.h"

#include "cmsLib/prolib/core_type.h"

#include <algorithm>
#include <list>
#include <iterator>

#pragma warning(disable:4100 4189)

//映射宏
#define BEGIN_LOAD_PRO( pdata, totlen, ext)	\
	virtual bool to_protocol( char* pdata, S_INT_32& totlen, void* ext=0)	\
	{
#define END_LOAD_PRO()	\
		return true;	\
	}

#define BEGIN_SAVE_PRO( pdata, buflen, len, ext)	\
	virtual bool to_buffer( char* pdata, S_INT_32 buflen, S_INT_32& len, void* ext =0)	\
	{
#define END_SAVE_PRO()	\
		return true;	\
	}

#define BEGIN_CLONE_PRO( T, pro)	\
	virtual void clone_pro( BasicProtocol* pro2)	\
	{	\
		T * pro =(T*)pro2;
#define END_CLONE_PRO()	\
	}

//------------------------------导入宏------------------------------------
#define LOAD_LIST_PRO_BEGIN( pdata, llen, clstype)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _ls =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	for( S_INT_16 _i =0; _i < _ls; ++_i)	\
	{	\
		clstype _item;

#define LOAD_LIST_PRO_END( var)	\
		var.push_back( _item);	\
	}	\
	}

#define LOAD_VECTOR_PRO_BEGIN( pdata, llen, clstype)	\
    {	\
    llen -= sizeof( S_INT_16);	\
    if( llen < 0)	\
        return false;	\
    S_INT_16 _ls =*(reinterpret_cast<S_INT_16*>(pdata));	\
    pdata =(pdata+sizeof( S_INT_16));	\
    for( S_INT_16 _i =0; _i < _ls; ++_i)	\
    {	\
        clstype _item;

#define LOAD_VECTOR_PRO_END( var)	\
    var.push_back( _item);	\
    }	\
    }

#define LOAD_INT8_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_8);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_INT_8*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_8));	\
	}

#define LOAD_UINT8_PRO_LS( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_8);	\
    if( llen < 0)	\
    return false;	\
    _item.var =*(reinterpret_cast<S_UINT_8*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_8));	\
    }

#define LOAD_INT16_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	}

#define LOAD_UINT16_PRO_LS( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_16);	\
    if( llen < 0)	\
    return false;	\
    _item.var =*(reinterpret_cast<S_UINT_16*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_16));	\
    }

#define LOAD_INT32_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_32);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_INT_32*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_32));	\
	}

#define LOAD_UINT32_PRO_LS( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_32);	\
    if( llen < 0)	\
    return false;	\
    _item.var =*(reinterpret_cast<S_UINT_32*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_32));	\
    }

#define LOAD_INT64_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_64);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_INT_64*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_64));	\
	}

#define LOAD_UINT64_PRO_LS( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_64);	\
    if( llen < 0)	\
    return false;	\
    _item.var =*(reinterpret_cast<S_UINT_64*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_64));	\
    }

#define LOAD_FLOAT32_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_FLOAT_32);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_FLOAT_32*>(pdata));	\
	pdata =(pdata+sizeof( S_FLOAT_32));	\
	}

#define LOAD_TIMESTAMP_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_TIMESTAMP);	\
	if( llen < 0)	\
		return false;	\
	_item.var =*(reinterpret_cast<S_TIMESTAMP*>(pdata));	\
	pdata =( pdata +sizeof( S_TIMESTAMP));	\
	}

#define LOAD_STR_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _strlen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _strlen;	\
	if( llen < 0)	\
		return false;	\
	_item.var.clear();	\
	_item.var.append( pdata, _strlen);	\
	pdata =(pdata + _strlen);	\
	}

#define LOAD_OBJECT_PRO_LS( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_8);	\
	if( llen < 0)	\
		return false;	\
	S_INT_8 _type =*(reinterpret_cast<S_INT_8*>(pdata));	\
	pdata =(pdata + sizeof( S_INT_8));	\
	if( _type != SERIALIZEFACTORY_NONE)	\
	{	\
		SerializeObjectFactory* _factory =SerializeFactoryContainer::instance().get_factory( _type);	\
		if( _factory == 0)	\
			return false;	\
		_item.var =_factory->to_protocol( &pdata, llen, ext);	\
		if( _item.var == 0)	\
			return false;	\
	}	\
	else	\
		_item.var =0;	\
	}

#define LOAD_BUF_PRO_LS( pdata, llen, var, varlen)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _buflen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _buflen;	\
	if( llen < 0)	\
		return false;	\
	memcpy(_item.var, pdata, _buflen);	\
	pdata =(pdata + _buflen);	\
	}

#define LOAD_ARRAY_PRO_LS( pdata, llen, itemsize, var, varlen)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _buflen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _buflen*itemsize;	\
	if( llen < 0)	\
		return false;	\
	memcpy( _item.var, pdata, _buflen*itemsize);	\
	pdata =(pdata + _buflen*itemsize);	\
	_item.varlen = _buflen; \
	}

#define LOAD_STRUCT_PRO_LS( var )    \
    int len = totlen;   \
    var.to_protocol(pdata,totlen,ext);    \
    pdata += (len - totlen);


//---------------------load struct--------------------------
#define LOAD_STRUCT_PRO_BEGIN( pdata, llen, clstype, var) \
{ \
	clstype &_item = var;

// reuse list element load

#define LOAD_STRUCT_PRO_END \
	}

//---------------------normal load macro--------------------

#define LOAD_LIST_BEGIN( pdata, llen, var, clstype)    \
    llen -= sizeof( S_INT_16);	\
    if( llen < 0)	\
    return false;	\
    S_INT_16   var =*(reinterpret_cast<S_INT_16*>(pdata));	\
    pdata =(pdata+sizeof( S_INT_16));	\
    for( S_INT_16 _i =0; _i < var; ++_i)	\
    { \
        clstype _data;

#define LOAD_LIST_END( var )	\
        var.push_back( _data );	\
    }

#define LOAD_VECTOR_BEGIN( pdata, llen, var, clstype)    \
    llen -= sizeof( S_INT_16);	\
    if( llen < 0)	\
    return false;	\
    S_INT_16 var =*(reinterpret_cast<S_INT_16*>(pdata));	\
    pdata =(pdata+sizeof( S_INT_16));	\
    for( S_INT_16 _i =0; _i < var; ++_i)	\
    { \
        clstype _data;

#define LOAD_VECTOR_END( var )	\
        var.push_back( _data );	\
     }

#define LOAD_UINT8_PRO( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_8);	\
    if( llen < 0)	\
    return false;	\
    var =*(reinterpret_cast<S_UINT_8*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_8));	\
    }

#define LOAD_INT8_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_8);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_INT_8*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_8));	\
	}

#define LOAD_INT16_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	}

#define LOAD_UINT16_PRO( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_16);	\
    if( llen < 0)	\
    return false;	\
    var =*(reinterpret_cast<S_UINT_16*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_16));	\
    }

#define LOAD_INT32_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_32);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_INT_32*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_32));	\
	}

#define LOAD_UINT32_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_UINT_32);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_UINT_32*>(pdata));	\
	pdata =(pdata+sizeof( S_UINT_32));	\
	}

#define LOAD_INT64_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_64);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_INT_64*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_64));	\
	}

#define LOAD_UINT64_PRO( pdata, llen, var)	\
    {	\
    llen -= sizeof( S_UINT_64);	\
    if( llen < 0)	\
    return false;	\
    var =*(reinterpret_cast<S_UINT_64*>(pdata));	\
    pdata =(pdata+sizeof( S_UINT_64));	\
    }

#define LOAD_FLOAT32_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_FLOAT_32);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_FLOAT_32*>(pdata));	\
	pdata =(pdata+sizeof( S_FLOAT_32));	\
	}

#define LOAD_TIMESTAMP_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_TIMESTAMP);	\
	if( llen < 0)	\
		return false;	\
	var =*(reinterpret_cast<S_TIMESTAMP*>(pdata));	\
	pdata =( pdata +sizeof( S_TIMESTAMP));	\
	}

#define LOAD_STR_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _strlen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _strlen;	\
	if( llen < 0)	\
		return false;	\
	var.clear();	\
	var.append( pdata, _strlen);	\
	pdata =(pdata + _strlen);	\
	}

#define LOAD_OBJECT_PRO( pdata, llen, var)	\
	{	\
	llen -= sizeof( S_INT_8);	\
	if( llen < 0)	\
		return false;	\
	S_INT_8 _type =*(reinterpret_cast<S_INT_8*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_8));	\
	if( _type != SERIALIZEFACTORY_NONE)	\
	{	\
		SerializeObjectFactory* _factory =SerializeFactoryContainer::instance().get_factory( _type);	\
		if( _factory == 0)	\
			return false;	\
		var = _factory->to_protocol( &pdata, llen, ext);	\
		if( var == 0)	\
			return false;	\
	}	\
	else	\
		var =0;	\
	}

#define LOAD_BUF_PRO( pdata, llen, var, varlen)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _buflen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _buflen;	\
	if( llen < 0)	\
		return false;	\
	memcpy(var, pdata, _buflen);	\
	pdata =(pdata+_buflen);	\
	varlen = _buflen; \
	}

#define LOAD_ARRAY_PRO( pdata, llen, itemsize, var, varlen)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _buflen =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	llen -= _buflen*itemsize;	\
	if( llen < 0)	\
		return false;	\
	memcpy(var, pdata, _buflen*itemsize);	\
	pdata =(pdata+_buflen*itemsize);	\
	varlen = _buflen; \
	}

#define LOAD_STRUCT_PRO( var )   \
    {   \
        int len = totlen;   \
        var.to_protocol(pdata,totlen,ext);    \
        pdata += (len - totlen);    \
    }

//--------------------------------------load serialize object list-------------------------
#define LOAD_SERIALIZEOBJ_LIST_BEGIN( pdata, llen)	\
	{	\
	llen -= sizeof( S_INT_16);	\
	if( llen < 0)	\
		return false;	\
	S_INT_16 _ls =*(reinterpret_cast<S_INT_16*>(pdata));	\
	pdata =(pdata+sizeof( S_INT_16));	\
	for( S_INT_16 _i =0; _i < _ls; ++_i)	\
	{	\
		SerializeObject* _item =0;

#define LOAD_SERIALIZEOBJ_LIST_END( var)	\
		var.push_back( _item);	\
	}	\
	}

//------------------------------写入buffer宏------------------------------------
#define SAVE_LIST_PRO_BEGIN( pdata, buflen, llen, var, clstype)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)var.size();	\
	pdata =(pdata + sizeof(S_INT_16));	\
	std::list<clstype>::iterator _iter, _eiter;	\
	for( _iter =var.begin(), _eiter =var.end(); _iter != _eiter; ++_iter)	\
	{	\
		clstype &_item =(*_iter);

#define SAVE_LIST_PRO_END()	\
	}	\
	}

#define SAVE_VECTOR_PRO_BEGIN( pdata, buflen, llen, var, clstype)	\
    {	\
    llen += sizeof( S_INT_16);	\
    if( llen > buflen)	\
        return false;	\
    *((S_INT_16*)pdata) =(S_INT_16)var.size();	\
    pdata =(pdata + sizeof(S_INT_16));	\
    for( size_t i = 0; i<var.size(); ++i )	\
    {	\
        clstype &_item = var[i];

#define SAVE_VECTOR_PRO_END()	\
    }	\
    }

#define SAVE_INT8_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_8);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_8*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_INT_8));	\
	}

#define SAVE_UINT8_PRO_LS( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_8);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_8*)pdata) =_item.var;	\
    pdata =(pdata + sizeof(S_UINT_8));	\
    }

#define SAVE_INT16_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	}

#define SAVE_UINT16_PRO_LS( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_16);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_16*)pdata) =_item.var;	\
    pdata =(pdata + sizeof(S_UINT_16));	\
    }

#define SAVE_INT32_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_32*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_INT_32));	\
	}

#define SAVE_UINT32_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_UINT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_UINT_32*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_UINT_32));	\
	}

#define SAVE_INT64_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_64);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_64*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_INT_64));	\
	}

#define SAVE_UINT64_PRO_LS( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_64);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_64*)pdata) =_item.var;	\
    pdata =(pdata + sizeof(S_UINT_64));	\
    }

#define SAVE_FLOAT32_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_FLOAT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_FLOAT_32*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_FLOAT_32));	\
	}

#define SAVE_TIMESTAMP_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_TIMESTAMP);	\
	if( llen > buflen)	\
		return false;	\
	*((S_TIMESTAMP*)pdata) =_item.var;	\
	pdata =(pdata + sizeof(S_TIMESTAMP));	\
	}

#define SAVE_STR_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	S_INT_32 _strlen =(S_INT_32)_item.var.size();	\
	*((S_INT_16*)pdata) =(S_INT_16)_strlen;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += _strlen;	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, _item.var.c_str(), _strlen);	\
	pdata =(pdata+_strlen);	\
	}

#define SAVE_OBJECT_PRO_LS( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_8);	\
	if( llen > buflen)	\
		return false;	\
	S_INT_8 _type =SERIALIZEFACTORY_NONE;	\
	if( _item.var != 0)	\
	{	\
		_type =_item.var->factory_;	\
		*((S_INT_8*)pdata) =_type;	\
		pdata =(pdata + sizeof(S_INT_8));	\
		if( _item.var->data( &pdata, buflen, llen, ext) == false)	\
			return false;	\
	}	\
	else	\
	{	\
		*((S_INT_8*)pdata) =_type;	\
		pdata =(pdata + sizeof(S_INT_8));	\
	}	\
	}

#define SAVE_BUF_PRO_LS( pdata, buflen, llen, var, varlen)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)varlen;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += varlen;	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, (char*)_item.var, varlen);	\
	pdata =(pdata+varlen);	\
	}

#define SAVE_ARRAY_PRO_LS( pdata, buflen, llen, itemsize, var, varlen)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)(_item.varlen);	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += (_item.varlen*itemsize);	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, _item.var, _item.varlen*itemsize);	\
	pdata =(pdata+_item.varlen*itemsize);	\
	}

#define SAVE_STRUCT_PRO_LS( var )   \
    int ilen = len; \
    var.to_buffer(pdata,buflen,len,ext);  \
    pdata += (len - ilen);

//-----------------------------save struct--------------------------------------
#define SAVE_STRUCT_PRO_BEGIN( pdata, buflen, llen, var) \
{ \
	clstype &_item = var;

#define SAVE_STRUCT_PRO_END \
}

//-----------------------------save serialize object list-----------------------
#define SAVE_SERIALIZEOBJ_LIST_BEGIN( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)var.size();	\
	pdata =(pdata + sizeof(S_INT_16));	\
	std::list<SerializeObject*>::iterator _iter, _eiter;	\
	for( _iter =var.begin(), _eiter =var.end(); _iter != _eiter; ++_iter)	\
	{	\
		SerializeObject* _item =(*_iter);

#define SAVE_SERIALIZEOBJ_LIST_END()	\
	}	\
	}

//-----------------------------normal macro-------------------------------------
#define SAVE_LIST_BEGIN( pdata, buflen, llen, var, clstype)	\
    {	\
        llen += sizeof( S_INT_16);	\
        if( llen > buflen)	\
            return false;	\
        *((S_INT_16*)pdata) =(S_INT_16)var.size();	\
        pdata =(pdata + sizeof(S_INT_16));	\
        std::list<clstype>::iterator _iter, _eiter;	\
        for( _iter =var.begin(), _eiter =var.end(); _iter != _eiter; ++_iter)	\
        {	\
            clstype &_data =(*_iter);

#define SAVE_LIST_END()	\
        }\
    }

#define SAVE_VECTOR_BEGIN( pdata, buflen, llen, var, clstype)	\
    {	\
        llen += sizeof( S_INT_16);	\
        if( llen > buflen)	\
            return false;	\
        *((S_INT_16*)pdata) =(S_INT_16)var.size();	\
        pdata =(pdata + sizeof(S_INT_16));	\
        for( size_t i = 0; i< var.size(); ++i )	\
        {	\
        clstype &_data = var[i];

#define SAVE_VECTOR_END()	\
        }\
    }

#define SAVE_INT8_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_8);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_8*)pdata) =var;	\
	pdata =(pdata + sizeof(S_INT_8));	\
	}

#define SAVE_UINT8_PRO( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_8);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_8*)pdata) =var;	\
    pdata =(pdata + sizeof(S_UINT_8));	\
    }

#define SAVE_INT16_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =var;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	}

#define SAVE_UINT16_PRO( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_16);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_16*)pdata) =var;	\
    pdata =(pdata + sizeof(S_UINT_16));	\
    }

#define SAVE_INT32_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_32*)pdata) =var;	\
	pdata =(pdata + sizeof(S_INT_32));	\
	}

#define SAVE_UINT32_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_UINT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_UINT_32*)pdata) =var;	\
	pdata =(pdata + sizeof(S_UINT_32));	\
	}

#define SAVE_INT64_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_64);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_64*)pdata) =var;	\
	pdata =(pdata + sizeof(S_INT_64));	\
	}

#define SAVE_UINT64_PRO( pdata, buflen, llen, var)	\
    {	\
    llen += sizeof( S_UINT_64);	\
    if( llen > buflen)	\
    return false;	\
    *((S_UINT_64*)pdata) =var;	\
    pdata =(pdata + sizeof(S_UINT_64));	\
    }

#define SAVE_FLOAT32_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_FLOAT_32);	\
	if( llen > buflen)	\
		return false;	\
	*((S_FLOAT_32*)pdata) =var;	\
	pdata =(pdata + sizeof(S_FLOAT_32));	\
	}

#define SAVE_TIMESTAMP_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_TIMESTAMP);	\
	if( llen > buflen)	\
		return false;	\
	*((S_TIMESTAMP*)pdata) =var;	\
	pdata =(pdata + sizeof(S_TIMESTAMP));	\
	}

#define SAVE_STR_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	S_INT_32 _strlen =(S_INT_32)var.size();	\
	*((S_INT_16*)pdata) =(S_INT_16)_strlen;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += _strlen;	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, var.c_str(), _strlen);	\
	pdata =(pdata+_strlen);	\
	}

#define SAVE_OBJECT_PRO( pdata, buflen, llen, var)	\
	{	\
	llen += sizeof( S_INT_8);	\
	if( llen > buflen)	\
		return false;	\
	S_INT_8 _type =SERIALIZEFACTORY_NONE;	\
	if( var != 0)	\
	{	\
		_type =var->factory_;	\
		*((S_INT_8*)pdata) =_type;	\
		pdata =(pdata + sizeof(S_INT_8));	\
		if( var->data( &pdata, buflen, llen, ext) == false)	\
			return false;	\
	}	\
	else	\
	{	\
		*((S_INT_8*)pdata) =_type;	\
		pdata =(pdata + sizeof(S_INT_8));	\
	}	\
	}

#define SAVE_BUF_PRO( pdata, buflen, llen, var, varlen)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)varlen;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += varlen;	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, (char*)var, varlen);	\
	pdata =(pdata+varlen);	\
	}

#define SAVE_ARRAY_PRO( pdata, buflen, llen, itemsize, var, varlen)	\
	{	\
	llen += sizeof( S_INT_16);	\
	if( llen > buflen)	\
		return false;	\
	*((S_INT_16*)pdata) =(S_INT_16)varlen;	\
	pdata =(pdata + sizeof(S_INT_16));	\
	llen += varlen*itemsize;	\
	if( llen > buflen)	\
		return false;	\
	memcpy( pdata, var, varlen*itemsize);	\
	pdata =(pdata+varlen*itemsize);	\
	}

#define SAVE_STRUCT_PRO( var )   \
    {   \
        int ilen = len; \
        var.to_buffer(pdata,buflen,len,ext);  \
        pdata += (len - ilen);  \
    }

//------------------------------clone宏------------------------------------
#define CLONE_VAR_PRO( pro, var)	\
	{ pro->var =this->var; }

#define CLONE_BUF_PRO(pro, var, varlen) \
	{ \
	memcpy(pro->var, var, varlen); \
	}

#define CLONE_ARRAY_PRO(pro, itemsize, var, varlen) \
	{ \
	memcpy( pro->var, var, varlen*itemsize); \
	}

#define CLONE_LIST_PRO( pro, var)	\
	{	\
	std::copy( var.begin(), var.end(), std::back_inserter( pro->var));	\
	}

#define CLONE_VECTOR_PRO( pro, var)	\
    {	\
    std::copy( var.begin(), var.end(), std::back_inserter( pro->var));	\
    }

#define CLONE_SERIALIZEOBJ_LIST_PRO( pro, var)	\
	{	\
	std::list<SerializeObject*>::iterator iter =this->var.begin(), eiter =this->var.end();	\
	for( ; iter != eiter; ++iter){	\
		pro->var.push_back( (*iter)->clone());	\
	}	\
	}

#define CLONE_OBJECT_PRO( pro, var)	\
	{	\
	if( this->var)	\
		pro->var =this->var->clone();	\
	}

//--------------------------------------------------protocol factory macro---------------------------------------------------------
#define BEGIN_PRO_FACTORY()	\
	BasicProtocol* ProtocolFactory::data_protocol( char *pdata, S_INT_32 len, void* ext, bool internals)	\
	{	\
		BasicProtocol *ret =NULL;	\
		S_INT_32 iid =*((S_INT_32*)pdata);	\
		switch( iid)	\
		{

#define PRO_MAPPING( iid, pro)	\
		case iid:	\
			ret =PRO_NEW_EX pro();	\
			break;

#define END_PRO_FACTORY()	\
		}	\
		try	\
		{	\
			if( ret == 0 || !ret->format( pdata, len, ext, internals))	\
			{	\
				if( ret)	\
					delete ret;	\
				ret =0;	\
			}	\
		}	\
		catch(...)	\
		{	\
			if( ret)	\
				delete ret;	\
			ret =0;	\
		}	\
		return ret;	\
	}

//---------------------------------------------------serialize object factory macro------------------------------------------------
#define BEGIN_LOAD_SERIALIZEOBJ( _pdata, totlen, ext)	\
	virtual bool to_protocol( char** _pdata, S_INT_32& totlen, void* ext=0)	\
	{	\
		char* pdata =*_pdata;

#define INHERIT_LOAD_SERIALIZEOBJ( parent, _pdata, totlen, ext)	\
	if( parent::to_protocol( _pdata, totlen, ext) == false)	\
		return false;	\
	else	\
		pdata =*_pdata;

#define END_LOAD_SERIALIZEOBJ()	\
		*_pdata =pdata;	\
		return true;	\
	}

#define BEGIN_SAVE_SERIALIZEOBJ( _pdata, buflen, len, ext)	\
	virtual bool to_buffer( char** _pdata, S_INT_32 buflen, S_INT_32& len, void* ext =0)	\
	{	\
		char* pdata =*_pdata;

#define INHERIT_SAVE_SERIALIZEOBJ( parent, _pdata, buflen, len, ext)	\
	if( parent::to_buffer( _pdata, buflen, len, ext) == false)	\
		return false;	\
	else	\
		pdata =*_pdata;

#define END_SAVE_SERIALIZEOBJ()	\
		*_pdata =pdata;	\
		return true;	\
	}

#define BEGIN_CLONE_SERIALIZEOBJ( T, pro)	\
	virtual void clone_pro( SerializeObject* pro2)	\
	{	\
		T * pro =(T*)pro2;

#define INHERIT_CLONE_SERIALIZEOBJ( parent, pro)	\
		parent::clone_pro( pro);

#define END_CLONE_SERIALIZEOBJ()	\
	}

#define BEGIN_SERIALIZEOBJ_FACTORY( cls, serializeType)	\
	SerializeObject* cls::to_protocol( char** pdata, S_INT_32& len, void* ext)	\
	{	\
		SerializeObject *ret =NULL;	\
		S_INT_16 iid =*((S_INT_16*)(*pdata));	\
		switch( iid)	\
		{

#define SERIALIZEOBJ_MAPPING( iid, pro)	\
		case iid:	\
			ret =PRO_NEW_EX pro();	\
			break;

#define END_SERIALIZEOBJ_FACTORY( serializeType)	\
		}	\
		try	\
		{	\
			if( ret == 0 || !ret->format( pdata, len, ext))	\
			{	\
				if( ret)	\
					delete ret;	\
				ret =0;	\
			}	\
			else	\
				ret->factory_ =serializeType;	\
		}	\
		catch(...)	\
		{	\
			if( ret)	\
				delete ret;	\
			ret =0;	\
		}	\
		return ret;	\
	}

#endif //__PRO_MACRO__H__

