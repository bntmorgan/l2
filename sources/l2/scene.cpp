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
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "ccube.h"

const char* filename_vs_tcube = "sources/l2/shaders/tcube.vs";
const char* filename_fs_tcube = "sources/l2/shaders/tcube.fs";
const char* filename_vs_ccube = "sources/l2/shaders/ccube.vs";
const char* filename_fs_ccube = "sources/l2/shaders/ccube.fs";

void Scene::CreateAABBTree(void) {
  std::vector<Object *> rects(cubes_.begin(), cubes_.end());
  collider_ = new AABBTree(rects, 100);
}

Scene::Scene(void) : camera_(Camera(WINDOW_WIDTH, WINDOW_HEIGHT)) {
  // Textures
  textures_ = new Textures;
  // Player
  player_ = new Player(0,  1,  0, textures_->texture("PLAYER"), &camera_);
  Graphic::InitGL();
  display_collider_ = false;
}

Scene::~Scene(void) {
  Graphic::DestroyGL();
  delete collider_;
}

void Scene::CompileShaders() {
  // Compile Textured Cube Shaders
  shader_tcube_.AddShader(filename_vs_tcube, GL_VERTEX_SHADER);
  shader_tcube_.AddShader(filename_fs_tcube, GL_FRAGMENT_SHADER);
  shader_tcube_.Link();
  // Compile Textured Cube Shaders
  shader_ccube_.AddShader(filename_vs_ccube, GL_VERTEX_SHADER);
  shader_ccube_.AddShader(filename_fs_ccube, GL_FRAGMENT_SHADER);
  shader_ccube_.Link();
}

/**
 * XXX Scene test function
 */
#define CUBES_NB 5
Scene *Scene::CreateTestScene(void) {
  Scene *s = new Scene;
  int i, j;

  // Shaders
  s->CompileShaders();

  Textures *ts = s->textures();

  for (i = -CUBES_NB; i <= CUBES_NB; i++) {
    for (j = -CUBES_NB; j <= CUBES_NB; j++) {
      if (i == -CUBES_NB || i == CUBES_NB || j == -CUBES_NB || j == CUBES_NB) {
        s->AddCube(new Object(new AABB(i,  0,  j),
            new TCube(ts->texture("ROCKS"))));
        s->AddCube(new Object(new AABB(i,  1,  j),
            new TCube(ts->texture("ROCKS"))));
      }
      if (i <= -CUBES_NB + 2 || i >= CUBES_NB - 2 ||
          j <= -CUBES_NB + 2 || j >= CUBES_NB - 2) {
        s->AddCube(new Object(new AABB(i,  0,  j),
            new TCube(ts->texture("WATER_TOP"))));
      } else {
        s->AddCube(new Object(new AABB(i,  0,  j),
            new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
      }
    }
  }

  // Perspective
  s->g_pers_proj_info()->FOV = 0.0f;
  s->g_pers_proj_info()->Height = CUBE_HEIGHT;
  s->g_pers_proj_info()->Width = CUBE_WIDTH;
  s->g_pers_proj_info()->zNear = -30.0f;
  s->g_pers_proj_info()->zFar = 30.0f;

  // AABBTree
  s->CreateAABBTree();

  // s->Dump();

  return s;
}

void Scene::OnKeyboard(int key) {
  if (key == OGLDEV_KEY_d) {
    display_collider_ = !display_collider_;
  }
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

void Scene::AddCube(Object *c) {
  cubes_.push_back(c);
}

void Scene::Physics(void) {
  unsigned int i;
  TCube *c;
  for (i = 0; i < cubes_.size(); i++) {
    c = (TCube *)cubes_[i]->g();
    c->set_collided(cubes_[i]->b()->Collide(player_));
  }
}

void Scene::Render(void) {
  unsigned int i;
  TCube *tc;
  CCube *cc;
  AABB *b;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_.OnRender();
  player_->Update();

  // Camera and perspective
  p_.SetCamera(camera_);
  p_.SetPerspectiveProj(g_pers_proj_info_);
  glUniformMatrix4fv(shader_tcube_.view(), 1, GL_TRUE,
      (const GLfloat*)p_.GetViewTrans());
  glUniformMatrix4fv(shader_tcube_.proj(), 1, GL_TRUE,
      (const GLfloat*)p_.GetOrthoProjTrans());

  // Cubes
  shader_tcube_.Use();
  // Cube drawing optimization
  TCube::DrawPre();
  if (!display_collider_) {
    for (i = 0; i < cubes_.size(); i++) {
      tc = (TCube *)cubes_[i]->g();
      b = cubes_[i]->b();
      p_.WorldPos(1.0f * b->x(), 1.0f * b->y(), 1.0f * b->z());
      if (tc->collided()) {
        p_.Scale(1.f, 2.f, 1.f);
      } else {
        p_.Scale(1.f, 1.f, 1.f);
      }
      p_.Rotate(0.f, 0.f, 0.0f);
      glUniformMatrix4fv(shader_tcube_.world(), 1, GL_TRUE,
          (const GLfloat*)p_.GetWorldTrans());
      tc->Draw();
    }
  }

  // Player
  p_.WorldPos(1.0f * player_->x(), 1.0f * player_->y(),
      1.0f * player_->z());
  p_.Rotate(0.f, player_->f(), 0.0f);
  p_.Scale(1.f, 1.f, 1.f);
  glUniformMatrix4fv(shader_tcube_.world(), 1, GL_TRUE,
      (const GLfloat*)p_.GetWorldTrans());
  player_->Draw();
  TCube::DrawPost();

  // AABBTree debug
  if (display_collider_) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniformMatrix4fv(shader_ccube_.view(), 1, GL_TRUE,
        (const GLfloat*)p_.GetViewTrans());
    glUniformMatrix4fv(shader_ccube_.proj(), 1, GL_TRUE,
        (const GLfloat*)p_.GetOrthoProjTrans());
    shader_ccube_.Use();
    CCube::DrawPre();
    for (i = 0; i < collider_->boxes().size(); i++) {
      cc = (CCube *)collider_->boxes()[i]->g();
      b = collider_->boxes()[i]->b();
      glUniform4fv(shader_ccube_.color(), 1, (const GLfloat*)cc->color());
      p_.WorldPos(b->x() + (b->w() / 4) * 2 - 0.5, b->y() + (b->h() / 4), b->z() +
          (b->d() / 4) * 2 - 0.5);
      p_.Scale(b->w(), b->h(), b->d());
      p_.Rotate(0.f, 0.f, 0.0f);
      glUniformMatrix4fv(shader_ccube_.world(), 1, GL_TRUE,
          (const GLfloat*)p_.GetWorldTrans());
      cc->Draw();
    }
    CCube::DrawPost();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  glutSwapBuffers();
}

void Scene::Dump(void) {
  unsigned int i;
  AABB *b;
  for (i = 0; i < cubes_.size(); i++) {
    b = cubes_[i]->b();
    b->Dump();
  }
}

void Scene::OnJoystickAxis(Vector2f l, Vector2f r) {
  l_ = l;
  r_ = r;
  camera_.OnJoystick(l_, r_);
  player_->OnJoystick(l_, r_);
}
