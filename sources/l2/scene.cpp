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
#include <cmath>
#include <float.h>
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
  player_ = new Player(0,  5,  0, textures_->texture("PLAYER"), &camera_);
  Graphic::InitGL();
  display_collider_ = false;
  display_collider_search_ = false;
  Vector4f color(1.f, 0, 0, 0.5f);
  move_box_ = new Object(new AABB(), new CCube(color));
}

Scene::~Scene(void) {
  Graphic::DestroyGL();
  delete move_box_->g();
  delete move_box_->b();
  delete move_box_;
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
#define CUBES_NB 6
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
      if (i > -CUBES_NB && i < CUBES_NB && j > -CUBES_NB && j < CUBES_NB){
        if (i <= -CUBES_NB + 2 || i >= CUBES_NB - 2 ||
            j <= -CUBES_NB + 2 || j >= CUBES_NB - 2){
          s->AddCube(new Object(new AABB(i,  0,  j),
                new TCube(ts->texture("WATER_TOP"))));
        } else {
          s->AddCube(new Object(new AABB(i,  0,  j),
                new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
        }
      }
    }
  }

  // Stairs
  s->AddCube(new Object(new AABB(3,  1,  3),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
  s->AddCube(new Object(new AABB(1,  1,  3),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
  s->AddCube(new Object(new AABB(0,  2,  3),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
  s->AddCube(new Object(new AABB(-1,  3,  3),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
  s->AddCube(new Object(new AABB(-1,  4,  2),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));
  s->AddCube(new Object(new AABB(-1,  5,  1),
        new TCube(ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP"))));

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
  if (key == OGLDEV_KEY_s) {
    display_collider_search_ = !display_collider_search_;
  }
  if (key == OGLDEV_KEY_SPACE) {
    player_->Jump();
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

Vector3f Min3(float x, float y, float z) {
  float ax = std::abs(x), ay = std::abs(y), az = std::abs(z);
  if (ax < ay && ax < az) {
    return Vector3f(x, 0, 0);
  } else if (ay < ax && ay < az) {
    return Vector3f(0, y, 0);
  }
  return Vector3f(0, 0, z);
}

void Scene::Physics(void) {
  static std::vector<Object*> res;
  bool swap = true;
  static bool tg = false;
  Object *t;
  int i;
  AABB *cb, *cb1; // collided box
  res.clear();
  // XXX : Recover the motion given by the joystick
  player_->set_pm(original_pm_);
  // Create AABB Player movement box
  Vector3f np = player_->NextPosition();
  AABB *eb = move_box_->b();
  AABB npb(np.x, np.y, np.z, player_->w(), player_->h(),
      player_->d());
  player_->Encompassing(eb, &npb);
  collider_->Search(&res, eb);
  // Sort the collisions by increasing distance closer collision first will
  // maybe delete farther collision
  if (res.size() > 0) {
    // XXX Other stuff than bubble sort
    while (swap) {
      swap = false;
      for (i = 0; i < res.size() - 1; i++) {
        cb = res[i]->b();
        cb1 = res[i + 1]->b();
        if (cb->Distance(player_) > cb1->Distance(player_)) {
          swap = true;
          t = res[i + 1];
          res[i + 1] = res[i];
          res[i] = t;
        }
      }
    }
  }
  // XXX Replace this by Ray tracing
  tg = false;
  for (i = 0; i < res.size(); i++) {
    cb = res[i]->b();
    if (npb.Collide(cb)) {
      // Computing the new speed vector from the collided boxes
      float delta_x = (cb->x() + cb->w()) - (npb.x() + npb.w());
      delta_x = (delta_x > 0) ? delta_x - cb->w() : delta_x + cb->w();
      float delta_y = (cb->y() + cb->h()) - (npb.y() + npb.h());
      delta_y = (delta_y > 0) ? delta_y - cb->h() : delta_y + cb->h();
      float delta_z = (cb->z() + cb->d()) - (npb.z() + npb.d());
      delta_z = (delta_z > 0) ? delta_z - cb->d() : delta_z + cb->d();
      Vector3f d = Min3(delta_x, delta_y, delta_z);
      if (std::abs(d.y) > 0.0) {
        tg = true;
      }
      player_->set_pm(player_->pm() + d);
      // Update motion vector and new box position for the next collided box
      np = player_->NextPosition();
      npb.set_x(np.x);
      npb.set_y(np.y);
      npb.set_z(np.z);
    }
  }
  if (!player_->grounded() && tg) {
    player_->set_grounded(true);
  } else if (player_->grounded() && !tg) {
    player_->set_grounded(false);
  }
}

void Scene::Render(void) {
  unsigned int i;
  TCube *tc;
  CCube *cc;
  AABB *b;
  Vector3f p_pos;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  player_->Update();
  // Set the camera position : It follows the player...
  p_pos.x = player_->x();
  p_pos.y = player_->y();
  p_pos.z = player_->z();
  camera_.set_pos(&p_pos);
  camera_.OnRender();

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
      p_.WorldPos(b->glx(), b->gly(), b->glz());
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
    const std::vector<Object *> &bx = (display_collider_search_) ?
        collider_->boxes_search() : collider_->boxes();
    for (i = 0; i < bx.size(); i++) {
      cc = (CCube *)bx[i]->g();
      b = bx[i]->b();
      glUniform4fv(shader_ccube_.color(), 1, (const GLfloat*)cc->color());
      p_.WorldPos(b->glx(), b->gly(), b->glz());
      p_.Scale(b->w(), b->h(), b->d());
      p_.Rotate(0.f, 0.f, 0.0f);
      glUniformMatrix4fv(shader_ccube_.world(), 1, GL_TRUE,
          (const GLfloat*)p_.GetWorldTrans());
      cc->Draw();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Search result
    if (display_collider_search_) {
      const std::vector<Object *> &rbx = collider_->boxes_result();
      for (i = 0; i < rbx.size(); i++) {
        cc = (CCube *)rbx[i]->g();
        b = rbx[i]->b();
        glUniform4fv(shader_ccube_.color(), 1, (const GLfloat*)cc->color());
        p_.WorldPos(b->glx(), b->gly(), b->glz());
        p_.Scale(b->w(), b->h(), b->d());
        p_.Rotate(0.f, 0.f, 0.0f);
        glUniformMatrix4fv(shader_ccube_.world(), 1, GL_TRUE,
            (const GLfloat*)p_.GetWorldTrans());
        cc->Draw();
      }
      // Draw player moving Box
      cc = (CCube *)move_box_->g();
      b = move_box_->b();
      glUniform4fv(shader_ccube_.color(), 1, (const GLfloat*)cc->color());
      p_.WorldPos(b->glx(), b->gly(), b->glz());
      p_.Scale(b->w(), b->h(), b->d());
      p_.Rotate(0.f, 0.f, 0.0f);
      glUniformMatrix4fv(shader_ccube_.world(), 1, GL_TRUE,
          (const GLfloat*)p_.GetWorldTrans());
      cc->Draw();
    }
    CCube::DrawPost();

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

void Scene::OnJoystickAxis(Gamepad *g, Vector2f l, Vector2f r) {
  l_ = l;
  r_ = r;
  camera_.OnJoystick(l_, r_);
  player_->OnJoystick(l_, r_);
  original_pm_ = player_->pm();
}

void Scene::OnJoystickButton(Gamepad *g) {
  if (g->a()) {
    player_->Jump();
  }
}
