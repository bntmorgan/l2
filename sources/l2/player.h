#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cube.h"
#include "texture.h"

class Player: public Cube {
 private:
   float rx_;
   float ry_;
   float rz_;
   // Facing
   float f_;
 public:
  Player(int x, int y, int z, Texture *t) : Cube(x, y, z, t), rx_(x), ry_(y),
      rz_(z) {};
  void OnJoystick(Vector2f l, Vector2f r);
  float rx(void) { return rx_; }
  float ry(void) { return ry_; }
  float rz(void) { return rz_; }
  float f(void) { return f_; }
};

#endif//__PLAYER_H__
