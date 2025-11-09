#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GL/gl.h>   

GLuint LoadTexture(const char* filename);

void FreeTexture(GLuint textureID);

#endif 
