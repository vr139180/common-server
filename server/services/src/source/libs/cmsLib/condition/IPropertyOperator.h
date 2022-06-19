#ifndef __IPROPERTYOPERATOR__H__
#define __IPROPERTYOPERATOR__H__

#include <string>
#include <assert.h>

enum PROPERTYOPERATOR_VALUETYPE
{
	PROPERTYOPER_VT_INT =0,
	PROPERTYOPER_VT_FLOAT,
	PROPERTYOPER_VT_STR,
};

class PropertyOperatorValue
{
public:
	PropertyOperatorValue():value_type_(PROPERTYOPER_VT_INT){
		value_.int_ =0;
	}

	void set_int( int v){
		value_.int_ =v;
		value_type_ =PROPERTYOPER_VT_INT;
	}
	int get_int(){
		assert( value_type_ == PROPERTYOPER_VT_INT);
		return value_.int_;
	}

	void set_float( float v){
		value_.float_ =v;
		value_type_ =PROPERTYOPER_VT_FLOAT;
	}
	float get_float(){
		assert( value_type_ == PROPERTYOPER_VT_FLOAT);
		return value_.float_;
	}

	void set_str( const char* v){
		str_ =v;
		value_type_ =PROPERTYOPER_VT_STR;
	}

	const std::string get_str(){
		assert( value_type_ == PROPERTYOPER_VT_STR);
		return str_;
	}

	bool operator > ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ > (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ > (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ > v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator >= ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ >= (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ >= (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ >= v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator < ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ < (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ < (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ < v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator <= ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ <= (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ <= (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ <= v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator == ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ == (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ == (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ == v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator != ( const PropertyOperatorValue& v) const{
		assert( !((value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_));
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ != (int)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ != (float)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ != v.str_;
		default:
			assert( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

public:
	union
	{
		int		int_;
		float	float_;
	}value_;
	std::string	str_;

	PROPERTYOPERATOR_VALUETYPE	value_type_;
};

class IPropertyOperator
{
public:
	virtual PropertyOperatorValue get_property( const char* propname) =0;

};

#endif	//__IPROPERTYOPERATOR__H__
