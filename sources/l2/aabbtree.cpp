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

#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <stack>

#include "aabbtree.h"
#include "aabb.h"
#include "ccube.h"

static AABB Encompassing(const std::vector<Object*> &e) {
  AABB *t;
  int i;
  float min_x = FLT_MAX, max_x = -FLT_MAX,
      min_y = FLT_MAX, max_y = -FLT_MAX,
      min_z = FLT_MAX, max_z = -FLT_MAX;
  for (i = 0; i < e.size(); i++) {
    t = e[i]->b();
    // X
    if (t->x() < min_x) {
      min_x = t->x();
    }
    if (t->x() + t->w() > max_x) {
      max_x = t->x() + t->w();
    }
    // Y
    if (t->y() < min_y) {
      min_y = t->y();
    }
    if (t->y() + t->h() > max_y) {
      max_y = t->y() + t->h();
    }
    // Z
    if (t->z() < min_z) {
      min_z = t->z();
    }
    if (t->z() + t->d() > max_z) {
      max_z = t->z() + t->d();
    }
  }
  AABB r(min_x, min_y, min_z, max_x - min_x, max_y - min_y, max_z - min_z);
  return r;
}

AABBCell *AABBTree::Pass(const std::vector<Object*> &e, AABBCell *p,
    int depth){
  if (e.size() == 0) {
    return NULL;
  }
  AABBCell *c = new AABBCell;
  AABB eb = Encompassing(e), *cb;
  c->parent = p;
  c->e_box = eb;
  c->boxes = e;
  // Add it into the debug boxes
  Vector4f color(1.f - (0.05f * depth), 0.f, 0.05f * depth, 0.5f);
  boxes_.push_back(new Object(&c->e_box, new CCube(color)));
  // XXX Maybe modify the stop condition
  if (e.size() == 1 || depth == max_depth_ || (eb.w() <= 1.f && eb.h() <= 1.f &&
        eb.d() <= 1.f)) {
    return c;
  }
  int ld = eb.LargestDim();
  std::vector<Object*> l_child, r_child;
  int i;
  float t;
  switch (ld) {
    case AXIS_X:
      t = eb.x() + eb.w() / 2;
      break;
    case AXIS_Y:
      t = eb.y() + eb.h() / 2;
      break;
    case AXIS_Z:
      t = eb.z() + eb.d() / 2;
      break;
    default:
      printf("Error dude axis %d doesn't exists\n", ld);
      exit(1);
  }
  // Compute left and right child
  for (i = 0; i < e.size(); i++) {
    cb = e[i]->b();
    switch (ld) {
      case AXIS_X:
        if (cb->x() + cb->w() <= t) {
          l_child.push_back(e[i]);
        } else {
          r_child.push_back(e[i]);
        }
        break;
      case AXIS_Y:
        if (cb->y() + cb->h() <= t) {
          l_child.push_back(e[i]);
        } else {
          r_child.push_back(e[i]);
        }
        break;
      case AXIS_Z:
        if (cb->z() + cb->d() <= t) {
          l_child.push_back(e[i]);
        } else {
          r_child.push_back(e[i]);
        }
        break;
      default:
        printf("Error dude axis %d doesn't exists\n", ld);
        exit(1);
    }
  }
  // Recursive call
  c->l_child = Pass(l_child, c, depth + 1);
  c->r_child = Pass(r_child, c, depth + 1);
  return c;
}

AABBTree::AABBTree(const std::vector<Object*> &e, unsigned int max_depth) :
    max_depth_(max_depth) {
  int i;
  // We copy the vector of objects
  for (i = 0; i < e.size(); i++) {
    world_.push_back(e[i]);
  }
  root_ = Pass(world_, NULL, 0);
}

static void Destroy(AABBCell *c) {
  if (c == NULL) {
    return;
  }
  Destroy(c->l_child);
  Destroy(c->r_child);
  delete c;
}

AABBTree::~AABBTree(void) {
  Destroy(root_);
}

void AABBTree::Search(std::vector<Object*> *matches, AABB *e) {
  AABBCell *c;
  int i;
  std::stack<struct AABBCell*> dfs;
  dfs.push(root_);
  // Empty the last boxex_search
  for (i = 0; i < boxes_search_.size(); i++) {
    delete boxes_search_[i]->g();
    delete boxes_search_[i];
  }
  boxes_search_.clear();
  while (dfs.size() > 0) { // If zero there is no more children to process
    c = dfs.top();
    dfs.pop();
    if (!c->e_box.Collide(e)) {
      continue;
    }
    Vector4f color(0.5f, 0.5f, 0.05f, 0.5f);
    boxes_search_.push_back(new Object(&c->e_box, new CCube(color)));
    if (c->l_child != NULL) {
      dfs.push(c->l_child);
    }
    if (c->r_child != NULL) {
      dfs.push(c->r_child);
    }
    if (c->l_child != NULL && c->r_child != NULL) {
      // Add it the results this is a leaf
      for (i = 0; i < c->boxes.size(); i++) {
        matches->push_back(c->boxes[i]);
      }
    }
  }
}
