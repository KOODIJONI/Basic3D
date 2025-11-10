#include "obj_file_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

static void printWorkingDir(void) {
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd))) {
        fprintf(stderr, "Current working directory: %s\n", cwd);
    } else {
        fprintf(stderr, "Warning: unable to get working directory: %s\n", strerror(errno));
    }
}

static void initMesh(ObjMesh* mesh) {
    mesh->vertices = NULL;
    mesh->vertex_count = 0;
    mesh->texcoords = NULL;
    mesh->texcoord_count = 0;
    mesh->normals = NULL;
    mesh->normal_count = 0;
    mesh->indices = NULL;
    mesh->index_count = 0;
    mesh->triangle_vertices = NULL;
    mesh->triangle_vertex_count = 0;
}

void freeMesh(ObjMesh* mesh) {
    if (!mesh) return;
    free(mesh->vertices);
    free(mesh->texcoords);
    free(mesh->normals);
    free(mesh->indices);
    free(mesh->triangle_vertices);
    initMesh(mesh);
}

// Helper to parse a single face vertex token of format "v", "v/vt", "v//vn" or "v/vt/vn"
static void parseFaceVertex(const char* token, int* vi, int* vti, int* vni) {
    *vi = -1; *vti = -1; *vni = -1;

    // Copy token to modifiable string
    char buf[64];
    strncpy(buf, token, sizeof(buf)-1);
    buf[sizeof(buf)-1] = 0;

    char* ptr = buf;
    char* slash1 = strchr(ptr, '/');
    if (!slash1) {
        // Format: v
        *vi = atoi(ptr) - 1;
        return;
    }

    *slash1 = 0;
    *vi = atoi(ptr) - 1;

    char* slash2 = strchr(slash1 + 1, '/');
    if (!slash2) {
        // Format: v/vt
        *vti = atoi(slash1 + 1) - 1;
        return;
    }

    *slash2 = 0;
    if (slash1 + 1 != slash2) {
        // Format: v/vt/vn
        *vti = atoi(slash1 + 1) - 1;
    }
    *vni = atoi(slash2 + 1) - 1;
}

