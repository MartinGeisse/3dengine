
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

void addSector() {
    sectors[sectorCount].vertexIndexStart = vertexIndexCount;
    sectors[sectorCount].lineCount = 0;
    sectorCount++;
}

void addLine(int index0, int index1) {
    vertexIndices[vertexIndexCount] = index0;
    vertexIndices[vertexIndexCount + 1]  = index1;
    vertexIndexCount += 2;
    sectors[sectorCount - 1].lineCount++;
}

void addCubeLines(int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7) {

    addSector();

    addLine(i0, i1);
    addLine(i1, i2);
    addLine(i2, i3);
    addLine(i3, i0);

    addLine(i4, i5);
    addLine(i5, i6);
    addLine(i6, i7);
    addLine(i7, i4);

    addLine(i0, i4);
    addLine(i1, i5);
    addLine(i2, i6);
    addLine(i3, i7);

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

    addCubeLines(0, 1, 2, 3, 4, 5, 6, 7);

}

void dumpLevel() {
/*
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
*/
}
