
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "engine.h"

int vertexCount = 0;
Vector3 vertices[maxVertices];
int triangleCount = 0;
int triangleVertexIndices[maxTriangles][3];

/*
const int vertexCount = 4;

const Vector3 vertices[vertexCount] = {
    Vector3(-0.3, -0.3, 1),
    Vector3(+0.3, -0.3, 1),
    Vector3(+0.3, +0.3, 1),
    Vector3(-0.3, +0.3, 1)
};

const int triangleCount = 2;

const int triangleVertexIndices[triangleCount][3] = {
    0, 1, 2,
    0, 2, 3
};
*/

Transform3 playerTransform;
static Transform3 inversePlayerTransform;
static Vector2 transformedAndProjectedVertices[maxVertices];

void render() {
    inversePlayerTransform = playerTransform.getInverse();
    ALLEGRO_COLOR wireframeColor;
    wireframeColor.r = 1.0f;
    wireframeColor.g = 0.0f;
    wireframeColor.b = 0.0f;
    wireframeColor.a = 1.0f;
    for (int i = 0; i < vertexCount; i++) {
        Vector3 v = inversePlayerTransform * vertices[i];
        transformedAndProjectedVertices[i] = Vector2(HALF_SCREEN_WIDTH + v.x / v.z * FOV_UNIT,
            HALF_SCREEN_HEIGHT - v.y / v.z * FOV_UNIT);
    }
    for (int i = 0; i < triangleCount; i++) {
        al_draw_triangle(
            transformedAndProjectedVertices[triangleVertexIndices[i][0]].x,
            transformedAndProjectedVertices[triangleVertexIndices[i][0]].y,
            transformedAndProjectedVertices[triangleVertexIndices[i][1]].x,
            transformedAndProjectedVertices[triangleVertexIndices[i][1]].y,
            transformedAndProjectedVertices[triangleVertexIndices[i][2]].x,
            transformedAndProjectedVertices[triangleVertexIndices[i][2]].y,
            wireframeColor,
            1.0f
        );
    }
}
