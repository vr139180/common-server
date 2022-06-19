/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __STATICSTRING__H__
#define __STATICSTRING__H__

#include "cmsLib/prolib/core_type.h"
#include <string>

template<int _STRINGLEN = 64>
class StaticString
{
public:
	StaticString();

	bool operator == ( const StaticString<_STRINGLEN>& v) const;
	bool operator == ( const char* pstr) const;
	void operator = ( const StaticString<_STRINGLEN>& v);
	void operator = ( const NS_STL::string& v);
	void operator = ( const char* pstr);

	operator NS_STL::string() const;
	operator const char*() const;

	void clear();
	void append( const char* pstr, int len);
	const char* c_str();
	int size();

private:
	char	str_buffer_[_STRINGLEN+1];
	int		str_length_;
};

template<int _STRINGLEN>
StaticString<_STRINGLEN>::StaticString()
{
	str_buffer_[0]= '\0';
	str_length_ =0;
}

template<int _STRINGLEN>
bool StaticString<_STRINGLEN>::operator == ( const StaticString<_STRINGLEN>& v) const
{
	if( this->str_length_ != v.str_length_)
		return false;
	return strcmp( this->str_buffer_, v.str_buffer_) == 0;
}

template<int _STRINGLEN>
bool StaticString<_STRINGLEN>::operator ==(const char *pstr) const
{
	if( pstr == 0 || this->str_length_ == 0)
		return false;
	return strcmp( this->str_buffer_, pstr) == 0;
}

template<int _STRINGLEN>
void StaticString<_STRINGLEN>::operator = ( const StaticString<_STRINGLEN>& v)
{
	this->str_length_ =v.str_length_;
	if( str_length_ > 0)
	{
		if( str_length_ >= _STRINGLEN)
			str_length_ =_STRINGLEN - 1;
		memcpy( &str_buffer_[0], v.str_buffer_, str_length_);
	}
	str_buffer_[str_length_] ='\0';
}

template<int _STRINGLEN>
void StaticString<_STRINGLEN>::operator = ( const char* pstr)
{
	if( pstr == 0)
		this->str_length_ =0;
	else
		str_length_ =(int)strlen( pstr);

	if( str_length_ >= _STRINGLEN)
		str_length_ =_STRINGLEN - 1;

	if( str_length_ > 0)
		memcpy( &str_buffer_[0], pstr, str_length_);
	str_buffer_[str_length_] ='\0';
}

template<int _STRINGLEN>
void StaticString<_STRINGLEN>::operator = ( const NS_STL::string& v)
{
	str_length_ =(int)v.size();
	if( str_length_ >= _STRINGLEN)
		str_length_ =_STRINGLEN - 1;

	if( str_length_ > 0)
		memcpy( &str_buffer_[0], v.c_str(), str_length_);
	str_buffer_[str_length_] ='\0';
}

template<int _STRINGLEN>
StaticString<_STRINGLEN>::operator NS_STL::string() const
{
	return str_buffer_;
}

template<int _STRINGLEN>
StaticString<_STRINGLEN>::operator const char*() const
{
	return str_buffer_;
}

template<int _STRINGLEN>
void StaticString<_STRINGLEN>::clear()
{
	this->str_length_ =0;
	this->str_buffer_[str_length_] ='\0';
}

template<int _STRLENGTH>
void StaticString<_STRLENGTH>::append( const char* pstr, int len)
{
	if( str_length_ + len >= _STRLENGTH)
		len =_STRLENGTH - str_length_ -1;
	
	if( len > 0)
	{
		memcpy( str_buffer_ + str_length_, pstr, len);
		str_length_ += len;
		str_buffer_[str_length_] ='\0';
	}
}

template<int _STRLENGTH>
const char* StaticString<_STRLENGTH>::c_str()
{
	return str_buffer_;
}

template<int _STRLENGTH>
int StaticString<_STRLENGTH>::size()
{
	return str_length_;
}

#endif	//__STATICSTRING__H__
