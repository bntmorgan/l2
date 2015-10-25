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

#ifndef __RECT_H__
#define __RECT_H__

#include "geometry.h"

class Rect : public Geometry {
 private:
  float w_;
  float h_;
  float d_;
 public:
  Rect (float x, float y, float z, float w, float h, float d) : Geometry(x, y,
      z), w_(w), h_(h), d_(d) { }
  Rect (void) : Geometry(0, 0, 0), w_(0), h_(0), d_(0) { }
  Rect (float x, float y, float z) : Geometry(x, y, z), w_(1), h_(1), d_(1) { }
  bool Collide(Rect *p) {
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
  float w() { return w_; }
  float h() { return h_; }
  float d() { return d_; }
  void set_w(float w) { w_ = w; }
  void set_h(float h) { h_ = h; }
  void set_d(float d) { d_ = d; }
  void Dump(void) {
    printf("x(%f), y(%f), z(%f), w(%f), h(%f), d(%f)\n", x(), y(), z(), w(),
        h(), d());
  }
};

#endif//__RECT_H__
