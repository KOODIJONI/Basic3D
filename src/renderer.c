// --- [ includes, globals ] ---
#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "renderer.h"
#include "gl_loader.h"
#include "projection.h"
#include "matrix_utils.h"
#include "shader.h"
#include "camera_control.h"
#include "OBJ_file_loader.h"
#include "shader_manager.h"
#include "gl_setup.h"
#include "object_manager.h"
#include "scene_loader.h"
#include <GL/glu.h>
#include "texture_utils.h"

static ObjMesh terrainMesh = {0};
static ObjMesh treeMesh = {0};

static GLuint shaderProgram = 0;

// Terrain
static GLuint vaoTerrain = 0;
static int terrainVertexCount = 0;
static float* terrainVertices = NULL;

// Tree
static GLuint vaoTree = 0;
static int treeVertexCount = 0;
static float* treeVertices = NULL;

static GLint uniformProjectionLoc = -1;
static GLint uniformModelLoc = -1;
static GLint uniformViewLoc = -1;
static GLint uniformCastsShadowsLoc = -1;
float projectionMatrix[16];
float viewMatrix[16];

float directionalLight[3] = {0.3f, 1.0f, 0.0f}; 
float lightColor[3] = {0.0f, 1.01f, 1.0f};

static ObjectVector objects; 

GLuint emptyTexture;

void Renderer_Init(void) {

    CameraControl_Init();

    // if (!LoadOBJ("assets/largeTerrain.obj", &terrainMesh)) {
    //     terrainVertices = terrainMesh.triangle_vertices;
    //     terrainVertexCount = terrainMesh.triangle_vertex_count;
    //     vaoTerrain = GLSetup_CreateVAO(terrainVertices, terrainVertexCount, 9);
    //     RenderableObject terrainObj = CreateRenderableObject(vaoTerrain, terrainVertexCount, 0.0f, 0.0f, -10.0f);          
    //     ObjectVector_Push(&objects, terrainObj);
    // } else {
    //     MessageBoxA(0, "Failed to load terrain", "Error", MB_ICONERROR);
    //     exit(1);
    // }
    // if (!LoadOBJ("assets/cube2.obj", &treeMesh)) {
    //     treeVertices = treeMesh.triangle_vertices;
    //     treeVertexCount = treeMesh.triangle_vertex_count;
    //     vaoTree = GLSetup_CreateVAO(treeVertices, treeVertexCount, 9);
    // } else {
    //     MessageBoxA(0, "Failed to load tree", "Error", MB_ICONERROR);
    //     exit(1);
    // }
    ObjectVector_Init(&objects);

    LoadSceneFromFile("assets/scene.json", &objects);
    printf("Number of objects loaded: %d\n", objects.size);

    shaderProgram = ShaderManager_CreateProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    if (!shaderProgram) {
        MessageBoxA(0, "Shader program failed", "Error", MB_ICONERROR);
        exit(1);
    }

    uniformProjectionLoc = ShaderManager_GetUniformLocation(shaderProgram, "uProjection");
    uniformViewLoc       = ShaderManager_GetUniformLocation(shaderProgram, "uView");
    uniformModelLoc      = ShaderManager_GetUniformLocation(shaderProgram, "uModel");
    uniformCastsShadowsLoc = glGetUniformLocation(shaderProgram, "uCastsShadows");
    
    GLint uTextureLoc = glGetUniformLocation(shaderProgram, "uTexture");
    GLint uNormalMapLoc = glGetUniformLocation(shaderProgram, "uNormalMap");
    GLint uRoughnessMapLoc = glGetUniformLocation(shaderProgram, "uRoughnessMap");
    GLint uMetalnessMapLoc = glGetUniformLocation(shaderProgram, "uMetalnessMap");
    GLint uAOMapLoc = glGetUniformLocation(shaderProgram, "uAOMap");


    GLint uLightDirLoc = glGetUniformLocation(shaderProgram, "uLightDir");
    printf("uLightDir location: %d\n", uLightDirLoc);

    if (uLightDirLoc != -1) {
        float len = sqrtf(directionalLight[0]*directionalLight[0] +
                        directionalLight[1]*directionalLight[1] +
                        directionalLight[2]*directionalLight[2]);
        float normDir[3] = { directionalLight[0]/len, directionalLight[1]/len, directionalLight[2]/len };
        glUniform3fv(uLightDirLoc, 1, normDir);
    }

    if (uNormalMapLoc != -1) {
        glUniform1i(uNormalMapLoc, 1); 
    }

    if (uTextureLoc != -1) {
        glUniform1i(uTextureLoc, 0); 
    }
    if (uRoughnessMapLoc != -1) {
        glUniform1i(uRoughnessMapLoc, 2); 
    }
    if (uMetalnessMapLoc != -1) {
        glUniform1i(uMetalnessMapLoc, 3); 
    }
    if (uAOMapLoc != -1) {
        glUniform1i(uAOMapLoc, 4); 
    }

    emptyTexture = CreateWhiteTexture();


    float fovY = 45.0f * (3.1415926f / 180.0f); 
    float aspect = 800.0f / 600.0f;
    float nearr = 0.1f;
    float farr = 10000.0f;
    CreatePerspectiveProjection(fovY, aspect, nearr, farr, projectionMatrix);
}

