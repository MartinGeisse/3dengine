
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "engine.h"

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
static Vector2 transformedAndProjectedVertices[maxVertices];
static ALLEGRO_COLOR wireframeColor;
static ALLEGRO_COLOR splitLineColor;

// internal data: 2d clipping
static const int maxClipperStackSize = 64;
static Plane2 clipperStack[maxClipperStackSize];
static int clipperStackStart;
static int clipperStackEnd;

static void renderLine(int vertexIndex1, int vertexIndex2) {
    Vector2 a(transformedAndProjectedVertices[vertexIndex1]), b(transformedAndProjectedVertices[vertexIndex2]);
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
    al_draw_line(a.x, a.y, b.x, b.y, wireframeColor, 1.0f);
}

static void renderSector(int sectorIndex) {

    Sector *sector = sectors + sectorIndex;
    int *sectorVertexIndices = vertexIndices + sector->vertexIndexStart;
    Polygon *polygon = polygons + sector->polygonStart;

    // draw portals
    for (int i = 0; i < sector->portalCount; i++) {

        // save current clipper stack
        int oldClipperStackEnd = clipperStackEnd;

        // add new clippers from the portal
        // TODO merge clippers, don't just add them
        for (int j = 0; j < polygon->vertexCount; j++) {
            // We have to invert the order here because while all logic seems to expect counter-clockwise winding for
            // portals, the transformation to screen coordinates (+y pointing down!) actually inverts that.
            clipperStack[clipperStackEnd] = buildPlane2FromPoints(
                transformedAndProjectedVertices[sectorVertexIndices[j]],
                transformedAndProjectedVertices[sectorVertexIndices[j == 0 ? polygon->vertexCount - 1 : j - 1]]
            );
            clipperStackEnd++;
        }

        // render the target sector with the new clipper stack
        renderSector(polygon->targetSectorOrColor);

        // restore clipper stack
        clipperStackEnd = oldClipperStackEnd;

        // advance to the next polygon and its vertices
        sectorVertexIndices += polygon->vertexCount;
        polygon++;
    }

    // draw solid polygons
    for (int i = 0; i < sector->solidPolygonCount; i++) {
        for (int j = 2; j < polygon->vertexCount; j++) {
            al_draw_filled_triangle(
                transformedAndProjectedVertices[sectorVertexIndices[0]].x,
                transformedAndProjectedVertices[sectorVertexIndices[0]].y,
                transformedAndProjectedVertices[sectorVertexIndices[j - 1]].x,
                transformedAndProjectedVertices[sectorVertexIndices[j - 1]].y,
                transformedAndProjectedVertices[sectorVertexIndices[j]].x,
                transformedAndProjectedVertices[sectorVertexIndices[j]].y,
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
        transformedAndProjectedVertices[i] = Vector2(HALF_SCREEN_WIDTH + v.x / v.z * FOV_UNIT,
            HALF_SCREEN_HEIGHT - v.y / v.z * FOV_UNIT);
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
