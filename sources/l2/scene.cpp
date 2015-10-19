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

const char* pVSFileName = "sources/l2/shader.vs";
const char* pFSFileName = "sources/l2/shader.fs";

Scene::Scene(void) : camera_(Camera(WINDOW_WIDTH, WINDOW_HEIGHT)) {
  // Textures
  textures_ = new Textures;
  // Player
  player_ = new Player(0,  1,  0, textures_->texture("PLAYER"), &camera_);
  Cube::InitGL();
}

Scene::~Scene(void) {
  Cube::DestroyGL();
}

bool ReadFile(const char* pFileName, std::string& outFile) {
  std::ifstream f(pFileName);

  bool ret = false;

  if (f.is_open()) {
    std::string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }

    f.close();

    ret = true;
  } else {
    printf("Failed to open file : %s\n", pFileName);
  }

  return ret;
}

void Scene::AddShader(GLuint ShaderProgram, const char* pShaderText,
    GLenum ShaderType) {
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = pShaderText;
  GLint Lengths[1];
  Lengths[0]= strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType,
        InfoLog);
    exit(1);
  }

  glAttachShader(ShaderProgram, ShaderObj);
}

void Scene::CompileShaders() {
  GLuint ShaderProgram = glCreateProgram();

  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }

  std::string vs, fs;

  if (!ReadFile(pVSFileName, vs)) {
    exit(1);
  };

  if (!ReadFile(pFSFileName, fs)) {
    exit(1);
  };

  AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
  AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  // GLSL 130 attribute location
  glBindAttribLocation(ShaderProgram, 0, "Position");
  glBindAttribLocation(ShaderProgram, 1, "TexCoord");

  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glUseProgram(ShaderProgram);

  // World + view transformation 4x4 matrix
  g_world_location_ = glGetUniformLocation(ShaderProgram, "gWVP");
  assert(g_world_location_ != 0xFFFFFFFF);
  g_sampler_ = glGetUniformLocation(ShaderProgram, "gSampler");
  assert(g_sampler_ != 0xFFFFFFFF);

  glUniform1i(g_sampler_, 0);
}

/**
 * XXX Scene test function
 */
Scene *Scene::CreateTestScene(void) {
  Scene *s = new Scene;

  // Shaders
  s->CompileShaders();

  Textures *ts = s->textures();

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
  s->AddCube(new Cube( 2,  0, -1, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
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
  s->AddCube(new Cube( 2,  0,  2, ts->texture("DIRT_SIDE"), ts->texture("DIRT_TOP")));
  s->AddCube(new Cube( 3,  0,  2, ts->texture("WATER_TOP")));
  s->AddCube(new Cube( 3, -1,  2, ts->texture("WOOD_SIDE"), ts->texture("WOOD_TOP")));

  // Perspective
  s->g_pers_proj_info()->FOV = 0.0f;
  s->g_pers_proj_info()->Height = CUBE_HEIGHT;
  s->g_pers_proj_info()->Width = CUBE_WIDTH;
  s->g_pers_proj_info()->zNear = -100.0f;
  s->g_pers_proj_info()->zFar = 100.0f;

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

void Scene::Render(void) {
  unsigned int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_.OnRender();
  player_->Update();
  for (i = 0; i < cubes_.size(); i++) {
		p_.WorldPos(1.0f * cubes_[i]->x(), 1.0f * cubes_[i]->y(),
        1.0f * cubes_[i]->z());
    p_.Rotate(0.f, 0.f, 0.0f);
    p_.SetCamera(camera_);
    p_.SetPerspectiveProj(g_pers_proj_info_);
    glUniformMatrix4fv(g_world_location_, 1, GL_TRUE,
        (const GLfloat*)p_.GetWVOrthoPTrans());
    cubes_[i]->Render();
  }

  // XXX Draw player
  p_.WorldPos(1.0f * player_->rx(), 1.0f * player_->ry(),
      1.0f * player_->rz());
  p_.Rotate(0.f, player_->f(), 0.0f);
  p_.SetCamera(camera_);
  p_.SetPerspectiveProj(g_pers_proj_info_);
  glUniformMatrix4fv(g_world_location_, 1, GL_TRUE,
      (const GLfloat*)p_.GetWVOrthoPTrans());
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
