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
#include "rect.h"

struct AABBCell {
  struct AABBCell *l_child; // Children
  struct AABBCell *r_child;
  struct AABBCell *parent; // Parent
  Rect e_box; // Encompassing counding box
  std::vector<Rect *> boxes; // Boxes handled by this node
};

class AABBTree {
 public:
  AABBTree(const std::vector<Rect *> &e);
 private:
  struct AABBCell *root_;
};

#endif//__AABBTREE_H__
