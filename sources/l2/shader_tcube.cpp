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

#include "shader_tcube.h"

void ShaderTCube::Link(void) {
  // GLSL 130 attribute location
  glBindAttribLocation(program_, 0, "Position");
  glBindAttribLocation(program_, 1, "TexCoord");
  // Link
  Shader::Link();
  sampler_ = glGetUniformLocation(program_, "sampler");
  assert(sampler_ != 0xFFFFFFFF);
  glUniform1i(sampler_, 0);
}