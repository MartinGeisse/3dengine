
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

const Fixed SPEED = buildFixed(0, 6554); // 0.1
const Fixed ROTATION = buildFixed(0, 6554); // 0.1
const Fixed ROT_SIN = buildFixed(0, 6543); // sin(0.1)
const Fixed ROT_COS = buildFixed(0, 65209); // cos(0.1)

static void moveRelative(Fixed dx, Fixed dy, Fixed dz) {
    Vector3 newPosition = playerTransform.v + playerTransform.m * Vector3(dx, dy, dz);
    Sector *sector = sectors + playerSectorIndex;
    for (int i = 0; i < sector->collisionPlaneCount; i++) {
        CollisionPlane *collisionPlane = collisionPlanes + sector->collisionPlaneStart + i;
        Fixed v = collisionPlane->plane.evaluate(newPosition);
        if (v < fixedZero) {
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
            moveRelative(SPEED, fixedZero, fixedZero);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_A)) {
            moveRelative(-SPEED, fixedZero, fixedZero);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_E)) {
            moveRelative(fixedZero, SPEED, fixedZero);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_C)) {
            moveRelative(fixedZero, -SPEED, fixedZero);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_W)) {
            moveRelative(fixedZero, fixedZero, SPEED);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_X)) {
            moveRelative(fixedZero, fixedZero, -SPEED);
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_RIGHT)) {
            playerTransform.m *= Matrix3(
                ROT_COS, fixedZero, ROT_SIN,
                fixedZero, fixedOne, fixedZero,
                -ROT_SIN, fixedZero, ROT_COS
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_LEFT)) {
            playerTransform.m *= Matrix3(
                ROT_COS, fixedZero, -ROT_SIN,
                fixedZero, fixedOne, fixedZero,
                ROT_SIN, fixedZero, ROT_COS
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_UP)) {
            playerTransform.m *= Matrix3(
                fixedOne, fixedZero, fixedZero,
                fixedZero, ROT_COS, -ROT_SIN,
                fixedZero, ROT_SIN, ROT_COS
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_DOWN)) {
            playerTransform.m *= Matrix3(
                fixedOne, fixedZero, fixedZero,
                fixedZero, ROT_COS, ROT_SIN,
                fixedZero, -ROT_SIN, ROT_COS
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_Q)) {
            playerTransform.m *= Matrix3(
                ROT_COS, -ROT_SIN, fixedZero,
                ROT_SIN, ROT_COS, fixedZero,
                fixedZero, fixedZero, fixedOne
            );
        }
        if (al_key_down(&keyboardState, ALLEGRO_KEY_R)) {
            playerTransform.m *= Matrix3(
                ROT_COS, ROT_SIN, fixedZero,
                -ROT_SIN, ROT_COS, fixedZero,
                fixedZero, fixedZero, fixedOne
            );
        }

        usleep(100000);
	}
    return 0;
}
