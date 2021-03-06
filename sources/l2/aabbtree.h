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

#ifndef __AABBTREE_H__
#define __AABBTREE_H__

#include <vector>
#include "aabb.h"
#include "object.h"

struct AABBCell {
  struct AABBCell *l_child; // Children
  struct AABBCell *r_child;
  struct AABBCell *parent; // Parent
  AABB e_box; // Encompassing bounding box
  std::vector<Object*> boxes; // Boxes handled by this node

  AABBCell(void) : l_child(NULL), r_child(NULL), parent(NULL) { }
};

class AABBTree {
 public:
  AABBTree(const std::vector<Object *> &e, unsigned int max_depth);
  ~AABBTree(void);
  const std::vector<Object*> boxes(void) { return boxes_; }
  const std::vector<Object*> boxes_search(void) { return boxes_search_; }
  const std::vector<Object*> boxes_result(void) { return boxes_result_; }
  void Search(std::vector<Object*> *matches, AABB *e);
 private:
  std::vector<Object*> world_; // Objects handled by the tree
  std::vector<Object*> boxes_; // Graphical display of the tree
  std::vector<Object*> boxes_search_; // Graphical display of the search
  std::vector<Object*> boxes_result_; // Graphical display of the search
  unsigned int max_depth_;
  struct AABBCell *root_;
  AABBCell *Pass(const std::vector<Object *> &e, AABBCell *p, int depth);
};

#endif//__AABBTREE_H__
