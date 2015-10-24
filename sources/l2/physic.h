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

#ifndef __PHYSIC_H__
#define __PHYSIC_H__

#include "object.h"

class Physic : public Object{
 private:
  float w_;
  float h_;
  float d_;
 public:
  Physic (float x, float y, float z) : Object(x, y, z), w_(1), h_(1), d_(1) { }
  bool Collide(Physic *p) {
    return !((p->x() >= x_ + w_)     // trop à droite
        || (p->x() + p->w() <= x_)   // trop à gauche
        || (p->y() >= y_ + h_)       // trop en bas
        || (p->y() + p->h() <= y_)   // trop en haut	
        || (p->z() >= z_ + d_)       // trop derrière
        || (p->z() + p->d() <= z_)); // trop devant
  }
  // Accessors
  float w() { return w_; }
  float h() { return h_; }
  float d() { return d_; }
};

#endif//__PHYSIC_H__
