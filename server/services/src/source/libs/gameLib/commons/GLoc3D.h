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

#ifndef __GLOC3D_H__
#define __GLOC3D_H__

#include <string>
#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

class GLoc3D
{
public:
	static const GLoc3D& zero_point();

	static bool build_from_str(std::string& str, GLoc3D& p);

	static void copy_to(const GLoc3D& loc, PRO::Location3D* pos);
	static void copy_to(PRO::Location3D* pos, GLoc3D& loc);

public:
	GLoc3D() :x_(0.0), y_(0.0), z_(0.0) {}
	GLoc3D(float x, float y, float z) :x_(x), y_(y), z_(z) {}

	GLoc3D& operator = (const GLoc3D& v);

	float x()const { return x_; }
	void set_x(float x) { x_ = x; }
	float y()const { return y_; }
	void set_y(float y) { y_ = y; }
	float z()const { return z_; }
	void set_z(float z) { z_ = z; }

	std::string to_string() const;
	bool is_zero_point() const;

	bool is_loc_change(const GLoc3D& loc);

private:
	float x_;
	float y_;
	float z_;
};

#endif //__GLOC3D_H__