int LoadOBJ(const char* filename, ObjMesh* mesh) {
    if (!filename || !mesh) {
        fprintf(stderr, "LoadOBJ error: null pointer provided\n");
        return 1;
    }

    printWorkingDir();

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: failed to open OBJ file '%s': %s\n", filename, strerror(errno));
        return 1;
    }

    initMesh(mesh);

    char line[512];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;

        if (line[0] == 'v' && line[1] == ' ') {
            float x, y, z;
            if (sscanf(line + 2, "%f %f %f", &x, &y, &z) == 3) {
                float* tmp = realloc(mesh->vertices, (mesh->vertex_count + 3) * sizeof(float));
                if (!tmp) {
                    fprintf(stderr, "Error: out of memory expanding vertices at line %d\n", lineNumber);
                    fclose(file);
                    return 2;
                }
                mesh->vertices = tmp;
                mesh->vertices[mesh->vertex_count++] = x;
                mesh->vertices[mesh->vertex_count++] = y;
                mesh->vertices[mesh->vertex_count++] = z;
            } else {
                fprintf(stderr, "Warning: malformed vertex at line %d: %s", lineNumber, line);
            }
        } else if (strncmp(line, "vt ", 3) == 0) {
            float u = 0.0f, v = 0.0f;
            if (sscanf(line + 3, "%f %f", &u, &v) >= 1) {
                float* tmp = realloc(mesh->texcoords, (mesh->texcoord_count + 2) * sizeof(float));
                if (!tmp) {
                    fprintf(stderr, "Error: out of memory expanding texcoords at line %d\n", lineNumber);
                    fclose(file);
                    return 2;
                }
                mesh->texcoords = tmp;
                mesh->texcoords[mesh->texcoord_count++] = u;
                v = 1.0f - v;

                mesh->texcoords[mesh->texcoord_count++] = v;
            } else {
                fprintf(stderr, "Warning: malformed texcoord at line %d: %s", lineNumber, line);
            }
        } else if (strncmp(line, "vn ", 3) == 0) {
            float nx, ny, nz;
            if (sscanf(line + 3, "%f %f %f", &nx, &ny, &nz) == 3) {
                float* tmp = realloc(mesh->normals, (mesh->normal_count + 3) * sizeof(float));
                if (!tmp) {
                    fprintf(stderr, "Error: out of memory expanding normals at line %d\n", lineNumber);
                    fclose(file);
                    return 2;
                }
                mesh->normals = tmp;
                mesh->normals[mesh->normal_count++] = nx;
                mesh->normals[mesh->normal_count++] = ny;
                mesh->normals[mesh->normal_count++] = nz;
            } else {
                fprintf(stderr, "Warning: malformed normal at line %d: %s", lineNumber, line);
            }
        } else if (line[0] == 'f' && line[1] == ' ') {
    int maxFaceVerts = 64;
    int* vis  = malloc(maxFaceVerts * sizeof(int));
    int* vnis = malloc(maxFaceVerts * sizeof(int));
    int* vtis = malloc(maxFaceVerts * sizeof(int));
    int faceVertexCount = 0;

    char* token = strtok(line + 2, " \t\r\n");
    while (token && faceVertexCount < maxFaceVerts) {
        int vi, vti, vni;
        parseFaceVertex(token, &vi, &vti, &vni);

        vis[faceVertexCount]  = vi;
        vtis[faceVertexCount] = vti;
        vnis[faceVertexCount] = vni;

        faceVertexCount++;
        token = strtok(NULL, " \t\r\n");
    }

    if (faceVertexCount < 3) {
        fprintf(stderr, "Warning: face with less than 3 vertices at line %d\n", lineNumber);
    } else if (faceVertexCount == 3) {
        AddFaceAsTriangles(mesh, vis, vnis, vtis, 3, lineNumber);
    } else {
        // Fan triangulation for n-gon
        for (int i = 1; i < faceVertexCount - 1; i++) {
            int tri[3]   = { vis[0], vis[i], vis[i+1] };
            int tri_n[3] = { vnis[0], vnis[i], vnis[i+1] };
            int tri_t[3] = { vtis[0], vtis[i], vtis[i+1] };
            AddFaceAsTriangles(mesh, tri, tri_n, tri_t, 3, lineNumber);
        }
        if (lineNumber % 1000 == 0) {
            printf("Triangulated an n-gon with %d vertices at line %d\n", faceVertexCount, lineNumber);
            
        }
    }

    free(vis);
    free(vnis);
    free(vtis);
}
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file '%s' at line %d: %s\n", filename, lineNumber, strerror(errno));
        fclose(file);
        return 3;
    }

    fclose(file);

    for (int i = 0; i < 16; ++i) {
        mesh->transform[i] = (i % 5 == 0) ? 1.0f : 0.0f; // Identity matrix
    }

    for (int i = 0; i < 3; ++i) {
        mesh->color[i] = 1.0f;
    }
    return 0;
}

void printVertices(const ObjMesh* mesh) {
    if (!mesh || !mesh->triangle_vertices) return;

    // Now each vertex has 9 floats: pos(3), normal(3), color(3)
    // Triangles = triangle_vertex_count / (3 vertices * 9 floats)
    size_t floatsPerTriangle = 3 * 9;
    for (size_t i = 0; i + floatsPerTriangle <= mesh->triangle_vertex_count; i += floatsPerTriangle) {
        printf("Triangle %zu:\n", i / floatsPerTriangle);
        for (int v = 0; v < 3; ++v) {
            size_t base = i + v * 9;
            printf("  Vertex %c: Pos(%f, %f, %f), Normal(%f, %f, %f), Color(%f, %f, %f)\n",
                'A' + v,
                mesh->triangle_vertices[base + 0], mesh->triangle_vertices[base + 1], mesh->triangle_vertices[base + 2],
                mesh->triangle_vertices[base + 3], mesh->triangle_vertices[base + 4], mesh->triangle_vertices[base + 5],
                mesh->triangle_vertices[base + 6], mesh->triangle_vertices[base + 7], mesh->triangle_vertices[base + 8]);
        }
    }
}
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Compare two vertices for near-equality


static inline int nearlyEqual(float a, float b, float eps) {
    return fabsf(a - b) < eps;
}

