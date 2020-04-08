
#include "engine.h"

void addVertex(Vector3 v) {
    if (vertexCount == maxVertices) {
        printf("too many vertices\n");
        exit(1);
    }
    vertices[vertexCount] = v;
    vertexCount++;
}

void addVertex(float x, float y, float z) {
    addVertex(Vector3(x, y, z));
}

void addTriangle(int index0, int index1, int index2) {
    primitives[primitiveCount][0] = vertexIndexCount;
    primitives[primitiveCount][1] = 3;
    primitiveCount++;
    vertexIndices[vertexIndexCount]  = index0;
    vertexIndices[vertexIndexCount + 1]  = index1;
    vertexIndices[vertexIndexCount + 2]  = index2;
    vertexIndexCount += 3;
}

void addQuad(int index0, int index1, int index2, int index3) {
    addTriangle(index0, index1, index2);
    addTriangle(index0, index2, index3);
}

void extendQuadStrip(int index4, int index5) {
    int index2 = vertexIndices[vertexIndexCount - 2];
    int index3 = vertexIndices[vertexIndexCount - 1];
    addQuad(index2, index3, index4, index5);
}

void buildLevel() {

    addVertex(-1, -1, -1); // 0
    addVertex(-1, -1, +1); // 1
    addVertex(+1, -1, +1); // 2
    addVertex(+1, -1, -1); // 3
    addVertex(-1, +1, -1); // 4
    addVertex(-1, +1, +1); // 5
    addVertex(+1, +1, +1); // 6
    addVertex(+1, +1, -1); // 7

    addQuad(0, 1, 2, 3);
    extendQuadStrip(7, 6);
    extendQuadStrip(5, 4);
    extendQuadStrip(0, 1);

}

void dumpLevel() {

    printf("%d vertices\n", vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        vertices[i].print();
        printf("\n");
    }
    printf("\n");

    printf("%d vertex indices\n", vertexIndexCount);
    for (int i = 0; i < vertexIndexCount; i++) {
        printf("%d, ", vertexIndices[i]);
    }
    printf("\n\n");

    printf("%d primitives\n", primitiveCount);
    for (int i = 0; i < primitiveCount; i++) {
        printf("from %d, count %d\n", primitives[i][0], primitives[i][1]);
    }
    printf("\n");

}
