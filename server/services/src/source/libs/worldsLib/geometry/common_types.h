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

#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__

#include <string>

class CMSPointXY
{
public:
	static const CMSPointXY& zero_point();

	static bool build_from_str(std::string& str, CMSPointXY& p);

public:
	CMSPointXY() :x_(0.0), y_(0.0) {}
	CMSPointXY(float x, float y) :x_(x), y_(y) {}

	CMSPointXY& operator = (const CMSPointXY& v);

	float x()const { return x_; }
	void set_x(float x) { x_ = x; }
	float y()const { return y_; }
	void set_y(float y) { y_ = y; }

	std::string to_string();

private:
	float x_;
	float y_;
};

class CMSBox
{
public:
	CMSBox();
	CMSBox(float x, float y, float w, float h);

	void init_box(const CMSPointXY& pos, float w, float h);

	bool is_point_in_box(const CMSPointXY& p);

	CMSBox& operator = (const CMSBox& v);

private:
	CMSPointXY left_top_;
	float width_;
	float height_;
};

#endif //__COMMON_TYPES_H__