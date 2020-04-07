#include <allegro5/allegro.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "Plane3.h"
#include "Transform3.h"
#include "engine.h"

int main() {
/*
	al_init();
	ALLEGRO_DISPLAY *display = al_create_display(640, 480);
	while (true) {
		al_clear_to_color(al_map_rgb(255, 0, 0));
		al_flip_display();
	}
*/
    Vector3 v(1, 2, 3);
    (3 * v).print();
    printf("\n");
    return 0;
}
