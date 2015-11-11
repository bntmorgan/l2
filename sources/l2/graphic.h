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

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math_3d.h"

struct Vertex2 {
  Vector3f pos;
  Vector2f tex;
};

};

class Graphic {
 public:
  virtual void Draw(void) = 0;
  virtual ~Graphic(void) {};
  // GL
  static Vertex2 cube_vertices_[];
  static Vector3f ccube_vertices_[];
  static unsigned int cube_indices_[];
  static GLuint cube_vbo_;
  static GLuint ccube_vbo_;
  static GLuint cube_ibo_;
  static void InitGL(void);
  static void DestroyGL(void);
};

#endif//__GRAPHIC_H__
