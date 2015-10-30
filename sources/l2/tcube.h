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

#ifndef __TCUBE_H__
#define __TCUBE_H__

#include <stdio.h>

#include "texture.h"
#include "graphic.h"

#include "math_3d.h"

class TCube : public Graphic {
 private:
  // Collided with player
  bool collided_;
  // Textures
  Texture *t_side_;
  Texture *t_top_;
  Texture *t_bottom_;
 public:
  TCube(Texture *t) : collided_(false), t_side_(t), t_top_(t), t_bottom_(t) { };
  TCube(Texture *t_side, Texture *t_top_bottom) : collided_(false),
      t_side_(t_side), t_top_(t_top_bottom), t_bottom_(t_top_bottom) { };
  TCube(Texture *t_side, Texture *t_top, Texture *t_bottom) : collided_(false),
      t_side_(t_side), t_top_(t_top), t_bottom_(t_bottom) { };
  void Draw(void);
  // Accessors
  void set_collided(bool collided) { collided_ = collided; };
  bool collided(void) { return collided_; };
  static void DrawPre(void);
  static void DrawPost(void);
};

#endif//__TCUBE_H__
