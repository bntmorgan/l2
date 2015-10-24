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

#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object {
 private:
 protected:
  float x_;
  float y_;
  float z_;
 public:
  Object(float x, float y, float z) : x_(x), y_(y), z_(z) { };
  float x() { return x_; }
  float y() { return y_; }
  float z() { return z_; }
  void Dump(void) {
    printf("x(%f), y(%f), z(%f)\n", x(), y(), z());
  }
};

#endif//__OBJECT_H__
