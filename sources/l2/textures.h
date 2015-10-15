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

#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <vector>
#include <map>

#include "texture.h"
#include "string.h"

#define TEXTURES_DEFAULT_LOCATION "./ressources"

struct cmp_str {
  bool operator()(char const *a, char const *b) {
    return strcmp(a, b) < 0;
  }
};

class Textures {
  private:
    std::vector<Texture*> textures_;
    std::map<const char*, Texture*, cmp_str> textures_map_;
  public:
    Textures(void);
		void LoadTxtFile(const char *file);
    Texture *texture(const char *name) {
      return textures_map_[name];
    }
};

#endif//__TEXTURES_H__
