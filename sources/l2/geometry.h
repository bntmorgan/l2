/*
Copyright (C) 2015  Benoît Morgan

This file is part of L2.

L2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

L2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with L2.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdio.h>

enum Axis {
  AXIS_X,
  AXIS_Y,
  AXIS_Z
};

class Geometry {
 private:
 protected:
  float x_;
  float y_;
  float z_;
 public:
  Geometry (float x, float y, float z) : x_(x), y_(y), z_(z) { };
  float x() { return x_; }
  float y() { return y_; }
  float z() { return z_; }
  void set_x(float x) { x_ = x; }
  void set_y(float y) { y_ = y; }
  void set_z(float z) { z_ = z; }
  void Dump(void) {
    printf("x(%f), y(%f), z(%f)\n", x(), y(), z());
  }
};

#endif//__GEOMETRY_H__
