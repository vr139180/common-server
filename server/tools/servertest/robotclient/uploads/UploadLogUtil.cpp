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

#include "StdAfx.h"

#include "UploadLogUtil.h"

#include <cmsLib/util/FileUtil.h>

extern CWindow	*g_mainwnd;

extern LogSaveUtil* g_logsave;

UploadLogUtil::UploadLogUtil() :
cancel_(false),
hthread(0),
dwThreadId(0)
{
	log_dir_ = "./";
}

UploadLogUtil::~UploadLogUtil()
{
	stop_uploads();
}

void UploadLogUtil::clear_logs()
{
	//FileUtil::del_specialfiles(log_dir_.c_str(), ".bak");
}

void UploadLogUtil::start_uploads()
{
	stop_uploads();

	//get all log files
	log_files.clear();
	FileUtil::get_filelist(log_dir_.c_str(), ".bak", log_files);

	hthread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, this, 0, &dwThreadId);
}

void UploadLogUtil::stop_uploads()
{
	cancel_ = true;
	if (hthread > 0)
	{
		::WaitForSingleObject(hthread, INFINITE);
		CloseHandle(hthread);
	}

	hthread = 0;
	dwThreadId = 0;
	cancel_ = false;
}

DWORD  WINAPI UploadLogUtil::Thread(LPVOID  lparam)
{
	UploadLogUtil *pThis = reinterpret_cast<UploadLogUtil*>(lparam);

	srand(static_cast<int>(time(0)));

	pThis->main_loop();

	return 1;
}

void UploadLogUtil::set_cancel()
{
	cancel_ = true;
};

#include <boost/filesystem.hpp>
#include <cmsLib/Log.h>

void UploadLogUtil::main_loop()
{
	::Sleep(1000);
	log_num_ = 0;
	g_mainwnd->PostMessage(WM_USER + 0x200, (WPARAM)0, 0);

	boost::filesystem::ifstream logf;
	bool uploading = false;

	g_logsave->start_save();

	while (1)
	{
		if (cancel_)
		{
			if( uploading)
				logf.close();
			break;
		}

		//1s send 10 logs
		::Sleep(1);

		if (!uploading)
		{
			if (log_files.size() == 0)
				break;

			std::string fs = log_files.front();
			log_files.pop_front();

			logf.open(fs);
			if (logf.is_open())
				uploading = true;
		}
		else
		{
			std::string str;
			if (std::getline(logf, str))
			{
				int ind = str.find(LOG_HEADER);
				if (ind < 0)
					continue;
				ind += strlen(LOG_HEADER);
				str = str.substr(ind);

				g_logsave->save_log(str.c_str());
			}
			else
			{
				uploading = false;
				logf.close();
			}
		}
	}

	g_logsave->set_no_logscaned();
}