static inline int sameVertex(const float* v1, const float* v2, float eps) {
    return nearlyEqual(v1[0], v2[0], eps) &&
           nearlyEqual(v1[1], v2[1], eps) &&
           nearlyEqual(v1[2], v2[2], eps);
}

static int FileExists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}


typedef struct {
    float x, y, z;
} Vec3;


#define EPSILON 1e-4f
#define ANGLE_THRESHOLD cosf(45.0f * 3.14159f / 180.0f)
#define FLOATS_PER_VERTEX 11

// Simple hash table entry for vertex groups
typedef struct VertexGroup {
    Vec3 pos;
    size_t* indices;   // indices of vertices with this position
    size_t count;
    size_t capacity;
    struct VertexGroup* next;
} VertexGroup;

#define HASH_SIZE 4096

// Hash function for Vec3
static unsigned int Vec3Hash(const Vec3* v) {
    int ix = (int)(v->x / EPSILON);
    int iy = (int)(v->y / EPSILON);
    int iz = (int)(v->z / EPSILON);
    return (unsigned int)((ix*73856093) ^ (iy*19349663) ^ (iz*83492791)) % HASH_SIZE;
}

// Compare two vertices within EPSILON
static int Vec3Equal(const Vec3* a, const Vec3* b) {
    return fabsf(a->x - b->x) < EPSILON &&
           fabsf(a->y - b->y) < EPSILON &&
           fabsf(a->z - b->z) < EPSILON;
}

// Insert vertex index into hash table
static void AddVertexGroup(VertexGroup** table, const Vec3* pos, size_t index) {
    unsigned int h = Vec3Hash(pos);
    VertexGroup* node = table[h];
    while (node) {
        if (Vec3Equal(&node->pos, pos)) {
            if (node->count >= node->capacity) {
                node->capacity = node->capacity ? node->capacity*2 : 4;
                node->indices = realloc(node->indices, node->capacity * sizeof(size_t));
            }
            node->indices[node->count++] = index;
            return;
        }
        node = node->next;
    }

    // New group
    node = (VertexGroup*)malloc(sizeof(VertexGroup));
    node->pos = *pos;
    node->capacity = 4;
    node->indices = (size_t*)malloc(node->capacity * sizeof(size_t));
    node->indices[0] = index;
    node->count = 1;
    node->next = table[h];
    table[h] = node;
}

// Free hash table
static void FreeVertexGroups(VertexGroup** table) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        VertexGroup* node = table[i];
        while (node) {
            VertexGroup* next = node->next;
            free(node->indices);
            free(node);
            node = next;
        }
    }
}

