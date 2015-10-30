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

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ccube.h"

void CCube::DrawPre(void) {
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, Graphic::ccube_vbo_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Graphic::cube_ibo_);
}

void CCube::DrawPost(void) {
  glDisableVertexAttribArray(0);
}

void CCube::Draw() {
  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
}
