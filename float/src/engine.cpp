
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "engine.h"

// constants
const float NEAR_Z = 0.1f;

// vertices
int vertexCount = 0;
Vector3 vertices[maxVertices];

// vertex indices (primitives use this table to share vertices since they are expensive to transform)
int vertexIndexCount = 0;
int vertexIndices[maxVertexIndices];

// polygons
int polygonCount = 0;
Polygon polygons[maxPolygons];

// sectors
int sectorCount = 0;
Sector sectors[maxSectors];

// player
Transform3 playerTransform;
int playerSectorIndex = 0;

// internal data
static Transform3 inversePlayerTransform;
static Vector3 transformedVertices[maxVertices];
static Vector2 projectedVertices[maxVertices]; // only valid if (z >= NEAR_Z), otherwise we must clip BEFORE projection
static ALLEGRO_COLOR wireframeColor;
static ALLEGRO_COLOR splitLineColor;

// internal data: 2d clipping
static const int maxClipperStackSize = 64;
static Plane2 clipperStack[maxClipperStackSize];
static int clipperStackStart;
static int clipperStackEnd;

// internal data: current polygon
static int currentPolygonVertexCount3;
static Vector3 currentPolygonVertices3[64];
static Vector3 currentPolygonBackupVertices3[64];
static int currentPolygonVertexCount2;
static Vector2 currentPolygonVertices2[64];
static Vector2 currentPolygonBackupVertices2[64];
static float currentPolygonEvaluations[64];

// called once for all non-near vertices, and on demand for near vertices after clipping
inline static Vector2 project(Vector3 v) {
    return Vector2(HALF_SCREEN_WIDTH + v.x / v.z * FOV_UNIT, HALF_SCREEN_HEIGHT - v.y / v.z * FOV_UNIT);
}

static void renderLine(Vector2 a, Vector2 b) {

    // clip against screen boundaries / portal boundaries
    for (int i = clipperStackStart; i < clipperStackEnd; i++) {
        Plane2 *clipper = clipperStack + i;
        float va = clipper->evaluate(a);
        float vb = clipper->evaluate(b);
        if (va < 0) {
            if (vb < 0) {
                // invisible
                return;
            } else {
                // point a clipped away
                a -= (b - a) * va / (vb - va);
            }
        } else if (vb < 0) {
            // point b clipped away
            b -= (a - b) * vb / (va - vb);
        } // else: fully visible WRT this clipper
    }

    // draw clipped line
    al_draw_line(a.x, a.y, b.x, b.y, wireframeColor, 1.0f);

}

static void renderLine(int vertexIndex1, int vertexIndex2) {
    // For "near" vertices, we must clip against the near plane, then transform the clipped vertex. For non-near
    // vertices, we can use the shared, already-transformed vertices. Note that if one vertex is near, we need the
    // non-projected version of the other vertex too, to perform clipping itself.
    float vaz = transformedVertices[vertexIndex1].z - NEAR_Z;
    float vbz = transformedVertices[vertexIndex2].z - NEAR_Z;
    if (vaz < 0) {
        if (vbz < 0) {
            // invisible
            return;
        } else {
            // first point clipped away
            Vector3 a = transformedVertices[vertexIndex1];
            Vector3 b = transformedVertices[vertexIndex2];
            a -= (b - a) * vaz / (vbz - vaz);
            renderLine(project(a), projectedVertices[vertexIndex2]);
        }
    } else if (vbz < 0) {
        // second point clipped away
        Vector3 a = transformedVertices[vertexIndex1];
        Vector3 b = transformedVertices[vertexIndex2];
        b -= (a - b) * vbz / (vaz - vbz);
        renderLine(projectedVertices[vertexIndex1], project(b));
    } else {
        // fully visible WRT the near plane, so use the shared, already-transformed vertices
        renderLine(projectedVertices[vertexIndex1], projectedVertices[vertexIndex2]);
    }
}

