#include "gl_setup.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

// Helper function for error checking
static void CheckGLError(const char* msg) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL Error [%s]: 0x%X\n", msg, err);
    }
}
GLuint GLSetup_CreateVAO(const float* data, int vertexCount, int componentsPerVertex) {
    // NOTE: componentsPerVertex here means floats per vertex total (e.g. 9)
    // We assume the format is: pos(3 floats), normal(3 floats), color(3 floats)
    if (componentsPerVertex != 11) {
        printf("Warning: Expected componentsPerVertex = 11 for pos+normal+color, got %d\n", componentsPerVertex);
    }

    GLuint vao = 0, vbo = 0;

    glGenVertexArrays(1, &vao);
    CheckGLError("glGenVertexArrays");
    glBindVertexArray(vao);
    CheckGLError("glBindVertexArray");

    glGenBuffers(1, &vbo);
    CheckGLError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    CheckGLError("glBindBuffer");

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), data, GL_STATIC_DRAW);
    CheckGLError("glBufferData");

    // Setup attribute pointers:

    // position (location = 0)
    glEnableVertexAttribArray(0);
    CheckGLError("glEnableVertexAttribArray(0)");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), (void*)(0));
    CheckGLError("glVertexAttribPointer(0)");

    // normal (location = 1)
    glEnableVertexAttribArray(1);
    CheckGLError("glEnableVertexAttribArray(1)");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
    CheckGLError("glVertexAttribPointer(1)");

    
    // texture (location = 2)
    glEnableVertexAttribArray(2);
    CheckGLError("glEnableVertexAttribArray(2)");
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), (void*)(6 * sizeof(float)));
    CheckGLError("glVertexAttribPointer(2)");

    // tangent (location = 3)
    glEnableVertexAttribArray(3);
    CheckGLError("glEnableVertexAttribArray(3)");
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), (void*)(8 * sizeof(float)));
    CheckGLError("glVertexAttribPointer(3)");
    
    glBindVertexArray(0);
    CheckGLError("glBindVertexArray (unbind)");

    if (vao == 0) {
        printf("Failed to create VAO.\n");
    }

    return vao;
}


GLuint GLSetup_CreateDynamicVAO(GLuint* outVBO) {
    GLuint vao = 0, vbo = 0;
    
    glGenVertexArrays(1, &vao);
    CheckGLError("glGenVertexArrays");
    glBindVertexArray(vao);
    CheckGLError("glBindVertexArray");

    glGenBuffers(1, &vbo);
    CheckGLError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    CheckGLError("glBindBuffer");

    // Do not upload data yet — it's a dynamic buffer.

    // Setup vertex attribute pointers
    glEnableVertexAttribArray(0);  // Position
    CheckGLError("glEnableVertexAttribArray (pos)");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    CheckGLError("glVertexAttribPointer (pos)");

    glEnableVertexAttribArray(1);  // Color
    CheckGLError("glEnableVertexAttribArray (color)");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    CheckGLError("glVertexAttribPointer (color)");

    

    glBindVertexArray(0);
    CheckGLError("glBindVertexArray (unbind)");

    if (outVBO) *outVBO = vbo;

    if (vao == 0 || vbo == 0) {
        printf("Failed to create dynamic VAO or VBO.\n");
    }

    return vao;
}
