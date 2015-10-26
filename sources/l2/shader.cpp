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
#include <iostream>
#include <fstream>

#include "shader.h"

bool ReadFile(const char *filename, std::string& outFile) {
  std::ifstream f(filename);

  bool ret = false;

  if (f.is_open()) {
    std::string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }

    f.close();

    ret = true;
  } else {
    printf("Failed to open file : %s\n", filename);
  }

  return ret;
}

void Shader::AddShader(const char *filename, GLenum shader_type) {
  std::string s;
  GLuint shader_obj = glCreateShader(shader_type);

  if (!ReadFile(filename, s)) {
    exit(1);
  };

  if (shader_obj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", shader_type);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = s.c_str();;
  GLint lengths[1];
  lengths[0]= s.size();
  glShaderSource(shader_obj, 1, p, lengths);
  glCompileShader(shader_obj);
  GLint success;
  glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info_log[1024];
    glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader_type,
        info_log);
    exit(1);
  }

  glAttachShader(program_, shader_obj);
}

void Shader::Link(void) {
  GLint success;
  GLchar error_log[1024] = { 0 };
  glLinkProgram(program_);
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(program_, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Error linking shader program: '%s'\n", error_log);
    exit(1);
  }

  glValidateProgram(program_);
  glGetProgramiv(program_, GL_VALIDATE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Invalid shader program: '%s'\n", error_log);
    exit(1);
  }
}

void Shader::Use(void) {
  glUseProgram(program_);
}

Shader::Shader(void) {
  program_ = glCreateProgram();

  if (program_ == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }
}

Shader::~Shader(void) {
  glDeleteProgram(program_);
}
