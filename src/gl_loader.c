#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_loader.h"

// Function pointer definitions
PFNGLCREATESHADERPROC           glCreateShader = NULL;
PFNGLSHADERSOURCEPROC           glShaderSource = NULL;
PFNGLCOMPILESHADERPROC          glCompileShader = NULL;
PFNGLGETSHADERIVPROC            glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC       glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC          glCreateProgram = NULL;
PFNGLATTACHSHADERPROC           glAttachShader = NULL;
PFNGLLINKPROGRAMPROC            glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC             glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC     glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC       glUniformMatrix4fv = NULL;
PFNGLGENVERTEXARRAYSPROC        glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC        glBindVertexArray = NULL;
PFNGLGENBUFFERSPROC             glGenBuffers = NULL;
PFNGLBINDBUFFERPROC             glBindBuffer = NULL;
PFNGLBUFFERDATAPROC             glBufferData = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC    glVertexAttribPointer = NULL;
PFNGLGETPROGRAMIVPROC           glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC      glGetProgramInfoLog = NULL;
PFNGLDELETEVERTEXARRAYSPROC     glDeleteVertexArrays = NULL;
PFNGLDELETEPROGRAMPROC          glDeleteProgram = NULL;
PFNGLDELETESHADERPROC           glDeleteShader = NULL;
PFNGLUNIFORM1IPROC              glUniform1i = NULL;
PFNGLACTIVETEXTUREPROC           glActiveTexture = NULL;
PFNGLUNIFORM3FVPROC           glUniform3fv = NULL;

//LOAD set active texture


#define LOAD_GL_FUNC(type, name)                     \
    name = (type)wglGetProcAddress(#name);           \
    if (!(name)) {                                   \
        char msg[256];                               \
        sprintf(msg, "Failed to load OpenGL function: %s", #name); \
        MessageBoxA(0, msg, "OpenGL Error", MB_ICONERROR); \
        exit(EXIT_FAILURE);                          \
    }

void LoadGLFunctions(void) {
    LOAD_GL_FUNC(PFNGLCREATESHADERPROC,           glCreateShader);
    LOAD_GL_FUNC(PFNGLSHADERSOURCEPROC,           glShaderSource);
    LOAD_GL_FUNC(PFNGLCOMPILESHADERPROC,          glCompileShader);
    LOAD_GL_FUNC(PFNGLGETSHADERIVPROC,            glGetShaderiv);
    LOAD_GL_FUNC(PFNGLGETSHADERINFOLOGPROC,       glGetShaderInfoLog);
    LOAD_GL_FUNC(PFNGLCREATEPROGRAMPROC,          glCreateProgram);
    LOAD_GL_FUNC(PFNGLATTACHSHADERPROC,           glAttachShader);
    LOAD_GL_FUNC(PFNGLLINKPROGRAMPROC,            glLinkProgram);
    LOAD_GL_FUNC(PFNGLUSEPROGRAMPROC,             glUseProgram);
    LOAD_GL_FUNC(PFNGLGETUNIFORMLOCATIONPROC,     glGetUniformLocation);
    LOAD_GL_FUNC(PFNGLUNIFORMMATRIX4FVPROC,       glUniformMatrix4fv);
    LOAD_GL_FUNC(PFNGLGENVERTEXARRAYSPROC,        glGenVertexArrays);
    LOAD_GL_FUNC(PFNGLBINDVERTEXARRAYPROC,        glBindVertexArray);
    LOAD_GL_FUNC(PFNGLGENBUFFERSPROC,             glGenBuffers);
    LOAD_GL_FUNC(PFNGLBINDBUFFERPROC,             glBindBuffer);
    LOAD_GL_FUNC(PFNGLBUFFERDATAPROC,             glBufferData);
    LOAD_GL_FUNC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    LOAD_GL_FUNC(PFNGLVERTEXATTRIBPOINTERPROC,    glVertexAttribPointer);
    LOAD_GL_FUNC(PFNGLGETPROGRAMIVPROC,           glGetProgramiv);
    LOAD_GL_FUNC(PFNGLGETPROGRAMINFOLOGPROC,      glGetProgramInfoLog);
    LOAD_GL_FUNC(PFNGLDELETEVERTEXARRAYSPROC,     glDeleteVertexArrays);
    LOAD_GL_FUNC(PFNGLDELETEPROGRAMPROC,          glDeleteProgram);
    LOAD_GL_FUNC(PFNGLDELETESHADERPROC,           glDeleteShader);
    LOAD_GL_FUNC(PFNGLUNIFORM1IPROC, glUniform1i);
    LOAD_GL_FUNC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    LOAD_GL_FUNC(PFNGLUNIFORM3FVPROC, glUniform3fv);


    printf("All OpenGL functions loaded successfully.\n");
}
