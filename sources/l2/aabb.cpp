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

#include "aabb.h"

int AABB::LargestDim(void) {
  int l = w(), i = AXIS_X;
  if (l < h()) {
    l = h();
    i = AXIS_Y;
  }
  if (l < d()) {
    l = d();
    i = AXIS_Z;
  }
  return i;
}

void AABB::Encompassing(AABB *out, AABB *b) {
  out->set_x((x() < b->x()) ? x() : b->x());
  out->set_y((y() < b->y()) ? y() : b->y());
  out->set_z((z() < b->z()) ? z() : b->z());
  out->set_w(((x() + w() > b->x() + b->w()) ? x() + w() : b->x() + b->w()) -
      out->x());
  out->set_h(((y() + h() > b->y() + b->h()) ? y() + h() : b->y() + b->h()) -
      out->y());
  out->set_d(((z() + d() > b->z() + b->d()) ? z() + d() : b->z() + b->d()) -
      out->z());
}

Vector3f AABB::Center(void) {
  return Vector3f(x() + w() / 2, y() + h() / 2, z() + d() / 2);
}

#define POW2(x) ((x) * (x))

float AABB::Distance(AABB *o) {
  Vector3f a = Center(), b = o->Center();
  return sqrt(POW2(a.x - b.x) + POW2(a.y - b.y) + POW2(a.z - b.z));
}
