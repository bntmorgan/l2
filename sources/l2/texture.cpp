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

#include <stdio.h>
#include "texture.h"

#include <GL/glew.h>

void Texture::Dump(void) {
  printf("txt(%u, %u, %u, %s, %lu B)\n", id_, w_, h_, name_, length_);
//  int i = 0;
//  printf("-- Beg Data --\n");
//  for (i = 0; i < length_; i++) {
//    printf("%x ", ((unsigned char *)data_)[i]);
//  }
//  printf("\n-- End Data --\n");
}

void Texture::InitGL(void) {
  texture_target_ = GL_TEXTURE_2D;
  glGenTextures(1, &texture_obj_);
  glBindTexture(texture_target_, texture_obj_);
  glTexImage2D(texture_target_, 0, GL_RGBA, w_, h_, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, data_);
  glTexParameterf(texture_target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(texture_target_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(texture_target_, 0);
}

void Texture::DestroyGL(void) {
}

void Texture::Bind(GLenum texture_unit) {
  glActiveTexture(texture_unit);
  glBindTexture(texture_target_, texture_obj_);
}

