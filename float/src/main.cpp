
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Plane2.h"
#include "Plane3.h"
#include "Transform3.h"
#include "engine.h"

int main() {
	al_init();
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	al_init_primitives_addon();
	while (true) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		render();
		al_flip_display();
	}
    return 0;
}
