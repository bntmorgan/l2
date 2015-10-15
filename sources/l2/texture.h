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

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>

#define TEXTURES_NAME_SIZE 256

class Texture {
  private:
    unsigned int id_;
    unsigned int w_;
    unsigned int h_;
    char name_[TEXTURES_NAME_SIZE];
    void *data_;
    size_t length_;
    GLenum texture_target_;
    GLuint texture_obj_;
  public:
    Texture(unsigned int id, unsigned int w, unsigned int h, const char *name,
        const void *data, size_t length) : id_(id), w_(w), h_(h) {
      strcpy(name_, name);
      data_ = malloc(length);
      memcpy(data_, data, length);
      length_ = length;
      InitGL();
    };
    ~Texture(void) {
      free(data_);
      DestroyGL();
    }
    void Dump(void);
    const char *name() { return name_; }
    void InitGL(void);
    void DestroyGL(void);
    void Bind(GLenum texture_unit);
};

#endif//__TEXTURE_H__
