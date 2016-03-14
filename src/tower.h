#ifndef TOWER_H
#define TOWER_H

#include "vector.h"

/* informationen um turm zu spawnen */
typedef struct {
	qw_image *sprite;
	int shot_timeout; /* gemessen in qw_ticks [int] */
	int damage;
	float attack_radius;
	int price;
} tower_data;

/* position, aussehen und andere eigenschaften des turms */
typedef struct {
	vector_t pos;
	tower_data data;
} tower;

/* erstellt und platziert einen neuen turm */
tower tower_place(int x, int y, tower_data td) {
	tower t = (tower) {
		.pos = vector_new(x, y),
		.data = td
	};
	
	return t;
}

/* Turm rendern */
void tower_draw(tower *t) {
	qw_placeimage(*t->data.sprite, t->pos.x - 50, t->pos.y - 50);
	qw_drawimage(*t->data.sprite);
#ifdef DEBUG
	qw_color(200, 0, 255, 255);
	qw_drawcircle(t->pos.x, t->pos.y, t->data.attack_radius);
#endif
}

/* Lasse Turm `t` angreifen */
int tower_attack(tower *t, enemy *enemies, int enemies_count) {
	for (int i = 0; i < enemies_count; ++i) {
		if (qw_tick_count % t->data.shot_timeout == 0) {
			/* ignoriere tote gegner */
			if (enemies[i].dead)
				continue;
			
			float dist = qw_distance(t->pos.x, t->pos.y, enemies[i].pos.x, enemies[i].pos.y);
			if (dist < t->data.attack_radius) {
				enemies[i].health -= t->data.damage;
				if (enemies[i].health <= 0) {
					enemies[i].dead = 1;
					return 1;
				}
				
				/* 
				 * turm greift nur einen gegner pro schuß an, 
				 * flow control entfernen damit alle gegner im radius angegriffen werden
				 */
				break;
			}
		}
	}

	return 0;
}

#endif
