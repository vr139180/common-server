#ifndef __DATABASEOPTION__H__
#define __DATABASEOPTION__H__

#include <string>

namespace tinyxml2 {
	class XMLElement;
}

namespace config {

	class DatabaseOption
	{
	public:
		DatabaseOption();

		void load_from_xml(tinyxml2::XMLElement* el);

	public:
		//用户名口令
		std::string dbuser_;
		std::string dbpwd_;
		//数据库名字
		std::string dbname_;
		std::string dbip_;
	};
}

#endif	//__DATABASEOPTION__H__
