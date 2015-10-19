/*

   Copyright 2010 Etay Meiri

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   */

#include "camera.h"
#include "keys.h"

const static float STEP_SCALE = 1.0f;
// const static float EDGE_STEP = 1.0f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight) {
  m_windowWidth  = WindowWidth;
  m_windowHeight = WindowHeight;
  m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
  m_target       = Vector3f(0.f, 0.f, 1.0f);
  // m_target       = Vector3f(-0.484841, -0.500066, 0.717540);
  m_target.Normalize();
  m_up           = Vector3f(0.0f, 1.0f, 0.0f);
  // Rotations au montion
  m_rh = 0;
  m_rv = 0;
  // Position speeds
  m_pm = Vector3f(0.f, 0.f, 0.f);

  Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const
    Vector3f& Target, const Vector3f& Up) {
  m_windowWidth  = WindowWidth;
  m_windowHeight = WindowHeight;
  m_pos = Pos;

  m_target = Target;
  m_target.Normalize();

  m_up = Up;
  m_up.Normalize();

  Init();
}

void Camera::Init() {
  Vector3f HTarget(m_target.x, 0.0, m_target.z);
  HTarget.Normalize();

  if (HTarget.z >= 0.0f) {
    if (HTarget.x >= 0.0f) {
      m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
    } else {
      m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
    }
  } else {
    if (HTarget.x >= 0.0f) {
      m_AngleH = ToDegree(asin(-HTarget.z));
    } else {
      m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
    }
  }

  m_AngleV = -ToDegree(asin(m_target.y));

  m_OnUpperEdge = false;
  m_OnLowerEdge = false;
  m_OnLeftEdge  = false;
  m_OnRightEdge = false;
  m_mousePos.x  = m_windowWidth / 2;
  m_mousePos.y  = m_windowHeight / 2;

  // glutWarpPointer(m_mousePos.x, m_mousePos.y);
}


void Camera::SetWindow(int WindowWidth, int WindowHeight) {
  m_windowWidth  = WindowWidth;
  m_windowHeight = WindowHeight;
}

bool Camera::OnKeyboard(OGLDEV_KEY Key) {
  bool Ret = false;

  switch (Key) {

    case OGLDEV_KEY_UP:
      m_pos.y += STEP_SCALE;
      break;

    case OGLDEV_KEY_DOWN:
      m_pos.y -= STEP_SCALE;
      break;

    case OGLDEV_KEY_LEFT: {
      Vector3f Left = m_target.Cross(m_up);
      Left.Normalize();
      Left *= STEP_SCALE;
      m_pos += Left;
      Ret = true;
      break;
    }

    case OGLDEV_KEY_RIGHT: {
      Vector3f Right = m_up.Cross(m_target);
      Right.Normalize();
      Right *= STEP_SCALE;
      m_pos += Right;
      Ret = true;
      break;
    }

    default:
      break;
  }

//  printf("pos(%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

  return Ret;
}


void Camera::OnMouse(int x, int y) {
  const int DeltaX = x - m_mousePos.x;
  const int DeltaY = y - m_mousePos.y;

  m_mousePos.x = x;
  m_mousePos.y = y;

  m_AngleH += (float)DeltaX / 20.0f;
  m_AngleV += (float)DeltaY / 20.0f;

  if (DeltaX == 0) {
    if (x <= MARGIN) {
      //    m_AngleH -= 1.0f;
      m_OnLeftEdge = true;
    } else if (x >= (m_windowWidth - MARGIN)) {
      //    m_AngleH += 1.0f;
      m_OnRightEdge = true;
    }
  } else {
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
  }

  if (DeltaY == 0) {
    if (y <= MARGIN) {
      m_OnUpperEdge = true;
    }
    else if (y >= (m_windowHeight - MARGIN)) {
      m_OnLowerEdge = true;
    }
  } else {
    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
  }

  Update();
}


void Camera::OnRender() {
// bool ShouldUpdate = false;

//  if (m_OnLeftEdge) {
//    m_AngleH -= EDGE_STEP;
//    ShouldUpdate = true;
//  } else if (m_OnRightEdge) {
//    m_AngleH += EDGE_STEP;
//    ShouldUpdate = true;
//  }
//
//  if (m_OnUpperEdge) {
//    if (m_AngleV > -90.0f) {
//      m_AngleV -= EDGE_STEP;
//      ShouldUpdate = true;
//    }
//  } else if (m_OnLowerEdge) {
//    if (m_AngleV < 90.0f) {
//      m_AngleV += EDGE_STEP;
//      ShouldUpdate = true;
//    }
//  }
//
//  if (ShouldUpdate) {
    Update();
//  }
}

void Camera::Update() {
  const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

  // Rotate the view vector by the horizontal angle around the vertical axis
  Vector3f View(1.5f, 0.0f, 0.0f);
  // Add the current Joystick value to Angle
  m_AngleH += 1.5 * m_rh;
  View.Rotate(m_AngleH, Vaxis);
  View.Normalize();

  // Rotate the view vector by the vertical angle around the horizontal axis
  Vector3f Haxis = Vaxis.Cross(View);
  Haxis.Normalize();
  // Add the current Joystick value to Angle
  m_AngleV += 1.5 * m_rv;
  View.Rotate(m_AngleV, Haxis);

  m_target = View;
  m_target.Normalize();

  m_up = m_target.Cross(Haxis);
  m_up.Normalize();

  // Position move
  Vector3f pm = m_pm;
  // printf("pos (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
  // XXX ? WTF
  pm.Rotate(m_AngleH + 90, Vaxis);
  m_pos.x += pm.x / 10;
  m_pos.z += pm.z / 10;
}

void Camera::OnJoystick(Vector2f l, Vector2f r) {
  m_rh = r.x;
  m_rv = r.y;
  m_pm.x = -l.x;
  m_pm.y = 1.;
  m_pm.z = l.y;
  // printf("l(%f, %f), r(%f, %f)\n", l.x, l.y, r.x, r.y);
}