// fills the "currentPolygon*" data structures
static void projectAndClipPolygon(int *polygonVertexIndices, int vertexCount) {

    // First, check if near plane clipping is necessary , i.e. at least one vertex is nearer than the near plane.
    // Also, fail fast if ALL vertices are nearer than the near plane.
    int anyNearVertex = 0, anyFarVertex = 0;
    for (int i = 0; i < vertexCount; i++) {
        float z = transformedVertices[polygonVertexIndices[i]].z;
        if (z > NEAR_Z) {
            anyFarVertex = 1;
        } else {
            anyNearVertex = 1;
        }
    }
    if (!anyFarVertex) {
        return;
    }
    if (anyNearVertex) {

        // near plane clipping is needed, so first make a copy of the vertices
        currentPolygonVertexCount3 = vertexCount;
        for (int i = 0; i < vertexCount; i++) {
            currentPolygonVertices3[i] = transformedVertices[polygonVertexIndices[i]];
        }

        // Evaluate all vertices against the near plane. While we could evaluate on the fly with a simple subtraction,
        // this might lead to this block and the 2d one below being merged in the future since we keep them similar
        // to each other.
        // During clipping, we read from the backup array, to handle the case that one vertex in the polygon
        // becomes two vertices after clipping. This happens if only a single vertex is clipped away.
        for (int j = 0; j < currentPolygonVertexCount3; j++) {
            currentPolygonEvaluations[j] = currentPolygonVertices3[j].z - NEAR_Z;
            currentPolygonBackupVertices3[j] = currentPolygonVertices3[j];
        }

        // clip each side
        int outputVertexCount = 0;
        for (int j = 0; j < currentPolygonVertexCount3; j++) {
            Vector3 currentVertex = currentPolygonBackupVertices3[j];
            float currentEvaluation = currentPolygonEvaluations[j];
            if (currentEvaluation > 0) {
                currentPolygonVertices3[outputVertexCount] = currentVertex;
                outputVertexCount++;
                continue;
            }
            int previousIndex = (j == 0 ? currentPolygonVertexCount3 - 1 : j - 1);
            int nextIndex = (j == currentPolygonVertexCount3 - 1 ? 0 : j + 1);
            float previousEvaluation = currentPolygonEvaluations[previousIndex];
            float nextEvaluation = currentPolygonEvaluations[nextIndex];
            if (previousEvaluation > 0) {
                currentPolygonVertices3[outputVertexCount] =
                    currentVertex - (currentPolygonBackupVertices3[previousIndex] - currentVertex) *
                        currentEvaluation / (previousEvaluation - currentEvaluation);
                outputVertexCount++;
            }
            if (nextEvaluation > 0) {
                currentPolygonVertices3[outputVertexCount] =
                    currentVertex - (currentPolygonBackupVertices3[nextIndex] - currentVertex) *
                        currentEvaluation / (nextEvaluation - currentEvaluation);
                outputVertexCount++;
            }
        }
        currentPolygonVertexCount3 = outputVertexCount;

        // project the clipped vertices
        currentPolygonVertexCount2 = currentPolygonVertexCount3;
        for (int i = 0; i < currentPolygonVertexCount3; i++) {
            currentPolygonVertices2[i] = project(currentPolygonVertices3[i]);
        }

    } else {

        // just copy the already projected vertices
        currentPolygonVertexCount2 = vertexCount;
        for (int i = 0; i < vertexCount; i++) {
            currentPolygonVertices2[i] = projectedVertices[polygonVertexIndices[i]];
        }

    }

    // 2d clipping against screen / portal boundaries
    for (int i = clipperStackStart; i < clipperStackEnd; i++) {
        Plane2 *clipper = clipperStack + i;

        // Evaluate all vertices against this clipper, and fail fast if none is visible.
        // Also, during clipping, we read from the backup array, to handle the case that one vertex in the polygon
        // becomes two vertices after clipping. This happens if only a single vertex is clipped away.
        int anyVertexVisible = 0;
        for (int j = 0; j < currentPolygonVertexCount2; j++) {
            float evaluation = clipper->evaluate(currentPolygonVertices2[j]);
            if (evaluation > 0) {
                anyVertexVisible = 1;
            }
            currentPolygonEvaluations[j] = evaluation;
            currentPolygonBackupVertices2[j] = currentPolygonVertices2[j];
        }
        if (!anyVertexVisible) {
            currentPolygonVertexCount2 = 0;
            return;
        }

        // clip each side
        int outputVertexCount = 0;
        for (int j = 0; j < currentPolygonVertexCount2; j++) {
            Vector2 currentVertex = currentPolygonBackupVertices2[j];
            float currentEvaluation = currentPolygonEvaluations[j];
            if (currentEvaluation > 0) {
                currentPolygonVertices2[outputVertexCount] = currentVertex;
                outputVertexCount++;
                continue;
            }
            int previousIndex = (j == 0 ? currentPolygonVertexCount2 - 1 : j - 1);
            int nextIndex = (j == currentPolygonVertexCount2 - 1 ? 0 : j + 1);
            float previousEvaluation = currentPolygonEvaluations[previousIndex];
            float nextEvaluation = currentPolygonEvaluations[nextIndex];
            if (previousEvaluation > 0) {
                currentPolygonVertices2[outputVertexCount] =
                    currentVertex - (currentPolygonBackupVertices2[previousIndex] - currentVertex) *
                        currentEvaluation / (previousEvaluation - currentEvaluation);
                outputVertexCount++;
            }
            if (nextEvaluation > 0) {
                currentPolygonVertices2[outputVertexCount] =
                    currentVertex - (currentPolygonBackupVertices2[nextIndex] - currentVertex) *
                        currentEvaluation / (nextEvaluation - currentEvaluation);
                outputVertexCount++;
            }
        }
        currentPolygonVertexCount2 = outputVertexCount;

    }

}

