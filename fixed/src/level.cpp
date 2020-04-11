
#include <stdlib.h>
#include "engine.h"

void addVertex(Vector3 v) {
    if (vertexCount == maxVertices) {
        printf("too many vertices\n");
        exit(1);
    }
    vertices[vertexCount] = v;
    vertexCount++;
}

void addVertex(Fixed x, Fixed y, Fixed z) {
    addVertex(Vector3(x, y, z));
}

void addVertex(int x, int y, int z) {
    addVertex(intToFixed(x), intToFixed(y), intToFixed(z));
}

// Note: after addSector, sector contents must be added in the order: solid polygons, then lines. Any other order
// will mess up the vertices since their order is partially implicit.
void addSector() {
    sectors[sectorCount].vertexIndexStart = vertexIndexCount;
    sectors[sectorCount].polygonStart = polygonCount;
    sectors[sectorCount].solidPolygonCount = 0;
    sectors[sectorCount].portalCount = 0;
    sectors[sectorCount].lineCount = 0;
    sectors[sectorCount].collisionPlaneStart = collisionPlaneCount;
    sectors[sectorCount].collisionPlaneCount = 0;
    sectorCount++;
}

void addPolygon4(int index0, int index1, int index2, int index3, int targetSectorOrColor) {

    // add vertex indices to the vertex index table
    vertexIndices[vertexIndexCount] = index0;
    vertexIndices[vertexIndexCount + 1] = index1;
    vertexIndices[vertexIndexCount + 2] = index2;
    vertexIndices[vertexIndexCount + 3] = index3;
    vertexIndexCount += 4;

    // add polygon to the polygon table
    polygons[polygonCount].vertexCount = 4;
    polygons[polygonCount].targetSectorOrColor = targetSectorOrColor;
    polygonCount++;

    // add polygon to the sector
    sectors[sectorCount - 1].solidPolygonCount++;

}

void finishPortals() {
    sectors[sectorCount - 1].portalCount = sectors[sectorCount - 1].solidPolygonCount;
    sectors[sectorCount - 1].solidPolygonCount = 0;
}

void addLine(int index0, int index1) {

    // add vertex indices to the vertex index table
    vertexIndices[vertexIndexCount] = index0;
    vertexIndices[vertexIndexCount + 1]  = index1;
    vertexIndexCount += 2;

    // add line to the sector
    sectors[sectorCount - 1].lineCount++;

}


void addCubeLines(int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7) {

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

void addCollisionPlane(Vector3 v1, Vector3 v2, Vector3 v3, int targetSector) {
    collisionPlanes[collisionPlaneCount].plane = buildPlane3FromPoints(v1, v2, v3).getNormalized();
    collisionPlanes[collisionPlaneCount].targetSector = targetSector;
    collisionPlaneCount++;
    sectors[sectorCount - 1].collisionPlaneCount++;
}

void addCollisionPlane(Fixed a, Fixed b, Fixed c, Fixed d, int targetSector) {
    collisionPlanes[collisionPlaneCount].plane = Plane3(a, b, c, d).getNormalized();
    collisionPlanes[collisionPlaneCount].targetSector = targetSector;
    collisionPlaneCount++;
    sectors[sectorCount - 1].collisionPlaneCount++;
}

void addCollisionPlane(int a, int b, int c, int d, int targetSector) {
    addCollisionPlane(intToFixed(a), intToFixed(b), intToFixed(c), intToFixed(d), targetSector);
}

void buildLevel() {

    addVertex(-1, -1, -1); // 0
    addVertex(-1, -1, +3); // 1
    addVertex(+1, -1, +1); // 2
    addVertex(+1, -1, -1); // 3
    addVertex(-1, +1, -1); // 4
    addVertex(-1, +1, +3); // 5
    addVertex(+1, +1, +1); // 6
    addVertex(+1, +1, -1); // 7

    addSector();
    addPolygon4(1, 2, 6, 5, 1);
    finishPortals();
    addPolygon4(0, 1, 2, 3, 4);
    addCubeLines(0, 1, 2, 3, 4, 5, 6, 7);

    addCollisionPlane(1, 0, 0, 1, -1);
    addCollisionPlane(-1, 0, 0, 1, -1);
    addCollisionPlane(0, 1, 0, 1, -1);
    addCollisionPlane(0, -1, 0, 1, -1);
    addCollisionPlane(0, 0, 1, 1, -1);
    addCollisionPlane(vertices[1], vertices[2], vertices[6], 1);

    // -------------------------------------------------------------------------------

    addVertex(+5, -1, +3); // 8
    addVertex(+3, -1, +1); // 9
    addVertex(+5, +1, +3); // 10
    addVertex(+3, +1, +1); // 11

    addSector();
    addPolygon4(5, 6, 2, 1, 0);
    addPolygon4(8, 9, 11, 10, 2);
    finishPortals();
    addPolygon4(1, 8, 9, 2, 4);
    addCubeLines(1, 8, 9, 2, 5, 10, 11, 6);

    addCollisionPlane(vertices[1], vertices[6], vertices[2], 0);
    addCollisionPlane(vertices[8], vertices[9], vertices[11], 2);
    addCollisionPlane(0, 1, 0, 1, -1);
    addCollisionPlane(0, -1, 0, 1, -1);
    addCollisionPlane(0, 0, 1, -1, -1);
    addCollisionPlane(0, 0, -1, 3, -1);


    // -------------------------------------------------------------------------------

    addVertex(+5, -1, -1); // 12
    addVertex(+3, -1, -1); // 13
    addVertex(+5, +1, -1); // 14
    addVertex(+3, +1, -1); // 15

    addSector();
    addPolygon4(10, 11, 9, 8, 1);
    finishPortals();
    addPolygon4(8, 9, 13, 12, 4);
    addCubeLines(8, 9, 13, 12, 10, 11, 15, 14);

    addCollisionPlane(vertices[8], vertices[11], vertices[9], 1);
    addCollisionPlane(0, 1, 0, 1, -1);
    addCollisionPlane(0, -1, 0, 1, -1);
    addCollisionPlane(1, 0, 0, -3, -1);
    addCollisionPlane(-1, 0, 0, 5, -1);
    addCollisionPlane(0, 0, 1, 1, -1);

}
