
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "engine.h"

int vertexCount = 0;
Vector3 vertices[maxVertices];
int vertexIndexCount = 0;
int vertexIndices[maxVertexIndices];
int primitiveCount = 0;
int primitives[maxPrimitives][2];

Transform3 playerTransform;
static Transform3 inversePlayerTransform;
static Vector2 transformedAndProjectedVertices[maxVertices];
static ALLEGRO_COLOR wireframeColor;
static ALLEGRO_COLOR splitLineColor;

static void drawTriangleSplitLines(int x1, int y1, int x2, int y2, int x3, int y3, int splitDepth) {
    int x12 = (x1 + x2) / 2;
    int x23 = (x2 + x3) / 2;
    int x31 = (x3 + x1) / 2;
    int y12 = (y1 + y2) / 2;
    int y23 = (y2 + y3) / 2;
    int y31 = (y3 + y1) / 2;
    al_draw_triangle(x12, y12, x23, y23, x31, y31, splitLineColor, 1.0f);
    splitDepth--;
    if (splitDepth > 0) {
        drawTriangleSplitLines(x1, y1, x12, y12, x31, y31, splitDepth);
        drawTriangleSplitLines(x2, y2, x12, y12, x23, y23, splitDepth);
        drawTriangleSplitLines(x3, y3, x23, y23, x31, y31, splitDepth);
    }
}

static void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int splitDepth) {
    al_draw_triangle(x1, y1, x2, y2, x3, y3, wireframeColor, 1.0f);
    if (splitDepth > 0) {
        drawTriangleSplitLines(x1, y1, x2, y2, x3, y3, splitDepth);
    }
}

void render() {

    wireframeColor.r = 0.0f;
    wireframeColor.g = 1.0f;
    wireframeColor.b = 0.0f;
    wireframeColor.a = 1.0f;
    splitLineColor.r = 1.0f;
    splitLineColor.g = 0.0f;
    splitLineColor.b = 0.0f;
    splitLineColor.a = 1.0f;

    inversePlayerTransform = playerTransform.getInverse();
    for (int i = 0; i < vertexCount; i++) {
        Vector3 v = inversePlayerTransform * vertices[i];
        transformedAndProjectedVertices[i] = Vector2(HALF_SCREEN_WIDTH + v.x / v.z * FOV_UNIT,
            HALF_SCREEN_HEIGHT - v.y / v.z * FOV_UNIT);
    }

    for (int i = 0; i < primitiveCount; i++) {
        int *thisPrimitive = primitives[i];
        int *thisPrimitiveVertexIndices = vertexIndices + thisPrimitive[0];
        int thisPrimitiveVertexCount = thisPrimitive[1];

        for (int j = 2; j < thisPrimitiveVertexCount; j++) {
            drawTriangle(
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[0]].x,
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[0]].y,
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[j - 1]].x,
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[j - 1]].y,
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[j]].x,
                transformedAndProjectedVertices[thisPrimitiveVertexIndices[j]].y,
                2
            );
        }
    }
}
