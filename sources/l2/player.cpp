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

void Player::OnJoystick(Vector2f l, Vector2f r) {
  if (l.x != 0.f || l.y != 0.f) {
    Vector2f orig(0.f, 1.f);
    float angle = atan2(orig.y,orig.x) - atan2(l.y,l.x);
    angle = (angle >= 0) ? angle : angle + (2 * M_PI);
    angle = (angle / (2*M_PI)) * 360;
    f_ = angle - camera_->GetAngleH() - 90;
  }
  // Moving speed
  m_pm.x = -l.x;
  m_pm.y = 1.;
  m_pm.z = l.y;
}

Vector3f Player::NextPosition(void) {
  Vector3f np(0, 0, 0);
  const Vector3f Vaxis(0.0f, 1.0f, 0.0f);
  // Position move
  Vector3f pm = m_pm;
  // We consider (0, 1, 0) as the origin
  pm.Rotate(camera_->GetAngleH() + 90, Vaxis);
  np.x = x_ + pm.x / 10;
  np.z = z_ + pm.z / 10;
  np.y = y_;
  return np;
}

void Player::Update(void) {
  Vector3f np = NextPosition();
  x_ = np.x;
  z_ = np.z;
}
