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

#include "scene.h"
#include "keys.h"
#include "textures.h"
#include "gamepad.h"

OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key)
{
    switch (Key) {
		case GLUT_KEY_F1:
			return OGLDEV_KEY_F1;
		case GLUT_KEY_F2:
			return OGLDEV_KEY_F2;
		case GLUT_KEY_F3:
			return OGLDEV_KEY_F3;
		case GLUT_KEY_F4:
			return OGLDEV_KEY_F4;
		case GLUT_KEY_F5:
			return OGLDEV_KEY_F5;
		case GLUT_KEY_F6:
			return OGLDEV_KEY_F6;
		case GLUT_KEY_F7:
			return OGLDEV_KEY_F7;
		case GLUT_KEY_F8:
			return OGLDEV_KEY_F8;
		case GLUT_KEY_F9:
			return OGLDEV_KEY_F9;
		case GLUT_KEY_F10:
			return OGLDEV_KEY_F10;
		case GLUT_KEY_F11:
			return OGLDEV_KEY_F11;
		case GLUT_KEY_F12:
			return OGLDEV_KEY_F12;
		case GLUT_KEY_LEFT:
			return OGLDEV_KEY_LEFT;
		case GLUT_KEY_UP:
			return OGLDEV_KEY_UP;
		case GLUT_KEY_RIGHT:
			return OGLDEV_KEY_RIGHT;
		case GLUT_KEY_DOWN:
			return OGLDEV_KEY_DOWN;
		case GLUT_KEY_PAGE_UP:
			return OGLDEV_KEY_PAGE_UP;
		case GLUT_KEY_PAGE_DOWN:
			return OGLDEV_KEY_PAGE_DOWN;
		case GLUT_KEY_HOME:
			return OGLDEV_KEY_HOME;
		case GLUT_KEY_END:
			return OGLDEV_KEY_END;	
		case GLUT_KEY_INSERT:
            return OGLDEV_KEY_INSERT;
        case GLUT_KEY_DELETE:
            return OGLDEV_KEY_DELETE;
        default:
            printf("Unimplemented GLUT key\n");
            // exit(1);
    }

    return OGLDEV_KEY_UNDEFINED;
}

Scene *s;
Gamepad *g = NULL;

static void RenderSceneCB(void) {
  if (g != NULL) {
    g->Update();
    int event = g->event();
    g->Clear();
    if (event == JS_EVENT_AXIS) {
      // g->Dump();
      s->OnJoystickAxis(g->l(), g->r());
    }
  }
  s->Physics();
  s->Render();
}

#define ABS(x) ((x < 0) ? -x : x)

#define KEYBOARD_ONE_NORM 0.7071067811865476f

static void norm_to_one(Vector2f *v) {
  if (ABS(v->x) == 1.0f && ABS(v->y) == 1.0f) {
    v->x = KEYBOARD_ONE_NORM * v->x;
    v->y = KEYBOARD_ONE_NORM * v->y;
  } else if (v->x == 0.f && ABS(v->y) == KEYBOARD_ONE_NORM) {
    v->y /= v->y;
  } else if (v->y == 0.f && ABS(v->x) == KEYBOARD_ONE_NORM) {
    v->x /= v->x;
  }
}

// XXX no globals
static float kb_h = 0.f;
static float kb_v = 0.f;

static void SpecialKeyboardCB(int Key, int x, int y) {
  switch (GLUTKeyToOGLDEVKey(Key)) {
    case OGLDEV_KEY_LEFT:
      kb_h = 1.f;
      break;
    case OGLDEV_KEY_RIGHT:
      kb_h = -1.f;
      break;
    case OGLDEV_KEY_UP:
      kb_v = 1.f;
      break;
    case OGLDEV_KEY_DOWN:
      kb_v = -1.f;
      break;
    default:
      ;
  }
  Vector2f l(kb_h, kb_v);
  norm_to_one(&l);
  Vector2f r(0., 0.);
  s->OnJoystickAxis(l, r);
}

static void SpecialKeyboardUpCB(int Key, int x, int y) {
  switch (GLUTKeyToOGLDEVKey(Key)) {
    case OGLDEV_KEY_LEFT:
      kb_h = 0.f;
      break;
    case OGLDEV_KEY_RIGHT:
      kb_h = 0.f;
      break;
    case OGLDEV_KEY_UP:
      kb_v = 0.f;
      break;
    case OGLDEV_KEY_DOWN:
      kb_v = 0.f;
      break;
    default:
      ;
  }
  Vector2f l(kb_h, kb_v);
  norm_to_one(&l);
  Vector2f r(0., 0.);
  s->OnJoystickAxis(l, r);
}

static void KeyboardCB(unsigned char Key, int x, int y) {
  s->OnKeyboard(Key);
}

static void ReshapeCB(int width, int height) {
  s->OnReshape(width, height);
  glViewport(0, 0, width, height);
  glutPostRedisplay();
}

static void PassiveMouseCB(int x, int y) {
  s->OnMouse(x, y);
}

int main(int argc, char *argv[]) {

  // XXX Scene creation

  // Glut initialization
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH/*|GLUT_MULTISAMPLE*/);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  // glutGameModeString("1014x768@32");
  // glutEnterGameMode();
  glutInitWindowPosition(40, 21);
  glutCreateWindow("Landstalker 2");

  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutSpecialUpFunc(SpecialKeyboardUpCB);
  glutKeyboardFunc(KeyboardCB);
  glutReshapeFunc(ReshapeCB);
  glutPassiveMotionFunc(PassiveMouseCB);

  // Antialiasing
  // glEnable(GL_MULTISAMPLE);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  // Gamepad object
  try {
    g = new Gamepad("/dev/input/js1");
  } catch (GamepadException const &e) {
    printf("Disabling gamepad\n");
    delete g;
    g = NULL;
  }

  // Create a test scene
  s = Scene::CreateTestScene();

  glutMainLoop();

  delete s;
  delete g;

  return 0;
}
