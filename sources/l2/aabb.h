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

#ifndef __AABB_H__
#define __AABB_H__

#include <stdio.h>

enum Axis {
  AXIS_X,
  AXIS_Y,
  AXIS_Z
};

class AABB {
 protected:
  float x_;
  float y_;
  float z_;
  float w_;
  float h_;
  float d_;
 public:
  AABB(float x, float y, float z, float w, float h, float d) : x_(x), y_(y),
      z_(z), w_(w), h_(h), d_(d) { }
  AABB(void) : x_(0), y_(0), z_(0), w_(0), h_(0), d_(0) { }
  AABB(float x, float y, float z) : x_(x), y_(y), z_(z), w_(1), h_(1),
      d_(1) { }
  bool Collide(AABB *p) {
    return !((p->x() >= x_ + w_)     // trop à droite
        || (p->x() + p->w() <= x_)   // trop à gauche
        || (p->y() >= y_ + h_)       // trop en bas
        || (p->y() + p->h() <= y_)   // trop en haut	
        || (p->z() >= z_ + d_)       // trop derrière
        || (p->z() + p->d() <= z_)); // trop devant
  }
  int LargestDim(void);
  float Volume(void) { return w() * h() * d(); }
  // Accessors
  float x() { return x_; }
  float y() { return y_; }
  float z() { return z_; }
  float glx() { return x_ + w_ / 2 - 0.5; }
  float gly() { return y_ + h_ / 2 - 0.5; }
  float glz() { return z_ + d_ / 2 - 0.5; }
  void set_x(float x) { x_ = x; }
  void set_y(float y) { y_ = y; }
  void set_z(float z) { z_ = z; }
  float w() { return w_; }
  float h() { return h_; }
  float d() { return d_; }
  void set_w(float w) { w_ = w; }
  void set_h(float h) { h_ = h; }
  void set_d(float d) { d_ = d; }
  void Dump(void) {
    printf("x(%.3f), y(%.3f), z(%.3f), w(%.3f), h(%.3f), d(%.3f)\n", x(), y(),
        z(), w(), h(), d());
  }
};

#endif//__AABB_H__