static void renderSector(int sectorIndex) {

    Sector *sector = sectors + sectorIndex;
    int *sectorVertexIndices = vertexIndices + sector->vertexIndexStart;
    Polygon *polygon = polygons + sector->polygonStart;

    // draw portals
    for (int i = 0; i < sector->portalCount; i++) {

        // clip the portal
        projectAndClipPolygon(sectorVertexIndices, polygon->vertexCount);
        if (currentPolygonVertexCount2 != 0) {

            // save current clipper stack
            int oldClipperStackStart = clipperStackStart;

            // install the clipped portal as the only active clippers
            clipperStackStart = clipperStackEnd;
            clipperStackEnd += currentPolygonVertexCount2;
            for (int j = 0; j < currentPolygonVertexCount2; j++) {
                // We have to invert the order here because while all logic seems to expect counter-clockwise winding for
                // portals, the transformation to screen coordinates (+y pointing down!) actually inverts that.
                clipperStack[clipperStackStart + j] = buildPlane2FromPoints(
                    currentPolygonVertices2[j],
                    currentPolygonVertices2[j == 0 ? currentPolygonVertexCount2 - 1 : j - 1]
                );
            }

            // render the target sector with the new clipper stack
            renderSector(polygon->targetSectorOrColor);

            // restore clipper stack
            clipperStackEnd = clipperStackStart;
            clipperStackStart = oldClipperStackStart;

        }

        // advance to the next polygon and its vertices
        sectorVertexIndices += polygon->vertexCount;
        polygon++;

    }

    // draw solid polygons
    for (int i = 0; i < sector->solidPolygonCount; i++) {
        projectAndClipPolygon(sectorVertexIndices, polygon->vertexCount);
        for (int j = 2; j < currentPolygonVertexCount2; j++) {
            al_draw_filled_triangle(
                currentPolygonVertices2[0].x,
                currentPolygonVertices2[0].y,
                currentPolygonVertices2[j - 1].x,
                currentPolygonVertices2[j - 1].y,
                currentPolygonVertices2[j].x,
                currentPolygonVertices2[j].y,
                splitLineColor
            );
        }
        sectorVertexIndices += polygon->vertexCount;
        polygon++;
    }

    // draw lines
    for (int i = 0; i < sector->lineCount; i++) {
        renderLine(sectorVertexIndices[0], sectorVertexIndices[1]);
        sectorVertexIndices += 2;
    }

}

void render() {

    // initialize (might go into a one-time initialization)
    wireframeColor.r = 0.0f;
    wireframeColor.g = 1.0f;
    wireframeColor.b = 0.0f;
    wireframeColor.a = 1.0f;
    splitLineColor.r = 1.0f;
    splitLineColor.g = 0.0f;
    splitLineColor.b = 0.0f;
    splitLineColor.a = 1.0f;

    // transform all vertices
    inversePlayerTransform = playerTransform.getInverse();
    for (int i = 0; i < vertexCount; i++) {
        Vector3 v = inversePlayerTransform * vertices[i];
        transformedVertices[i] = v;
        if (v.z >= NEAR_Z) {
            projectedVertices[i] = project(v);
        }
    }

    // reset clipping
    clipperStackStart = 0;
    clipperStackEnd = 4;
    clipperStack[0] = buildPlane2FromPoints(Vector2(0, 0), Vector2(SCREEN_WIDTH, 0));
    clipperStack[1] = buildPlane2FromPoints(Vector2(SCREEN_WIDTH, 0), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
    clipperStack[2] = buildPlane2FromPoints(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), Vector2(0, SCREEN_HEIGHT));
    clipperStack[3] = buildPlane2FromPoints(Vector2(0, SCREEN_HEIGHT), Vector2(0, 0));

    // render
    renderSector(playerSectorIndex);

}