// Compute smooth normals in pure C with hash map
void ComputeSmoothNormals(const char* filename, ObjMesh* mesh) {
    if(FileExists(filename)){
        FILE * f = fopen(filename, "rb");
        if(f){
            size_t count =0;
            fread(&count, sizeof(size_t),1,f);
            if(count == mesh->triangle_vertex_count){
                fread(mesh->triangle_vertices, sizeof(float), count, f);
                fclose(f);
                printf("[ComputeSmoothNormals] Loaded smooth normals from file.\n");
                return;
            }
            else{
                printf("[ComputeSmoothNormals] File mismatch, recalculating normals...\n");
            }
            fclose(f);
        }
    }
    else{
        printf("[ComputeSmoothNormals] No precomputed normals file found, calculating...\n");
    }
    if (!mesh || !mesh->triangle_vertices || mesh->triangle_vertex_count == 0)
        return;

    if (FileExists(filename)) {
        FILE* f = fopen(filename, "rb");
        if (f) {
            size_t count = 0;
            fread(&count, sizeof(size_t), 1, f);
            if (count == mesh->triangle_vertex_count) {
                fread(mesh->triangle_vertices, sizeof(float), count, f);
                fclose(f);
                fprintf(stderr, "[ComputeSmoothNormals] Loaded smooth normals from file.\n");
                return;
            }
            fclose(f);
        }
        fprintf(stderr, "[ComputeSmoothNormals] File mismatch, recalculating normals...\n");
    }

    size_t vertexCount = mesh->triangle_vertex_count / FLOATS_PER_VERTEX;
    Vec3* positions = (Vec3*)malloc(sizeof(Vec3) * vertexCount);
    Vec3* faceNormals = (Vec3*)malloc(sizeof(Vec3) * vertexCount);

    if (!positions || !faceNormals) {
        fprintf(stderr, "[ComputeSmoothNormals] Out of memory\n");
        free(positions);
        free(faceNormals);
        return;
    }

    // Copy positions
    for (size_t i = 0; i < vertexCount; ++i) {
        positions[i].x = mesh->triangle_vertices[i*FLOATS_PER_VERTEX + 0];
        positions[i].y = mesh->triangle_vertices[i*FLOATS_PER_VERTEX + 1];
        positions[i].z = mesh->triangle_vertices[i*FLOATS_PER_VERTEX + 2];
    }

    // Compute face normals per triangle
    for (size_t i = 0; i + 3 <= vertexCount; i += 3) {
        Vec3 v0 = positions[i];
        Vec3 v1 = positions[i+1];
        Vec3 v2 = positions[i+2];

        Vec3 e1 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
        Vec3 e2 = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};

        Vec3 fn = {
            e1.y * e2.z - e1.z * e2.y,
            e1.z * e2.x - e1.x * e2.z,
            e1.x * e2.y - e1.y * e2.x
        };

        float len = sqrtf(fn.x*fn.x + fn.y*fn.y + fn.z*fn.z);
        if (len > 1e-6f) {
            fn.x /= len; fn.y /= len; fn.z /= len;
        }

        for (int j = 0; j < 3; ++j)
            faceNormals[i+j] = fn;
    }

    // Build hash table for vertices
    VertexGroup* table[HASH_SIZE] = {0};
    for (size_t i = 0; i < vertexCount; ++i)
        AddVertexGroup(table, &positions[i], i);

    // Smooth normals per vertex group
    for (int h = 0; h < HASH_SIZE; ++h) {
        VertexGroup* node = table[h];
        while (node) {
            for (size_t idx = 0; idx < node->count; ++idx) {
                Vec3 sum = {0,0,0};
                for (size_t jdx = 0; jdx < node->count; ++jdx) {
                    float dot = faceNormals[node->indices[idx]].x*faceNormals[node->indices[jdx]].x +
                                faceNormals[node->indices[idx]].y*faceNormals[node->indices[jdx]].y +
                                faceNormals[node->indices[idx]].z*faceNormals[node->indices[jdx]].z;
                    if (dot >= ANGLE_THRESHOLD) {
                        sum.x += faceNormals[node->indices[jdx]].x;
                        sum.y += faceNormals[node->indices[jdx]].y;
                        sum.z += faceNormals[node->indices[jdx]].z;
                    }
                }
                float len = sqrtf(sum.x*sum.x + sum.y*sum.y + sum.z*sum.z);
                if (len > 1e-6f) {
                    sum.x /= len; sum.y /= len; sum.z /= len;
                }
                size_t vi = node->indices[idx];
                mesh->triangle_vertices[vi*FLOATS_PER_VERTEX + 3] = sum.x;
                mesh->triangle_vertices[vi*FLOATS_PER_VERTEX + 4] = sum.y;
                mesh->triangle_vertices[vi*FLOATS_PER_VERTEX + 5] = sum.z;
            }
            node = node->next;
        }
    }

    SaveMeshVertices(filename, mesh);
    free(positions);
    free(faceNormals);
    FreeVertexGroups(table);

    fprintf(stderr, "[ComputeSmoothNormals] Smoothing done (%zu verts)\n", vertexCount);
}


