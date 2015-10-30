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

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cube.h"
#include "aabb.h"
#include "texture.h"
#include "camera.h"

class Player: public AABB, public Cube {
 private:
   // Facing
   float f_;
   // Camera
   Camera *camera_;
   // Moving speed
   Vector3f m_pm;
 public:
  Player(float x, float y, float z, Texture *t, Camera *c) : AABB(x, y, z),
      Cube(t), f_(0), camera_(c), m_pm(0, 0, 0) { };
  void OnJoystick(Vector2f l, Vector2f r);
  void Update(void);
  float f(void) { return f_; }
};

#endif//__PLAYER_H__
