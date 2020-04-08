
#ifndef ENGINE_H
#define ENGINE_H

#include "Vector3.h"
#include "Transform3.h"

// screen size in pixels
#define SCREEN_WIDTH 800
#define HALF_SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600
#define HALF_SCREEN_HEIGHT 300

// inverse half-screen FOV; half the above screen size means a total FOV of 90 degrees (on that axis)
#define FOV_UNIT 400

struct Sector {

    // meta-index of the first vertex index of the first line; other vertex indices must follow
    int vertexIndexStart;

    // number of lines, each taking two vertex indices
    int lineCount;

};

// static limits
const int maxVertices = 1024;
const int maxVertexIndices = 1024;
const int maxSectors = 64;

// vertices
extern int vertexCount;
extern Vector3 vertices[];

// vertex indices (primitives use this table to share vertices since they are expensive to transform)
extern int vertexIndexCount;
extern int vertexIndices[];

// sectors
extern int sectorCount;
extern Sector sectors[];

// player
extern Transform3 playerTransform;
extern int playerSectorIndex;

// functions
void render();

#endif
