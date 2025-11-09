
#include "scene_loader.h"
#include "OBJ_file_loader.h"
#include "gl_setup.h"
#include "matrix_utils.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_loader.h"
void LoadSceneFromFile(const char* filename, ObjectVector* objects) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open scene file: %s\n", filename);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    if (!root) {
        printf("Failed to parse scene JSON.\n");
        return;
    }

    cJSON* objectsArray = cJSON_GetObjectItem(root, "objects");
    int objectCount = cJSON_GetArraySize(objectsArray);
    printf("Loading %d objects from scene file.\n", objectCount);
    for (int i = 0; i < objectCount; i++) {
        cJSON* objItem = cJSON_GetArrayItem(objectsArray, i);
        cJSON* meshItem = cJSON_GetObjectItem(objItem, "mesh");
        if (!meshItem || !meshItem->valuestring) {
            printf("Object %d missing mesh file!\n", i);
            continue;
        }
        const char* meshFile = meshItem->valuestring;

        ObjMesh mesh;
        if (LoadOBJ(meshFile, &mesh)) {
            printf("Failed to load OBJ: %s\n", meshFile);
            continue;
        }
        
        cJSON* textureItem = cJSON_GetObjectItem(objItem, "textures");
        const char* textureFile = NULL;
        if (textureItem && textureItem->valuestring) {
            printf("Object %d tex file!\n", i);
            printf("Texture file: %s\n", textureItem->valuestring);
            textureFile = textureItem->valuestring;
            printf("Texture file: %s\n", textureFile);

            GLuint myTexture = LoadTexture(textureFile); // path to your image
            if (myTexture == 0) {
                fprintf(stderr, "Failed to load texture!\n");
                continue;

            }
            else{
                printf("Texture loaded successfully with ID: %u\n", myTexture);
                mesh.textureID = myTexture;
            }
        }
        else{
            printf("Object %d has no texture file.\n", i);

        }

         
        

        printf("Loaded mesh with %zu triangle vertices.\n", mesh.triangle_vertex_count);

        cJSON* folder = cJSON_GetObjectItem(objItem, "folder");
         const char* folderString = NULL; 
         if (folder && folder->valuestring) {
            folderString = folder->valuestring;
             char file[512];
             snprintf(file, sizeof(file), "%s/smoothNormals.bins", folderString);
             printf("Looking for smooth normals file: %s\n", file);
             ComputeSmoothNormals(file,&mesh);

         }






        printf("Computed smooth normals for mesh.\n");
        GLuint vao = GLSetup_CreateVAO(mesh.triangle_vertices, mesh.triangle_vertex_count,8);

        cJSON* pos = cJSON_GetObjectItem(objItem, "position");
        cJSON* rot = cJSON_GetObjectItem(objItem, "rotation");
        cJSON* scale = cJSON_GetObjectItem(objItem, "scale");

        float model[16];
        CreateIdentityMatrix(model);

        CreateScaleMatrix(scale->child->valuedouble, scale->child->next->valuedouble, scale->child->next->next->valuedouble, model);

        float rotMat[16];
        CreateRotationMatrix(rot->child->valuedouble, rot->child->next->valuedouble, rot->child->next->next->valuedouble, rotMat);
        MultiplyMatrices(model, rotMat, model);

        float trans[16];
        CreateTranslationMatrix(pos->child->valuedouble, pos->child->next->valuedouble, pos->child->next->next->valuedouble, trans);
        MultiplyMatrices(model, trans, model);

        RenderableObject obj;
        obj.vao = vao;
        obj.vertexCount = mesh.triangle_vertex_count;
        obj.textureID = mesh.textureID;
        memcpy(obj.modelMatrix, model, sizeof(float) * 16);

        // Add to vector
        ObjectVector_Push(objects, obj);
    }

    cJSON_Delete(root);
}
