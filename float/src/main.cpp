
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <unistd.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "Plane3.h"
#include "Transform3.h"
#include "engine.h"
#include "level.h"

#define SPEED 0.1f
#define ROTATION 0.1f

static void moveRelative(float dx, float dy, float dz) {
    Vector3 newPosition = playerTransform.v + playerTransform.m * Vector3(dx, dy, dz);
    Sector *sector = sectors + playerSectorIndex;
    for (int i = 0; i < sector->collisionPlaneCount; i++) {
        CollisionPlane *collisionPlane = collisionPlanes + sector->collisionPlaneStart + i;
        float v = collisionPlane->plane.evaluate(newPosition);
        if (v < 0) {
            if (collisionPlane->targetSector < 0) {
                newPosition = collisionPlane->plane.projectPointOntoPlane(newPosition);
            } else {
                playerSectorIndex = collisionPlane->targetSector;
                sector = sectors + playerSectorIndex;
                i = -1;
            }
        }
    }
    playerTransform.v = newPosition;
}

int main() {
    buildLevel();
	al_init();
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	al_init_primitives_addon();
	al_install_keyboard();
	while (true) {

		al_clear_to_color(al_map_rgb(0, 0, 0));
		render();
		al_flip_display();

        ALLEGRO_KEYBOARD_STATE keyboardState;
        al_get_keyboard_state(&keyboardState);
        if (al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) {
            break;
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_D)) {
            moveRelative(SPEED, 0, 0);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_A)) {
            moveRelative(-SPEED, 0, 0);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_E)) {
            moveRelative(0, SPEED, 0);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_C)) {
            moveRelative(0, -SPEED, 0);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_W)) {
            moveRelative(0, 0, SPEED);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_X)) {
            moveRelative(0, 0, -SPEED);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_RIGHT)) {
            playerTransform.m *= Matrix3(
                cosf(ROTATION), 0, sinf(ROTATION),
                0, 1, 0,
                -sinf(ROTATION), 0, cosf(ROTATION)
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_LEFT)) {
            playerTransform.m *= Matrix3(
                cosf(ROTATION), 0, -sinf(ROTATION),
                0, 1, 0,
                sinf(ROTATION), 0, cosf(ROTATION)
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_UP)) {
            playerTransform.m *= Matrix3(
                1, 0, 0,
                0, cosf(ROTATION), -sinf(ROTATION),
                0, sinf(ROTATION), cosf(ROTATION)
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_DOWN)) {
            playerTransform.m *= Matrix3(
                1, 0, 0,
                0, cosf(ROTATION), sinf(ROTATION),
                0, -sinf(ROTATION), cosf(ROTATION)
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_Q)) {
            playerTransform.m *= Matrix3(
                cosf(ROTATION), -sinf(ROTATION), 0,
                sinf(ROTATION), cosf(ROTATION), 0,
                0, 0, 1
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_R)) {
            playerTransform.m *= Matrix3(
                cosf(ROTATION), sinf(ROTATION), 0,
                -sinf(ROTATION), cosf(ROTATION), 0,
                0, 0, 1
            );
        }

        usleep(100000);
	}
    return 0;
}
