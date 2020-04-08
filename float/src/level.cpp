
#include "engine.h"

int addVertex(Vector3 v) {
    if (vertexCount == maxVertices) {
        printf("too many vertices\n");
        exit(1);
    }
    vertices[vertexCount] = v;
    int returnValue = vertexCount;
    vertexCount++;
    return returnValue;
}

int addVertex(float x, float y, float z) {
    return addVertex(Vector3(x, y, z));
}

int addTriangle(int index0, int index1, int index2) {
    triangleVertexIndices[triangleCount][0] = index0;
    triangleVertexIndices[triangleCount][1] = index1;
    triangleVertexIndices[triangleCount][2] = index2;
    int returnValue = triangleCount;
    triangleCount++;
    return returnValue;
}

void addQuad(int index0, int index1, int index2, int index3) {
    addTriangle(index0, index1, index2);
    addTriangle(index0, index2, index3);
}

void extendQuadStrip(int index4, int index5) {
    int index2 = triangleVertexIndices[triangleCount - 1][1];
    int index3 = triangleVertexIndices[triangleCount - 1][2];
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
