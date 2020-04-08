
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "engine.h"

// vertices
int vertexCount = 0;
Vector3 vertices[maxVertices];

// vertex indices (primitives use this table to share vertices since they are expensive to transform)
int vertexIndexCount = 0;
int vertexIndices[maxVertexIndices];

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

static void renderLine(int vertexIndex1, int vertexIndex2) {
    al_draw_line(
        transformedAndProjectedVertices[vertexIndex1].x,
        transformedAndProjectedVertices[vertexIndex1].y,
        transformedAndProjectedVertices[vertexIndex2].x,
        transformedAndProjectedVertices[vertexIndex2].y,
        wireframeColor, 1.0f
    );
}

static void renderSector(int sectorIndex) {
    Sector *sector = sectors + sectorIndex;
    int *sectorVertexIndices = vertexIndices + sector->vertexIndexStart;
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

    // render
    renderSector(playerSectorIndex);

}
