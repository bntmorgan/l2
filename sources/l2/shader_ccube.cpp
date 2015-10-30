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

#include <assert.h>

#include "shader_ccube.h"

void ShaderCCube::Link(void) {
  // GLSL 130 attribute location
  glBindAttribLocation(program_, 0, "Position");
  // Link
  Shader::Link();
  color_ = glGetUniformLocation(program_, "color");
  assert(color_ != 0xFFFFFFFF);
}
