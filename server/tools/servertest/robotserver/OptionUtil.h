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

#pragma once

#include <windows.h>
#include <map>
#include <list>
#include <string>

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/util/FileUtil.h>

#include <gameLib/protobuf/Proto_all.h>

class TestCaseXml
{
public:
	NS_STL::string name_;
	NS_STL::string xml_;
};

class OptionUtil
{
public:
	class RobotDesctiption
	{
	public:
		int robotid_;
		NS_STL::string robotname_;
		NS_STL::string robotpwd_;
		int startuserid_;
		int users_;
		int totles_;
	};

	class DatabaseOption
	{
	public:
		//用户名口令
		std::string dbuser_;
		std::string dbpwd_;
		//数据库名字
		std::string dbname_;
		std::string dbip_;
	};

public:

	OptionUtil(void)
	{
	}

	virtual ~OptionUtil(void)
	{
	}

	const char* get_testcase( std::string name)
	{
		for( NS_STL::list<TestCaseXml>::iterator iter =testcase_.begin(); iter != testcase_.end(); ++iter)
		{
			TestCaseXml& tc =(*iter);
			if( tc.name_ == name)
				return tc.xml_.c_str();
		}

		return 0;
	}

	OptionUtil::RobotDesctiption* find_robot_byname( NS_STL::string name) 
	{
		NS_STL::map<int, RobotDesctiption>::iterator it = robots.begin();
		while ( it != robots.end())
		{
			if ( name == it->second.robotname_)
				return &it->second;
			it ++;
		}
		return 0;
	}

	OptionUtil::RobotDesctiption* find_robot_byid(int robotid)
	{
		NS_STL::map<int, RobotDesctiption>::iterator it = robots.find(robotid);
		if (it == robots.end())
			return 0;
		return &it->second;
	}

	void load_testcase()
	{
		std::list< std::string> xmls;
		FileUtil::get_filelist( "./server", ".xml", xmls);
		if( xmls.size() == 0)
			return;

		std::list< std::string>::iterator iter =xmls.begin(), eiter =xmls.end();
		for(; iter != eiter; ++iter)
		{
			std::string xn =(*iter);
			
			TestCaseXml tx;
			tx.name_ =xn;

			tx.xml_ =FileUtil::get_filecontent( "./server", xn.c_str());

			testcase_.push_back( tx);
		}
	}

	bool load( const char* filename)
	{
		load_testcase();

		char text[1024];
		GetCurrentDirectory( 1024, text);
		
		NS_STL::string filepath = NS_STL::string(text) + "\\" + filename;

		//db section
		GetPrivateProfileString("db", "dbip", "", text, sizeof(text), filepath.c_str());
		db_.dbip_ = text;
		GetPrivateProfileString("db", "dbuser", "", text, sizeof(text), filepath.c_str());
		db_.dbuser_ = text;
		GetPrivateProfileString("db", "dbpwd", "", text, sizeof(text), filepath.c_str());
		db_.dbpwd_ = text;
		GetPrivateProfileString("db", "dbname", "", text, sizeof(text), filepath.c_str());
		db_.dbname_ = text;

		if( GetPrivateProfileString( "run", "serverip", "", text, sizeof(text), filepath.c_str()) == 0 )
			return false;
		
		serverip_ = text;
		serverport_ = GetPrivateProfileInt("run", "serverport", 12345, filepath.c_str());

		if (GetPrivateProfileString("run", "phpurl", "", text, sizeof(text), filepath.c_str()) == 0)
			return false;
		phpurl_ = text;
		if (GetPrivateProfileString("run", "openprefix", "", text, sizeof(text), filepath.c_str()) == 0)
			return false;
		openprefix_ = text;


		if ((robottotal_ = GetPrivateProfileInt("run", "robottotal", 0, filepath.c_str())) == 0)
			return false;

		if (GetPrivateProfileString("run", "robotlist", "", text, sizeof(text), filepath.c_str()) == 0)
			return false;

		NS_STL::string robotlist = text;

		NS_STL::list<NS_STL::string> lls;
		char *buf = ::strtok(const_cast<char*>(robotlist.c_str()), ",");
		while (buf)
		{
			lls.push_back(buf);
			buf = ::strtok(0, ",");
		}

		NS_STL::list<NS_STL::string>::iterator iter = lls.begin();
		int ii = 0;
		for (iter = lls.begin(); iter != lls.end(); ++iter)
		{
			RobotDesctiption des;
			NS_STL::string ss = *iter;
			if (GetPrivateProfileString(ss.c_str(), "robotname", "", text, sizeof(text), filepath.c_str()) == 0)
				return false;
			des.robotname_ = text;

			if (GetPrivateProfileString(ss.c_str(), "robotpwd", "", text, sizeof(text), filepath.c_str()) == 0)
				return false;
			des.robotpwd_ = text;

			if ((des.startuserid_ = GetPrivateProfileInt(ss.c_str(), "startuserid ", 0xFFFFFFFF, filepath.c_str())) == 0xFFFFFFFF)
				return false;

			if ((des.users_ = GetPrivateProfileInt(ss.c_str(), "users", 0xFFFFFFFF, filepath.c_str())) == 0xFFFFFFFF)
				return false;

			des.robotid_ = ii;
			robots[ii] = des;
			ii++;
		}

		return (robots.size() > 0);
	}

	NS_STL::string get_urladdr() {
		return phpurl_;
	}
	NS_STL::string get_openprefix() { return openprefix_; }

public:
	NS_STL::string serverip_;
	unsigned short serverport_;

	NS_STL::string phpurl_;
	NS_STL::string openprefix_;

	unsigned short robottotal_;
	NS_STL::map<int, RobotDesctiption> robots;

	DatabaseOption	db_;

	NS_STL::list<TestCaseXml>	testcase_;
};
