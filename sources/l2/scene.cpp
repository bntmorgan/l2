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

#include "scene.h"

#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

const char* filename_vs_cube = "sources/l2/shader.vs";
const char* filename_fs_cube = "sources/l2/shader.fs";

void Scene::CreateAABBTree(void) {
  std::vector<Rect *> rects(cubes_.begin(), cubes_.end());
  collider_ = new AABBTree(rects);
}

Scene::Scene(void) : camera_(Camera(WINDOW_WIDTH, WINDOW_HEIGHT)) {
  // Textures
  textures_ = new Textures;
  // Player
  player_ = new Player(0,  1,  0, textures_->texture("PLAYER"), &camera_);
  Cube::InitGL();
}

Scene::~Scene(void) {
  Cube::DestroyGL();
  delete collider_;
}

void Scene::CompileShaders() {
  // Compile Cube Shaders
  shader_cube_.AddShader(filename_vs_cube, GL_VERTEX_SHADER);
  shader_cube_.AddShader(filename_fs_cube, GL_FRAGMENT_SHADER);
  // GLSL 130 attribute location
  glBindAttribLocation(shader_cube_.program(), 0, "Position");
  glBindAttribLocation(shader_cube_.program(), 1, "TexCoord");
  // Link
  shader_cube_.Link();
  // World + view transformation 4x4 matrix
  world_ = glGetUniformLocation(shader_cube_.program(), "world");
  view_ = glGetUniformLocation(shader_cube_.program(), "view");
  proj_ = glGetUniformLocation(shader_cube_.program(), "proj");
  assert(world_ != 0xFFFFFFFF);
  g_sampler_ = glGetUniformLocation(shader_cube_.program(), "gSampler");
  assert(g_sampler_ != 0xFFFFFFFF);
  glUniform1i(g_sampler_, 0);
  shader_cube_.Use();
}

/**
 * XXX Scene test function
 */
Scene *Scene::CreateTestScene(void) {
  Scene *s = new Scene;

  // Shaders
  s->CompileShaders();

  Textures *ts = s->textures();

  s->AddCube(new Cube(0, -1,  0, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(0, -1,  1, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(1, -1,  0, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(1, -1,  1, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(0,  0,  0, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(0,  0,  1, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(1,  0,  0, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(1,  0,  1, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(-10,  0,  10, ts->texture("ROCK_TOP")));

  // Create test cubes
  s->AddCube(new Cube(-1,  0,  0, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube(-2,  0,  0, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-3,  0,  0, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 0,  0,  0, ts->texture("ROCKS")));
  s->AddCube(new Cube( 1,  0,  0, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 2,  0,  0, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 3,  0,  0, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-1,  0,  1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-2,  0,  1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-3,  0,  1, ts->texture("WOOD_SIDE"), ts->texture("WOOD_TOP")));
  s->AddCube(new Cube( 0,  0,  1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 1,  0,  1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 2,  0,  1, ts->texture("WATER_TOP")));
  s->AddCube(new Cube( 3,  0,  1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-1,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-2,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-3,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 0,  0, -1, ts->texture("ROCK_TOP")));
  s->AddCube(new Cube( 1,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 2,  1, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 3,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-1,  0, -2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-2,  0, -2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-3,  0, -2, ts->texture("WOOD_SIDE"), ts->texture("WOOD_TOP")));
  s->AddCube(new Cube( 0,  0, -2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 1,  0, -2, ts->texture("WATER_TOP")));
  s->AddCube(new Cube( 2,  0, -2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 3,  0, -2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-1,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-2,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube(-3,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 0,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 1,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 2,  1,  2, ts->texture("ROCKS")));
  s->AddCube(new Cube( 3,  0,  2, ts->texture("WATER_TOP")));
  s->AddCube(new Cube( 3, -1,  2, ts->texture("WOOD_SIDE"), ts->texture("WOOD_TOP")));

  // Perspective
  s->g_pers_proj_info()->FOV = 0.0f;
  s->g_pers_proj_info()->Height = CUBE_HEIGHT;
  s->g_pers_proj_info()->Width = CUBE_WIDTH;
  s->g_pers_proj_info()->zNear = -100.0f;
  s->g_pers_proj_info()->zFar = 100.0f;

  // AABBTree
  s->CreateAABBTree();

  // s->Dump();

  return s;
}

void Scene::OnKeyboard(int key) {
  if (key == OGLDEV_KEY_q) {
    exit(0);
  }
  camera_.OnKeyboard((OGLDEV_KEY)key);
}

void Scene::OnReshape(int width, int height) {
  g_pers_proj_info()->Width = CUBE_WIDTH * ((float)WINDOW_WIDTH / width);
  g_pers_proj_info()->Height = (g_pers_proj_info()->Width / ((float)width / height));
  camera_.SetWindow(width, height);
}

void Scene::OnMouse(int x, int y) {
  camera_.OnMouse(x, y);
}

void Scene::AddCube(Cube *c) {
  cubes_.push_back(c);
}

void Scene::Physics(void) {
  unsigned int i;
  for (i = 0; i < cubes_.size(); i++) {
    cubes_[i]->set_collided(cubes_[i]->Collide(player_));
  }
}

void Scene::Render(void) {
  unsigned int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_.OnRender();
  player_->Update();

  // Camera and perspective
  p_.SetCamera(camera_);
  p_.SetPerspectiveProj(g_pers_proj_info_);
  glUniformMatrix4fv(view_, 1, GL_TRUE, (const GLfloat*)p_.GetViewTrans());
  glUniformMatrix4fv(proj_, 1, GL_TRUE, (const GLfloat*)p_.GetOrthoProjTrans());

  // Cubes
  for (i = 0; i < cubes_.size(); i++) {
		p_.WorldPos(1.0f * cubes_[i]->x(), 1.0f * cubes_[i]->y(),
        1.0f * cubes_[i]->z());
    if (cubes_[i]->collided()) {
      p_.Scale(1.f, 2.f, 1.f);
    } else {
      p_.Scale(1.f, 1.f, 1.f);
    }
    p_.Rotate(0.f, 0.f, 0.0f);
    glUniformMatrix4fv(world_, 1, GL_TRUE, (const GLfloat*)p_.GetWorldTrans());
    cubes_[i]->Render();
  }

  // Player
  p_.WorldPos(1.0f * player_->x(), 1.0f * player_->y(),
      1.0f * player_->z());
  p_.Rotate(0.f, player_->f(), 0.0f);
  p_.Scale(1.f, 1.f, 1.f);
  glUniformMatrix4fv(world_, 1, GL_TRUE, (const GLfloat*)p_.GetWorldTrans());
  player_->Render();

  glutSwapBuffers();
}

void Scene::Dump(void) {
  unsigned int i;
  for (i = 0; i < cubes_.size(); i++) {
    cubes_[i]->Dump();
  }
}

void Scene::OnJoystickAxis(Vector2f l, Vector2f r) {
  l_ = l;
  r_ = r;
  camera_.OnJoystick(l_, r_);
  player_->OnJoystick(l_, r_);
}
