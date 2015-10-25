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
#include "graphic.h"
#include "rect.h"

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

class Cube : public Graphic, public Rect {
 private:
  // Collided with player
  bool collided_;
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
  Cube(float x, float y, float z, Texture *t) : Rect(x, y, z),
      collided_(false), t_side_(t), t_top_(t), t_bottom_(t) { };
  Cube(float x, float y, float z, Texture *t_side, Texture *t_top_bottom) :
      Rect(x, y, z), collided_(false), t_side_(t_side),
      t_top_(t_top_bottom), t_bottom_(t_top_bottom) { };
  Cube(float x, float y, float z, Texture *t_side, Texture *t_top,
      Texture *t_bottom) : Rect(x, y, z), collided_(false),
      t_side_(t_side), t_top_(t_top), t_bottom_(t_bottom) { };
  ~Cube() { DestroyGL(); };
  static void InitGL(void);
  static void DestroyGL(void);
  void Render();
  // Accessors
  void set_collided(bool collided) { collided_ = collided; };
  bool collided(void) { return collided_; };
};

#endif//__CUBE_H__
