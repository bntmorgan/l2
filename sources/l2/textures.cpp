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

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

#include "textures.h"
#include "texture.h"

#include "ImageMagick-6/Magick++.h"

// Texture description extension filter
static int txt_filter(const struct dirent *dir) {
	const char *s = dir->d_name;
	int len = strlen(s) - 4;
	if(len >= 0) {
		if (strncmp(s + len, ".txt", 4) == 0) {
			return 1;
		}
	}
	return 0;
}

void Textures::LoadTxtFile(const char *file) {
  std::string s(TEXTURES_DEFAULT_LOCATION);
  s += "/";
  s += file;
  printf("Loading %s\n", s.c_str());
  FILE *fd = fopen(s.c_str(), "r");
  if (fd == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  s.replace(s.size() - 3, 3, "png");
  printf("Loading associated %s\n", s.c_str());
  // COmpute image
  Magick::Image image;
  Magick::Blob blob;
  image.read(s.c_str());
  unsigned int ox, oy, w, h;
  char name[TEXTURES_NAME_SIZE];
  while (fscanf(fd, "%d,%d,%d,%d,%255s", &ox, &oy, &w, &h, name) == 5) {
    Magick::Image i(image);
    Magick::Blob b;
    i.crop(Magick::Geometry(w, h, ox, oy));
    i.write(&b, "RGBA");
    // Metadata
    Texture *t = new Texture(textures_.size(), w, h, name, b.data(),
        b.length());
    t->Dump();
    textures_.push_back(t);
    textures_map_[t->name()] = t;
  }
  fclose(fd);
  printf("Texture successfully loaded\n");
}

Textures::Textures(void) {
	struct dirent **namelist;
	int n;

  printf("Loading textures...\n");

	n = scandir(TEXTURES_DEFAULT_LOCATION, &namelist, txt_filter, alphasort);
	if (n < 0) {
		perror("scandir");
		exit(EXIT_FAILURE);
	} else {
		while (n--) {
      LoadTxtFile(namelist[n]->d_name);
			free(namelist[n]);
		}
		free(namelist);
	}
}
