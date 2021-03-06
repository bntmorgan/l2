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

#ifndef __SHADER_CCUBE__
#define __SHADER_CCUBE__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"

class ShaderCCube : public Shader {
 private:
  GLuint color_;
 public:
  GLuint color(void) { return color_; }
  void Link(void);
};

#endif//__SHADER_CCUBE__
