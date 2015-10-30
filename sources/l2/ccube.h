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

#ifndef __CCUBE_H__
#define __CCUBE_H__

#include <stdio.h>

#include "texture.h"
#include "graphic.h"

#include "math_3d.h"

class CCube : public Graphic {
 private:
  Vector4f color_;
 public:
  CCube(const Vector4f &color) : color_(color) { };
  void Draw(void);
  static void DrawPre(void);
  static void DrawPost(void);
  // Accessors
  Vector4f *color(void) { return &color_; }
};

#endif//__CCUBE_H__
