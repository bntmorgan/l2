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

#include "cube.h"

Vertex Cube::vertices_[] = {
  Vertex(Vector3f(-0.5f,-0.5f,0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(0.5f,-0.5f,0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(0.5f,0.5f,0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(-0.5f,0.5f,0.5f), Vector2f(0.0,0.0)),
  Vertex(Vector3f(-0.5f,0.5f,-0.5f), Vector2f(0.0,0.0)),
  Vertex(Vector3f(0.5f,0.5f,-0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(0.5f,-0.5f,-0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(-0.5f,-0.5f,-0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(0.5f,-0.5f,0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(0.5f,-0.5f,-0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(0.5f,0.5f,-0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(0.5f,0.5f,0.5f), Vector2f(0.0,0.0)),
  Vertex(Vector3f(-0.5f,-0.5f,-0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(-0.5f,-0.5f,0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(-0.5f,0.5f,0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(-0.5f,0.5f,-0.5f), Vector2f(0.0,0.0)),
  Vertex(Vector3f(-0.5f,0.5f,0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(0.5f,0.5f,0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(0.5f,0.5f,-0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(-0.5f,0.5f,-0.5f), Vector2f(0.0,0.0)),
  Vertex(Vector3f(-0.5f,-0.5f,-0.5f), Vector2f(0.0,1.0)),
  Vertex(Vector3f(0.5f,-0.5f,-0.5f), Vector2f(1.0,1.0)),
  Vertex(Vector3f(0.5f,-0.5f,0.5f), Vector2f(1.0,0.0)),
  Vertex(Vector3f(-0.5f,-0.5f,0.5f), Vector2f(0.0,0.0))
};

unsigned int Cube::indices_[] = {
  0,1,2,3, // side left
  4,5,6,7, // side right
  8,9,10,11, // side back
  12,13,14,15, // side front
  16,17,18,19, // top
  20,21,22,23 // bottom
};

GLuint Cube::vbo_;
GLuint Cube::ibo_;

void Cube::Dump(void) {
  printf("x(%d), y(%d), z(%d)\n", x(), y(), z());
}

void Cube::Render() {
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, Cube::vbo_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (const GLvoid*)sizeof(Vector3f));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  // Sides
  t_side_->Bind(GL_TEXTURE0);
  glDrawElements(GL_QUADS, 16, GL_UNSIGNED_INT, 0);
  // Bottom
  t_bottom_->Bind(GL_TEXTURE0);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (const GLvoid *)
      (16 * sizeof(unsigned int)));
  // Up
  t_top_->Bind(GL_TEXTURE0);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (const GLvoid *)
      (20 * sizeof(unsigned int)));

  glDisableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void Cube::InitGL(void) {
  // Indices
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_,
      GL_STATIC_DRAW);
}

void Cube::DestroyGL(void) {
}
