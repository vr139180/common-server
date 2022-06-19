﻿#include "cmsLib/util/XmlUtil.h"

#include "cmsLib/GlobalSettings.h"

bool XmlUtil::loadfromfile(char * sfile, tinyxml2::XMLDocument& doc)
{
	if(sfile == 0)
		return false;

	size_t iSize=strlen(sfile);

	sfile[iSize]='\n';
	sfile[iSize+1]=0;
	sfile[iSize+2]='\0';
	
	if( doc.Parse(sfile) == 0)
		return false;

	return true;
}

bool XmlUtil::loadfromstring( const char *contents, tinyxml2::XMLDocument& doc )
{
	if( contents == 0 || strlen( contents) == 0)
		return false;

	doc.Parse( contents);
	if( doc.Error())
		return false;

	return true;
}


bool XmlUtil::GetXmlAttrBool( tinyxml2::XMLElement* e, const char* attr, bool defv)
{
	if( e == 0)
		return defv;

	const char* r =e->Attribute( attr);
	if( r == 0)
		return defv;

	if( strcasecmp( r, "true") ==0)
		return true;
	else if( strcasecmp( r, "false") ==0)
		return false;
	else
		return defv;
}

bool XmlUtil::GetXmlAttrYesNo( tinyxml2::XMLElement* e, const char* attr, bool defv)
{
	if( e == 0)
		return defv;

	const char* r =e->Attribute( attr);
	if( r == 0)
		return defv;

	if( strcasecmp( r, "yes") ==0)
		return true;
	else if( strcasecmp( r, "no") ==0)
		return false;
	else
		return defv;
}

bool XmlUtil::GetXmlAttrBoolFromInt( tinyxml2::XMLElement* e, const char* attr, bool defv, int ntruev)
{
	if( e == 0)
		return defv;

	int r =(defv?ntruev+1:ntruev);
	e->QueryIntAttribute( attr, &r);

	return (r != ntruev);
}

int XmlUtil::GetXmlAttrInt( tinyxml2::XMLElement* e, const char* attr, int defv)
{
	if( e == 0)
		return defv;

	int r =defv;
	e->QueryIntAttribute( attr, &r);

	return r;
}

float XmlUtil::GetXmlAttrFloat( tinyxml2::XMLElement* e, const char* attr, float defv)
{
	if( e == 0)
		return defv;

	float r =defv;
	e->QueryFloatAttribute( attr, &r);

	return r;
}

std::string XmlUtil::GetXmlAttrStr( tinyxml2::XMLElement* e, const char* attr, const char* defv)
{
	if( e == 0)
		return defv;


	char buf[255] = { 0 };
	const char* pbuf = &buf[0];
	if( e->QueryStringAttribute( attr, &pbuf) != tinyxml2::XML_SUCCESS)
		return defv;

	return pbuf;
}

std::string XmlUtil::GetXmlText( tinyxml2::XMLElement* e, const char* defv)
{
	if( e == 0)
		return defv;

	const tinyxml2::XMLNode* child = e->FirstChild();
	if( child)
	{
		const tinyxml2::XMLText* childText =child->ToText();
		if( childText)
			return childText->Value();
	}

	return defv;
}
