#ifndef OBJ_FILE_LOADER_H
#define OBJ_FILE_LOADER_H

#include <stddef.h>
#include <GL/gl.h>
typedef struct {
    float* vertices;     
    size_t vertex_count;  

    float* texcoords;     
    size_t texcoord_count; 

    float* normals;       
    size_t normal_count;  

    int* indices;         
    size_t index_count;   

    float* triangle_vertices; 
    size_t triangle_vertex_count;

    float transform[16];

    GLuint textureID;

    float color[3]; 
} ObjMesh;

typedef struct VertexNode {
    size_t index;
    struct VertexNode* next;
} VertexNode;

int LoadOBJ(const char* filename, ObjMesh* mesh);
void freeMesh(ObjMesh* mesh);

void printVertices(const ObjMesh* mesh);
void ComputeSmoothNormals(const char * filename ,ObjMesh* mesh);
void AddFaceAsTriangles(ObjMesh* mesh, int* vis, int* vnis, int* vtis, int vcount, int lineNumber);
void SaveMeshVertices(const char* filename, const ObjMesh* mesh);

#endif
