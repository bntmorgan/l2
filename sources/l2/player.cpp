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

#include <math.h>
#include "player.h"

const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

float Player::JumpModel(int t) {
  float v;
  t -= 0;
  v = (-(t * t * 4.0) + 1200) / 14000;
  return (v > PLAYER_WEIGHT_MAX) ? v : PLAYER_WEIGHT_MAX;
}

void Player::OnJoystick(Vector2f l, Vector2f r) {
  if (l.x != 0.f || l.y != 0.f) {
    Vector2f orig(0.f, 1.f);
    float angle = atan2(orig.y,orig.x) - atan2(l.y,l.x);
    angle = (angle >= 0) ? angle : angle + (2 * M_PI);
    angle = (angle / (2*M_PI)) * 360;
    f_ = angle - camera_->GetAngleH() - 90;
  }
  // Moving speed
  m_pm.x = -l.x * PLAYER_SPEED;
  m_pm.y = 0.;
  m_pm.z = l.y * PLAYER_SPEED;
  // We consider (0, 1, 0) as the origin
  m_pm.Rotate(camera_->GetAngleH() + 90, Vaxis);
}

Vector3f Player::NextPosition(void) {
  Vector3f np(0, 0, 0);
  np.x = x_ + m_pm.x;
  np.y = y_ + m_pm.y + weight_.y;
  np.z = z_ + m_pm.z;
  return np;
}

Vector3f Player::NextPositionMove(void) {
  Vector3f np(0, 0, 0);
  np.x = x_ + m_pm.x;
  np.y = y_ + m_pm.y;
  np.z = z_ + m_pm.z;
  return np;
}

Vector3f Player::NextPositionWeight(void) {
  Vector3f np(0, 0, 0);
  np.y = y_ + weight_.y;
  return np;
}

void Player::Jump(void) {
  if (grounded_) {
    jumping_ = true;
    jump_t_ = 0;
  }
}

void Player::Update(void) {
  // We need to change player's weight according to jump model
  if (jumping_) {
    weight_.y = JumpModel(jump_t_);
    // printf("New weight ! %.3f\n", weight_.y);
    jump_t_++;
    if (weight_.y <= PLAYER_WEIGHT_MAX) {
      jumping_ = false;
    }
  }
  Vector3f np = NextPosition();
  x_ = np.x;
  y_ = np.y;
  z_ = np.z;
}
