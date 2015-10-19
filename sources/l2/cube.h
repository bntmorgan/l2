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

#ifndef __CUBE_H__
#define __CUBE_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "texture.h"

#include "math_3d.h"

struct Vertex {
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex) {
        m_pos = pos;
        m_tex = tex;
    }
};

class Cube {
 private:
  // Pointers
  // Scene *s_;
  // Coordinates
  int x_;
  int y_;
  int z_;
  // Textures
  Texture *t_side_;
  Texture *t_top_;
  Texture *t_bottom_;
  // GL
  static Vertex vertices_[];
  static unsigned int indices_[];
  static GLuint vbo_;
  static GLuint ibo_;
 public:
  Cube(int x, int y, int z, Texture *t) : x_(x), y_(y), z_(z), t_side_(t),
      t_top_(t), t_bottom_(t) {};
  Cube(int x, int y, int z, Texture *t_side, Texture *t_top_bottom) : x_(x),
      y_(y), z_(z), t_side_(t_side), t_top_(t_top_bottom),
      t_bottom_(t_top_bottom) {};
  Cube(int x, int y, int z, Texture *t_side, Texture *t_top, Texture *t_bottom)
      : x_(x), y_(y), z_(z), t_side_(t_side), t_top_(t_top),
      t_bottom_(t_bottom) {};
  ~Cube() { DestroyGL(); };

  static void InitGL(void);

  static void DestroyGL(void);

  void Dump(void);

  void Render();

  inline int x() {
    return x_;
  }
  inline int y() {
    return y_;
  }
  inline int z() {
    return z_;
  }
};

#endif//__CUBE_H__
