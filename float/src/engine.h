
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

const int maxVertices = 1024;
const int maxTriangles = 1024;

extern int vertexCount;
extern Vector3 vertices[];
extern int triangleCount;
extern int triangleVertexIndices[][3];
extern Transform3 playerTransform;

void render();

#endif