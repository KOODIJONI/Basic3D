#include "texture_loader.h"
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/gl.h>
GLuint LoadTexture(const char* filename) {
    if (!filename) return 0;

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) return 0;

    GLenum format;
    if (channels == 1) format = GL_LUMINANCE;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    printf("Loaded texture %s: %dx%d, channels: %d\n", filename, width, height, channels);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    printf("Generated texture ID: %u\n", textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // IMPORTANT for 1,3 channel images
    printf("Uploading texture data to GPU...\n");
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    printf("Texture data uploaded.\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    printf("Texture parameters set: MIN_FILTER=LINEAR, MAG_FILTER=LINEAR\n");
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    printf("Texture %s loaded with ID: %u\n", filename, textureID);
    return textureID;
}


void FreeTexture(GLuint textureID) {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}
