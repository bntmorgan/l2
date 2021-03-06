/*

   Copyright 2010 Etay Meiri
   Copyright 2015 Benoît Morgan

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

#ifndef CAMERA_H
#define	CAMERA_H

#include "math_3d.h"
#include "keys.h"

class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const
        Vector3f& Target, const Vector3f& Up);
    bool OnKeyboard(OGLDEV_KEY Key);
    void OnMouse(int x, int y);
    void OnRender();
    void OnJoystick(Vector2f l, Vector2f r);
    void SetWindow(int WindowWidth, int WindowHeight);
    const Vector3f& GetPos() const {
      return m_pos;
    }
    const Vector3f& GetTarget() const {
      return m_target;
    }
    const Vector3f& GetUp() const {
      return m_up;
    }
    float GetAngleH(void) { return m_AngleH; }
    void set_pos(Vector3f *pos) { m_pos = *pos; }
private:

    void Init();
    void Update();

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    Vector2i m_mousePos;

    // Rotation speeds
    float m_rh;
    float m_rv;
    // Position speeds
    Vector3f m_pm;
};

#endif	/* CAMERA_H */

