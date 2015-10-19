#include <math.h>
#include "player.h"

void Player::OnJoystick(Vector2f l, Vector2f r) {
  Vector2f orig(0.f, 1.f);
  float angle = atan2(orig.y,orig.x) - atan2(l.y,l.x);
  angle = (angle >= 0) ? angle : angle + 2 * M_PI;
  printf("Joystick absolute angle (%f)\n", angle);
  f_ = angle;
}