void AddFaceAsTriangles(ObjMesh* mesh, int* vis, int* vnis, int* vtis, int vcount, int lineNumber) {
    if (vcount != 3) {
        fprintf(stderr, "AddFaceAsTriangles expects exactly 3 vertices, got %d at line %d\n", vcount, lineNumber);
        return;
    }

    for (int i = 0; i < 3; ++i) {
        int vi = vis[i];
        int vni = vnis[i];  
        int vti = vtis[i];
        if (vi < 0 || vi * 3 + 2 >= mesh->vertex_count) {
            fprintf(stderr, "Invalid vertex index %d at line %d\n", vi, lineNumber);
            continue;
        }

        float vx = mesh->vertices[vi * 3 + 0];
        float vy = mesh->vertices[vi * 3 + 1];
        float vz = mesh->vertices[vi * 3 + 2];

        float nx = 0.0f, ny = 0.0f, nz = 0.0f;
        if (vni >= 0 && vni * 3 + 2 < mesh->normal_count) {
            nx = mesh->normals[vni * 3 + 0];
            ny = mesh->normals[vni * 3 + 1];
            nz = mesh->normals[vni * 3 + 2];
        }
        float u = 1.0f, v = 1.0f;
        if (vti >= 0 && vti * 2 + 1 < mesh->texcoord_count) {
            u = mesh->texcoords[vti * 2 + 0];
            v = mesh->texcoords[vti * 2 + 1];
        }
        // Default white color
        float r = 1.0f, g = 1.0f, b = 1.0f;

        float* tmp = realloc(mesh->triangle_vertices, (mesh->triangle_vertex_count + 11) * sizeof(float));
        if (!tmp) {
            fprintf(stderr, "Out of memory expanding triangle buffer at line %d\n", lineNumber);
            return;
        }

        mesh->triangle_vertices = tmp;

        mesh->triangle_vertices[mesh->triangle_vertex_count++] = vx;
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = vy;
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = vz;

        mesh->triangle_vertices[mesh->triangle_vertex_count++] = nx;
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = ny;
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = nz;

        mesh->triangle_vertices[mesh->triangle_vertex_count++] = u;
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = v;

        mesh->triangle_vertices[mesh->triangle_vertex_count++] = 0.0f; // tangent x
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = 0.0f; // tangent y
        mesh->triangle_vertices[mesh->triangle_vertex_count++] = 0.0f; // tangent z
    }
}
void SaveMeshVertices(const char* filename, const ObjMesh* mesh) {
    if (!mesh || !mesh->triangle_vertices || mesh->triangle_vertex_count == 0) {
        fprintf(stderr, "[SaveMeshVertices] Invalid mesh\n");
        return;
    }

    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("[SaveMeshVertices] Failed to open file");
        return;
    }

    // Write vertex count first (for easier loading)
    fwrite(&mesh->triangle_vertex_count, sizeof(size_t), 1, f);

    // Write the actual vertex data
    fwrite(mesh->triangle_vertices, sizeof(float), mesh->triangle_vertex_count, f);

    fclose(f);
    fprintf(stderr, "[SaveMeshVertices] Saved %zu floats to %s\n", mesh->triangle_vertex_count, filename);
}
void ComputeTangents(ObjMesh* mesh) {
    if (!mesh || !mesh->triangle_vertices) return;

    size_t vertexCount = mesh->triangle_vertex_count / FLOATS_PER_VERTEX;

    for (size_t i = 0; i + 3 <= vertexCount; i += 3) {
        float* v0 = &mesh->triangle_vertices[i*FLOATS_PER_VERTEX + 0];
        float* v1 = &mesh->triangle_vertices[(i+1)*FLOATS_PER_VERTEX + 0];
        float* v2 = &mesh->triangle_vertices[(i+2)*FLOATS_PER_VERTEX + 0];

        // Positions
        float x1 = v1[0] - v0[0];
        float y1 = v1[1] - v0[1];
        float z1 = v1[2] - v0[2];

        float x2 = v2[0] - v0[0];
        float y2 = v2[1] - v0[1];
        float z2 = v2[2] - v0[2];

        // UVs
        float s1 = v1[6] - v0[6];
        float t1 = v1[7] - v0[7];
        float s2 = v2[6] - v0[6];
        float t2 = v2[7] - v0[7];

        float r = (s1 * t2 - s2 * t1);
        if (fabsf(r) < 1e-8f) r = 1.0f; // avoid division by zero
        else r = 1.0f / r;

        float tx = (t2 * x1 - t1 * x2) * r;
        float ty = (t2 * y1 - t1 * y2) * r;
        float tz = (t2 * z1 - t1 * z2) * r;

        // Store tangent per vertex
        for (int j = 0; j < 3; ++j) {
            mesh->triangle_vertices[(i+j)*FLOATS_PER_VERTEX + 8] = tx;
            mesh->triangle_vertices[(i+j)*FLOATS_PER_VERTEX + 9] = ty;
            mesh->triangle_vertices[(i+j)*FLOATS_PER_VERTEX + 10] = tz;
        }
    }
}
