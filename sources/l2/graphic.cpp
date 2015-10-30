#include "graphic.h"

Vertex Graphic::cube_vertices_[] = {
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

unsigned int Graphic::cube_indices_[] = {
  0,1,2,3, // side left
  4,5,6,7, // side right
  8,9,10,11, // side back
  12,13,14,15, // side front
  16,17,18,19, // top
  20,21,22,23 // bottom
};

GLuint Graphic::cube_vbo_;
GLuint Graphic::cube_ibo_;

void Graphic::InitGL(void) {
  // Indices
  glGenBuffers(1, &Graphic::cube_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, Graphic::cube_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Graphic::cube_vertices_),
      Graphic::cube_vertices_, GL_STATIC_DRAW);

  glGenBuffers(1, &Graphic::cube_ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Graphic::cube_ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Graphic::cube_indices_),
      Graphic::cube_indices_, GL_STATIC_DRAW);
}

void Graphic::DestroyGL(void) {
}
