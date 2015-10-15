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

static void RenderSceneCB(void) {
  s->Render();
}

static void SpecialKeyboardCB(int Key, int x, int y) {
  s->OnKeyboard(GLUTKeyToOGLDEVKey(Key));
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
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Landstalker 2");

  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutKeyboardFunc(KeyboardCB);
  glutReshapeFunc(ReshapeCB);
  // glutPassiveMotionFunc(PassiveMouseCB);

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

  // Create a test scene
  s = Scene::CreateTestScene();

  glutMainLoop();

  return 0;
}
