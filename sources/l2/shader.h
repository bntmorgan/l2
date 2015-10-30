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

#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <GL/freeglut.h>

class Shader {
 private:
  GLuint world_; // Object position in world
  GLuint view_; // Camera view
  GLuint proj_; // Projection
 protected:
  GLint program_;
 public:
  Shader(void);
  ~Shader(void);
  void Use(void);
  void AddShader(const char *filename, GLenum shader_type);
  virtual void Link(void);
  // Accessors
  GLint program(void) { return program_; }
  GLint world(void) { return world_; }
  GLint view(void) { return view_; }
  GLint proj(void) { return proj_; }
};

#endif//__SHADER_H__
