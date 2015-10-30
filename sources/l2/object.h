#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "aabb.h"
#include "graphic.h"

class Object {
 private:
  AABB *b_;
  Graphic *g_;
 public:
  Object(AABB *b, Graphic *g) : b_(b), g_(g) { }
  AABB *b(void) { return b_; }
  Graphic *g(void) { return g_; }
  void set_b(AABB *b) { b_ = b; }
  void set_g(Graphic *g) { g_ = g; }
};

#endif//__OBJECT_H__
