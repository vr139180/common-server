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

#ifndef __UPLOADLOGUTIL_H__
#define __UPLOADLOGUTIL_H__

#include <cmsLib/util/FileUtil.h>
#include <cmsLib/ThreadLock.h>
#include "uploads/LogSaveUtil.h"

#define LOG_HEADER "#analysis#"

class UploadLogUtil
{
public:
	UploadLogUtil();
	~UploadLogUtil();

	void start_uploads();
	void stop_uploads();

	int get_lognums() { return log_num_; }
	void clear_logs();

protected:
	static DWORD  WINAPI Thread(LPVOID  lparam);
	void main_loop();

	void set_cancel();

protected:
	std::list<std::string> log_files;
	int	log_num_;

private:
	std::string log_dir_;

	bool	cancel_;
	HANDLE	hthread;
	DWORD	dwThreadId;
};

#endif //__UPLOADLOGUTIL_H__