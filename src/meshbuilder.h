#ifndef MESHBUILDER_H
#define MESHBUILDER_H

//https://www.raylib.com/cheatsheet/cheatsheet.html
#include "raylib.h"

class meshbuilder {

    public:
        meshbuilder(int triangles) 
        : mesh{ 0 }
        {
            mesh.triangleCount = triangles;
            mesh.vertexCount = mesh.triangleCount * 3;
            mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
        }

        meshbuilder& addVertex(Vector3 v) {
            mesh.vertices[currentIndex] = v.x;
            mesh.vertices[currentIndex + 1] = v.y;
            mesh.vertices[currentIndex + 2] = v.z;
            currentIndex += 3;

            return *this;
        }

        meshbuilder& addTriangle(Vector3 v1, Vector3 v2, Vector3 v3) {
            addVertex(v1);
            addVertex(v2);
            addVertex(v3);

            return *this;
        }

        Mesh build() {
            UploadMesh(&mesh, false);
            return mesh;
        }

    private:
        Mesh mesh;
        int currentIndex = 0;

};

#endif