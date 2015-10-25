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

#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <linux/joystick.h>
#include "math_3d.h"

#define GAMEPAD_AXIS_MAX 32767.0
#define GAMEPAD_AXIS_DEAD_ZONE 6000

enum gamepad_button {
  GAMEPAD_BUTTON_A,
  GAMEPAD_BUTTON_X,
  GAMEPAD_BUTTON_B,
  GAMEPAD_BUTTON_Y,
  GAMEPAD_BUTTON_LB,
  GAMEPAD_BUTTON_RB,
  GAMEPAD_BUTTON_BACK,
  GAMEPAD_BUTTON_START,
  GAMEPAD_BUTTON_XBOX,
  GAMEPAD_BUTTON_L,
  GAMEPAD_BUTTON_R
};

enum gamepad_axis {
  GAMEPAD_AXIS_L_X,
  GAMEPAD_AXIS_L_Y,
  GAMEPAD_AXIS_LT,
  GAMEPAD_AXIS_R_X,
  GAMEPAD_AXIS_R_Y,
  GAMEPAD_AXIS_RT,
  GAMEPAD_AXIS_CROSS_X,
  GAMEPAD_AXIS_CROSS_Y
};

class Gamepad {
  private:
    // Axis
    Vector2f l_;
    Vector2f lr_;
    Vector2f r_;
    Vector2f rr_;
    float lt_;
    float rt_;
    Vector2f cross_;
    // Buttons
    int a_;
    int b_;
    int x_;
    int y_;
    int lb_;
    int rb_;
    int back_;
    int start_;
    int xbox_;
    int bl_;
    int br_;
    // System
    int fd_;
    int event_;
    struct js_event e_;
    // Functions
    void HandleButton(void);
    void HandleAxis(void);
    void AdjustAxis(void);
  public:
    Gamepad(std::string f);
    ~Gamepad(void);
    void Update(void);
    void PrintEvent(struct js_event *e);
    void Clear(void);
    void Dump(void);
    // Accessors
    int event(void) { return event_; }
    Vector2f r(void) { return r_;}
    Vector2f l(void) { return l_;}
};

#endif//__GAMEPAD_H__
