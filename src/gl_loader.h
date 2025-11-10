#ifndef GL_LOADER_H
#define GL_LOADER_H

#include <GL/gl.h>
#include <GL/glext.h>

// Function pointer declarations
extern PFNGLCREATESHADERPROC           glCreateShader;
extern PFNGLSHADERSOURCEPROC           glShaderSource;
extern PFNGLCOMPILESHADERPROC          glCompileShader;
extern PFNGLGETSHADERIVPROC            glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC       glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC          glCreateProgram;
extern PFNGLATTACHSHADERPROC           glAttachShader;
extern PFNGLLINKPROGRAMPROC            glLinkProgram;
extern PFNGLUSEPROGRAMPROC             glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC     glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC       glUniformMatrix4fv;
extern PFNGLGENVERTEXARRAYSPROC        glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC        glBindVertexArray;
extern PFNGLGENBUFFERSPROC             glGenBuffers;
extern PFNGLBINDBUFFERPROC             glBindBuffer;
extern PFNGLBUFFERDATAPROC             glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC    glVertexAttribPointer;
extern PFNGLGETPROGRAMIVPROC           glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC      glGetProgramInfoLog;
extern PFNGLDELETEVERTEXARRAYSPROC     glDeleteVertexArrays;
extern PFNGLDELETEPROGRAMPROC          glDeleteProgram;
extern PFNGLDELETESHADERPROC           glDeleteShader;
extern PFNGLUNIFORM1IPROC              glUniform1i;
extern PFNGLACTIVETEXTUREPROC          glActiveTexture;
extern PFNGLUNIFORM3FVPROC           glUniform3fv;
// Loader function
void LoadGLFunctions(void);

#endif // GL_LOADER_H
