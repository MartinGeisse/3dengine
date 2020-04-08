
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
static Vector2 transformedAndProjectedVertices[maxVertices];
static float transformedVertexDepth[maxVertices];
static ALLEGRO_COLOR wireframeColor;
static ALLEGRO_COLOR splitLineColor;

// internal data: 2d clipping
static const int maxClipperStackSize = 64;
static Plane2 clipperStack[maxClipperStackSize];
static int clipperStackStart;
static int clipperStackEnd;

static void renderLine(int vertexIndex1, int vertexIndex2) {
    Vector2 a(transformedAndProjectedVertices[vertexIndex1]), b(transformedAndProjectedVertices[vertexIndex2]);

    // clip against near plane
    // TODO right now there is a simple bug in the code that produces totally wrong results
    // TODO but after that, we have a real problem here because we cannot simply clip a line after projection -- the
    // result will be wrong in a similar way to non-perspective-correct textures. We have to clip before the division:
    // either in "clip space", like OpenGL, which is immediately before the division, or in eye space.
    // -- this is probably not the same as the above "simple bug" because right now it only makes lines that intersect
    // the near plane have the wrong length from the visible vertex towards the invisible vertex -- the line's angle
    // should not change. In the code below, (vaz / (vbz - vaz)) produces the wrong result.
    // --> the above "simple bug" is related. The invisible point gets reflected to the wrong side of the screen center.
    // All this means that the main work of the projection (at least the division, but then it's complex, better the
    // whole projection) cannot be reliably done before (near plane) clipping. This is sad because near plane clipping
    // is the only clipping we have to do in 3d -- portal clipping could be done in 2d. Also, sharing the projected
    // vertices becomes way more complex now. The only upside is that clipping may now help us discard a few vertices
    // before projection.
    // --
    // Alternative: Since it's only about the near plane, we might do the following:
    // - store all vertices in eye coords and after projection (easy, just takes some memory)
    // -- might be solved by a trick: the Z coordinate is the same for both, so if z < NEAR, then it's in eye coords,
    //    otherwise projected
    // -- no, that won't do: If one of the vertices of a line is too near, we need BOTH of them in eye coords! So
    //    we have to store both. Anyway,
    // - check if fully visible or invisible WRT near plane, if so, use already projected vertices
    // - if partially visible, clip, then project, and use those --> sounds like a plan
    float vaz = transformedVertexDepth[vertexIndex1] - NEAR_Z;
    float vbz = transformedVertexDepth[vertexIndex2] - NEAR_Z;
    if (vaz < 0) {
        if (vbz < 0) {
            // invisible
            return;
        } else {
            // point a clipped away
            a -= (b - a) * vaz / (vbz - vaz);
        }
    } else if (vbz < 0) {
        // point b clipped away
        b -= (a - b) * vbz / (vaz - vbz);
    } // else: fully visible WRT the near plane

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
        // TODO how does clipping portals themselves work?
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
        transformedVertexDepth[i] = v.z;
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
