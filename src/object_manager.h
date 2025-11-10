#pragma once
#include <GL/gl.h>
#include <stddef.h>
#include <stdbool.h>
typedef struct {
    GLuint vao;
    int vertexCount;
    float modelMatrix[16];
    
    GLuint textureID;
    GLuint normalID;
    GLuint roughnessID;
    GLuint metalnessID;
    GLuint aoID;

    bool castsShadows;

} RenderableObject;

typedef struct {
    RenderableObject* data;
    int size;
    int capacity;
} ObjectVector;

void ObjectVector_Init(ObjectVector* vec);
void ObjectVector_Push(ObjectVector* vec, RenderableObject obj);
void ObjectVector_Free(ObjectVector* vec);
RenderableObject CreateRenderableObject(GLuint vao, int vertexCount, float x, float y, float z);
