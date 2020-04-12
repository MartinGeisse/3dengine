
#ifndef ENGINE_H
#define ENGINE_H

#define DEBUG_RENDERING 0

#include "Fixed.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "Plane3.h"
#include "Transform3.h"

// screen size in pixels
const int SCREEN_WIDTH = 800;
const int HALF_SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int HALF_SCREEN_HEIGHT = 300;

// inverse half-screen FOV; half the above screen size means a total FOV of 90 degrees (on that axis)
const int FOV_UNIT = 400;

// Fixed-typed variants of the above constants
const Fixed SCREEN_WIDTH_FIXED = intToFixed(SCREEN_WIDTH);
const Fixed HALF_SCREEN_WIDTH_FIXED = intToFixed(HALF_SCREEN_WIDTH);
const Fixed SCREEN_HEIGHT_FIXED = intToFixed(SCREEN_HEIGHT);
const Fixed HALF_SCREEN_HEIGHT_FIXED = intToFixed(HALF_SCREEN_HEIGHT);
const Fixed FOV_UNIT_FIXED = intToFixed(FOV_UNIT);

struct Polygon {

    // number of vertices
    int vertexCount;

    // For portals, this is the index of the target sector. For solid polygons, it is the color.
    int targetSectorOrColor;

};

struct Sector {

    // Meta-index of the first vertex index; other vertex indices must follow. Vertex indices are used by polygons
    // first, followed by lines.
    int vertexIndexStart;

    // index of the first polygon in the polygon table; other polygons must follow
    int polygonStart;

    // number of portal polygons. Note: winding must be counter-clockwise.
    int portalCount;

    // number of solid (colored) polygons
    int solidPolygonCount;

    // number of lines, each taking two vertex indices
    int lineCount;

    // index of the first collision plane
    int collisionPlaneStart;

    // number of collision planes
    int collisionPlaneCount;

};

struct CollisionPlane {
    Plane3 plane;
    int targetSector;
};

// static limits
const int maxVertices = 1024;
const int maxVertexIndices = 1024;
const int maxPolygons = 256;
const int maxSectors = 64;
const int maxCollisionPlanes = 1024;

// vertices
extern int vertexCount;
extern Vector3 vertices[];

// vertex indices (primitives use this table to share vertices since they are expensive to transform)
extern int vertexIndexCount;
extern int vertexIndices[];

// polygons
extern int polygonCount;
extern Polygon polygons[];

// collision detection
extern int collisionPlaneCount;
extern CollisionPlane collisionPlanes[];

// sectors
extern int sectorCount;
extern Sector sectors[];

// player
extern Transform3 playerTransform;
extern int playerSectorIndex;

// functions
void render();

#endif
