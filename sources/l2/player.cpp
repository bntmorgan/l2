#include <math.h>
#include "player.h"

void Player::OnJoystick(Vector2f l, Vector2f r) {
  if (l.x != 0.f || l.y != 0.f) {
    Vector2f orig(0.f, 1.f);
    float angle = atan2(orig.y,orig.x) - atan2(l.y,l.x);
    angle = (angle >= 0) ? angle : angle + (2 * M_PI);
    angle = (angle / (2*M_PI)) * 360;
    // printf("Joystick absolute angle (%f)\n", angle);
    // printf("Camera angle (%f)\n", camera_->GetAngleH());
    f_ = angle - camera_->GetAngleH() - 90;
  }
  // Moving speed
  m_pm.x = -l.x;
  m_pm.y = 1.;
  m_pm.z = l.y;
}

void Player::Update(void) {
  const Vector3f Vaxis(0.0f, 1.0f, 0.0f);
  // Position move
  Vector3f pm = m_pm;
  // printf("pos (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
  // XXX ? WTF
  pm.Rotate(camera_->GetAngleH() + 90, Vaxis);
  rx_ += pm.x / 10;
  rz_ += pm.z / 10;
}
