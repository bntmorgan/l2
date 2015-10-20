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

#include "gamepad.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

Gamepad::Gamepad(std::string f) {
  // Init ourselves
  memset(this, 0, sizeof(Gamepad));
  // Open the xbox gamepad
  fd_ = open(f.c_str(),  O_RDONLY | O_NONBLOCK);
  if (fd_ == -1) {
    perror("Error while openning the gamepad device");
    exit(-1);
  }
}

Gamepad::~Gamepad(void) {
  close(fd_);
}

void Gamepad::HandleButton(void) {
  // PrintEvent(&e_);
  event_ |= JS_EVENT_BUTTON;
  switch (e_.number) {
    case GAMEPAD_BUTTON_A:
      a_ = e_.value;
      break;
    case GAMEPAD_BUTTON_X:
      x_ = e_.value;
      break;
    case GAMEPAD_BUTTON_B:
      b_ = e_.value;
      break;
    case GAMEPAD_BUTTON_Y:
      y_ = e_.value;
      break;
    case GAMEPAD_BUTTON_LB:
      lb_ = e_.value;
      break;
    case GAMEPAD_BUTTON_RB:
      rb_ = e_.value;
      break;
    case GAMEPAD_BUTTON_BACK:
      back_ = e_.value;
      break;
    case GAMEPAD_BUTTON_START:
      start_ = e_.value;
      break;
    case GAMEPAD_BUTTON_XBOX:
      xbox_ = e_.value;
      break;
    case GAMEPAD_BUTTON_L:
      bl_ = e_.value;
      break;
    case GAMEPAD_BUTTON_R:
      br_ = e_.value;
      break;
    default:
      printf("unknown button number %d\n", e_.number);
      // Cancel the event
      event_ &= ~(int)JS_EVENT_BUTTON;
  }
}

#define DEAD(x) \
  ((x) < GAMEPAD_AXIS_DEAD_ZONE && (x) > -GAMEPAD_AXIS_DEAD_ZONE)

#define ZERO_IF_DEAD(x) \
    ((DEAD(x)) ? \
    x / GAMEPAD_AXIS_MAX : \
    0.)

// Joysticks deadzones work by pair
void Gamepad::AdjustAxis(void) {
  if (DEAD(lr_.x) && DEAD(lr_.y)) {
    l_.x = 0.;
    l_.y = 0.;
  } else {
    l_.x = lr_.x / GAMEPAD_AXIS_MAX;
    l_.y = lr_.y / GAMEPAD_AXIS_MAX;
  }
  if (DEAD(rr_.x) && DEAD(rr_.y)) {
    r_.x = 0.;
    r_.y = 0.;
  } else {
    r_.x = rr_.x / GAMEPAD_AXIS_MAX;
    r_.y = rr_.y / GAMEPAD_AXIS_MAX;
  }
}

void Gamepad::HandleAxis(void) {
  float v = 0., lv = 0.;
  // Reverse the value, up is positive !!
  v = e_.value * -1;
  switch (e_.number) {
    case GAMEPAD_AXIS_L_X:
      lr_.x  = v;
      break;
    case GAMEPAD_AXIS_L_Y:
      lr_.y  = v;
      break;
    case GAMEPAD_AXIS_LT:
      lt_  = ZERO_IF_DEAD(v);
      break;
    case GAMEPAD_AXIS_R_X:
      rr_.x = v;
      break;
    case GAMEPAD_AXIS_R_Y:
      rr_.y = v;
      break;
    case GAMEPAD_AXIS_RT:
      rt_  = ZERO_IF_DEAD(v);
      break;
    case GAMEPAD_AXIS_CROSS_X:
      cross_.x  = ZERO_IF_DEAD(v);
      break;
    case GAMEPAD_AXIS_CROSS_Y:
      cross_.y  = ZERO_IF_DEAD(v);
      break;
    default:
      printf("unknown axis number %d\n", e_.number);
  }
  if (lv != 0 || v != 0) {
    // PrintEvent(&e_);
  }
  event_ |= JS_EVENT_AXIS;
}

void Gamepad::Update(void) {
  ssize_t r = 1;
  // Read every queued events available now
  while (r > 0) {
    r = read(fd_, &e_, sizeof(struct js_event));
    if (r == -1) {
      if (errno != EAGAIN) {
        perror("Error while reading the joystick device");
        exit(-1);
      }
    } else if (r > 0) {
      // Handle the event
      switch (e_.type) {
        case JS_EVENT_BUTTON:
          HandleButton();
          break;
        case JS_EVENT_AXIS:
          HandleAxis();
          break;
        case JS_EVENT_INIT:
          break;
        default:
          // printf("unknown event type\n");
          ;
      }
    }
  }
  // Adjust Axis
  AdjustAxis();
}

void Gamepad::Clear(void) {
  event_ = 0;
}

void Gamepad::Dump(void) {
  printf("l(%.2f, %.2f), r(%.2f, %.2f), lt(%.2f), rt(%.2f), x(%.2f, %.2f)\n",
      l_.x, l_.y, r_.x, r_.y, lt_, rt_, cross_.x, cross_.y);
  printf("a(%d), b(%d), x(%d), y(%d), lb(%d), rb(%d), back(%d), start(%d), \
xbox(%d), bl(%d), br(%d)\n",
      a_, b_, x_, y_, lb_, rb_, back_, start_, xbox_, bl_, br_);
}

void Gamepad::PrintEvent(struct js_event *e) {
  printf("time: %d, value: %d, type: %d, number: %d\n", e->time, e->value,
      e->type, e->number);
}
