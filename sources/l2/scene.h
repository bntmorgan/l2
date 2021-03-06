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

#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "object.h"
#include "player.h"
#include "pipeline.h"
#include "math_3d.h"
#include "camera.h"
#include "textures.h"
#include "aabbtree.h"
#include "shader.h"
#include "shader_tcube.h"
#include "shader_ccube.h"
#include "gamepad.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define CUBE_WIDTH 20
#define CUBE_HEIGHT ((CUBE_WIDTH) / ((float)WINDOW_WIDTH / WINDOW_HEIGHT))

class Cube;

class Scene {
 private:
  bool display_collider_;
  bool display_collider_search_;
  std::vector<Object*> cubes_;
  Textures *textures_;
  // Physics
  AABBTree *collider_;
  // Math
  Pipeline p_;
  PersProjInfo g_pers_proj_info_;
  Camera camera_;
  // Joystick
  Vector2f l_;
  Vector2f r_;
  // Player
  Player *player_;
  Object *move_box_;
  // XXX Motion beforce collision
  Vector3f original_pm_;
  // Opengl
  ShaderTCube shader_tcube_;
  ShaderCCube shader_ccube_;
 public:
  Scene(void);
  ~Scene(void);
  static Scene *CreateTestScene(void);
  void CompileShaders();
  void AddShader(GLuint ShaderProgram, const char* pShaderText,
      GLenum ShaderType);
  void AddCube(Object *c);
  void Render(void);
  void Physics(void);
  void Dump(void);
  void OnKeyboard(int key);
  void OnMouse(int x, int y);
  void OnReshape(int x, int y);
  void OnJoystickAxis(Gamepad *g, Vector2f l, Vector2f r);
  void OnJoystickButton(Gamepad *g);
  void CreateAABBTree(void);
  // Accessors
  PersProjInfo *g_pers_proj_info(void) { return &g_pers_proj_info_; }
  Camera p_game_camera(void) { return camera_; }
  void set_textures(Textures *textures) {
    textures_ = textures;
  }
  Textures *textures(void) {
    return textures_;
  }
};

#endif//__SCENE_H__
