
#include "engine.h"

const int vertexCount = 4;

const Vector3 vertices[vertexCount] = {
    Vector3(-0.5, -0.5, 1),
    Vector3(+0.5, -0.5, 1),
    Vector3(+0.5, +0.5, 1),
    Vector3(-0.5, +0.5, 1)
};

const int triangleCount = 2;

const int triangleVertexIndices[triangleCount][3] = {
    0, 1, 2,
    0, 2, 3
};

Vector3 transformedVertices[vertexCount];

Transform3 playerTransform;

