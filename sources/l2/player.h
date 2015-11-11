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

#include "tcube.h"
#include "aabb.h"
#include "texture.h"
#include "camera.h"

#define PLAYER_SPEED 0.10
#define PLAYER_WEIGHT_MAX -0.2f

class Player: public AABB, public TCube {
 private:
  // Facing
  float f_;
  // Camera
  Camera *camera_;
  // Moving speed
  Vector3f m_pm;
  Vector3f weight_;
  // Jump function
  int jump_t_;
  // Jump Model
  float JumpModel(int t);
  bool jumping_;
  bool grounded_;
 public:
  Player(float x, float y, float z, Texture *t, Camera *c) : AABB(x, y, z),
      TCube(t), f_(0), camera_(c), m_pm(0, 0, 0), weight_(0, PLAYER_WEIGHT_MAX, 0),
      jumping_(false), grounded_(false) { };
  Vector3f NextPosition(void);
  Vector3f NextPositionMove(void);
  Vector3f NextPositionWeight(void);
  void OnJoystick(Vector2f l, Vector2f r);
  void Update(void);
  void Jump(void);
  void Land(void);
  // Accessors
  float f(void) { return f_; }
  Vector3f pm(void) { return m_pm; }
  void set_pm(Vector3f pm) { m_pm = pm; }
  Vector3f weight(void) { return weight_; }
  void set_weight(Vector3f weight) { weight_ = weight; }
  bool grounded(void) { return grounded_; }
  void set_grounded(bool grounded) { grounded_ = grounded; }
};

#endif//__PLAYER_H__
