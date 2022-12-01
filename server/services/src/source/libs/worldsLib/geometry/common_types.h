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
#include <gameLib/commons/GLoc3D.h>

class CMSPointXZ
{
public:
	static const CMSPointXZ& zero_point();

	static bool build_from_str(std::string& str, CMSPointXZ& p);

public:
	CMSPointXZ() :x_(0.0), z_(0.0) {}
	CMSPointXZ(float x, float z) :x_(x), z_(z) {}

	CMSPointXZ& operator = (const CMSPointXZ& v);

	float x()const { return x_; }
	void set_x(float x) { x_ = x; }
	float z()const { return z_; }
	void set_z(float z) { z_ = z; }

	std::string to_string();

private:
	float x_;
	float z_;
};

class CMSBox
{
public:
	CMSBox();
	CMSBox(float x, float z, float w, float h);

	void init_box(const CMSPointXZ& pos, float w, float h);
	void set_xz(float x, float z) { left_top_.set_x(x); left_top_.set_z(z); }
	void set_wh(float w, float h) { width_ = w; height_ = h; }

	bool is_point_in_box(const CMSPointXZ& p);
	bool is_point_in_box(const GLoc3D& loc);

	CMSBox& operator = (const CMSBox& v);

	float width() const { return width_; }
	float height() const { return height_; }

	void extend_left( int cells);
	void extend_right(int cells);
	void extend_top(int cells);
	void extend_buttom(int cells);

	//根据给出的GLoc3D世界坐标normlize出一个相对坐标
	CMSPointXZ normlize(const GLoc3D& loc);

private:
	CMSPointXZ left_top_;
	float width_;
	float height_;
};

#endif //__COMMON_TYPES_H__