void DrawObject(GLuint vao, int vertexCount, float* modelMatrix, GLuint textureID, GLuint normalID, GLuint roughnessID, GLuint metalnessID, GLuint aoID) {
    glBindVertexArray(vao);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error before drawing: %s\n", gluErrorString(err));
        printf("At line: %d\n", __LINE__);
    }
    // Upload the model matrix
    if (uniformModelLoc != -1) {
        glUniformMatrix4fv(uniformModelLoc, 1, GL_FALSE, modelMatrix);
    }
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %s\n", gluErrorString(err));
    }
    // Bind the texture
   if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalID ? normalID : emptyTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, roughnessID ? roughnessID : emptyTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, metalnessID ? metalnessID : emptyTexture);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, aoID ? aoID : emptyTexture);


    glDrawArrays(GL_TRIANGLES, 0, vertexCount); // Or glDrawElements if using EBO

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

// --- [ draw ] ---
void Renderer_Draw(float deltaTime) {
    // Clear screen and enable depth test
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Upload the projection matrix
    if (uniformProjectionLoc != -1) {
        glUniformMatrix4fv(uniformProjectionLoc, 1, GL_FALSE, projectionMatrix);
    }

    // Update the camera and upload the view matrix
    CameraControl_Update(deltaTime, viewMatrix);
    if (uniformViewLoc != -1) {
        glUniformMatrix4fv(uniformViewLoc, 1, GL_FALSE, viewMatrix);
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %s\n", gluErrorString(err));
        printf("At line: %d\n", __LINE__);
    }

    // Draw all objects in the vector
    for (int i = 0; i < objects.size; i++) {
        RenderableObject* obj = &objects.data[i];
        if (uniformCastsShadowsLoc != -1) {
            glUniform1i(uniformCastsShadowsLoc, obj->castsShadows ? 1 : 0);
        }

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, obj->normalID ? obj->normalID : emptyTexture);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, obj->roughnessID ? obj->roughnessID : emptyTexture);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, obj->metalnessID ? obj->metalnessID : emptyTexture);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, obj->aoID ? obj->aoID : emptyTexture);

        // Make sure shader uses unit 1
        GLint uNormalMapLoc = glGetUniformLocation(shaderProgram, "uNormalMap");
        glUniform1i(uNormalMapLoc, 1);

        GLint uRoughnessMapLoc = glGetUniformLocation(shaderProgram, "uRoughnessMap");
        glUniform1i(uRoughnessMapLoc, 2);

        GLint uMetalnessMapLoc = glGetUniformLocation(shaderProgram, "uMetalnessMap");
        glUniform1i(uMetalnessMapLoc, 3);

        GLint uAOMapLoc = glGetUniformLocation(shaderProgram, "uAOMap");
        glUniform1i(uAOMapLoc, 4);

        DrawObject(obj->vao, obj->vertexCount, obj->modelMatrix,obj->textureID,obj->normalID,obj->roughnessID,obj->metalnessID,obj->aoID);
        
    
    }
}


// --- [ cleanup ] ---
void Renderer_Cleanup(void) {
    glDeleteVertexArrays(1, &vaoTerrain);
    glDeleteVertexArrays(1, &vaoTree);
    glDeleteProgram(shaderProgram);
}

float Get_Aspect_Ratio(void) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = viewport[2];
    int height = viewport[3];
    return (height == 0) ? 1.0f : (float)width / (float)height;
}

void UpdateProjectionMatrix(float aspect) {
    float fovY = 45.0f * (3.1415926f / 180.0f); // radians
    float nearr = 0.1f;
    float farr = 10000.0f;
    CreatePerspectiveProjection(fovY, aspect, nearr, farr, projectionMatrix);

    if (uniformProjectionLoc != -1) {
        glUniformMatrix4fv(uniformProjectionLoc, 1, GL_FALSE, projectionMatrix);
    }
}

void PrintVertexAndNormalBuffer(float* buffer, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        int baseIdx = i * 11;
        printf("Vertex %d: Pos(%.3f, %.3f, %.3f), Normal(%.3f, %.3f, %.3f)\n",
            i,
            buffer[baseIdx], buffer[baseIdx + 1], buffer[baseIdx + 2],
            buffer[baseIdx + 3], buffer[baseIdx + 4], buffer[baseIdx + 5]);
    }
}