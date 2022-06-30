#ifndef __XMLUTIL__H__
#define __XMLUTIL__H__

#include <string>

#include "cmsLib/tinyxml2/tinyxml2.h"

/**
* @class	XmlUtil
* @author	Created by Lu Jianfeng ( lujf1978@163.com ), 2009
* 
* @brief xml文件处理类
**/
class XmlUtil
{
private:
	XmlUtil(){};
	~XmlUtil(){};

public:
	static bool loadfromfile(char * sfile, tinyxml2::XMLDocument& doc);
	static bool loadfromstring(const char *contents, tinyxml2::XMLDocument& doc);

	/**
	* 获取xml节点属性
	* @param e		节点
	* @param attr	属性名称
	* @param defv	缺省值
	* @return 
	**/
	static bool GetXmlAttrBool( tinyxml2::XMLElement* e, const char* attr, bool defv=false);
	static bool GetXmlAttrYesNo( tinyxml2::XMLElement* e, const char* attr, bool defv=false);
	static int GetXmlAttrInt( tinyxml2::XMLElement* e, const char* attr, int defv =0);
	static float GetXmlAttrFloat( tinyxml2::XMLElement* e, const char* attr, float defv =0.f);
	static std::string GetXmlAttrStr( tinyxml2::XMLElement* e, const char* attr, const char* defv="");

	/**
	* 获取xml节点属性
	* @param e		节点
	* @param attr	属性名称
	* @param defv	缺省值
	* @param ntruev	true值的int定义
	* @return bool
	**/
	static bool GetXmlAttrBoolFromInt( tinyxml2::XMLElement* e, const char* attr, bool defv=false, int ntruev =0);

	static std::string GetXmlText( tinyxml2::XMLElement* e, const char* defv="");

	static std::string build_path(const char* fmt, ...);
};

#endif	//__XMLUTIL__H__
