#ifndef ENEMY_H
#define ENEMY_H

#include "QW/qw.h"
#include "vector.h"

/* Gegner haben position & wegpunkt information */
typedef struct {
	/* Position und richtung der bewegung*/
	vector_t pos,
	         dir;
	/* Momentaner wegpunkt */
	int waypoint;
	/* Aussehen */
	qw_image sprite;
	/* Lebt gegner noch */
	int dead;
} enemy;

/* Erstelle neuen gegner */
enemy enemy_new(ai_path wps) {
	return (enemy) {
		.pos = vector_new(wps.points[0].x, wps.points[0].y),
		.dir = vector_new(0, 0),
		.waypoint = 0,
		.sprite = qw_loadimage("assets/levels/level_1/enemies/penis.png"),
		.dead = 0
	};
}


/* Rendere gegner */
void enemy_draw(enemy *enemy) {
	qw_imagerotation(&enemy->sprite, vector_angle(enemy->pos, vector_add(enemy->pos, enemy->dir)));
	qw_placeimage(enemy->sprite, enemy->pos.x - qw_imagewidth(enemy->sprite) / 2, enemy->pos.y - qw_imageheight(enemy->sprite) / 2);
	qw_drawimage(enemy->sprite);
}

/* Bewege gegner `e` entlang dem pfad `waypoints` mit einer geschwindigkeit von `speed`  */
void enemy_move(enemy *e, ai_path waypoints, float speed) {
	/* Wähle nächsten wegpunkt als ziel */
	int wp_i = e->waypoint + 1;
	/* Überprüfe ob es der letzte wegpunkt ist */
	if (wp_i >= waypoints.points_count) {
		/* Beende bewegungsvorgang */
		//wp_i = waypoints.points_count - 1;
		return;
	}
	
	/* Choose target position */
	vector_t target = vector_new(waypoints.points[wp_i].x, waypoints.points[wp_i].y);
	/* calculate distance to next point, if we are close enough select the waypoint after that */
	float distance = vector_len(vector_sub(target, e->pos));
	if (distance < 2.f) {
		++e->waypoint;
		/* update target vector */
		target = vector_new(waypoints.points[wp_i].x, waypoints.points[wp_i].y);
	}
	

#ifdef DEBUG
	/* mark the next point */
	qw_color(200, 100, 120, 255);
	qw_drawline(e->pos.x, e->pos.y, target.x, target.y);
	qw_fillrect(target.x - 5, target.y - 5, 10, 10);
#endif
	
	/* Calculate direction to walk in */
	e->dir = vector_normalize(vector_sub(target, e->pos));
	e->dir = vector_smult(e->dir, speed);
	
	/* Finally update the enemy's position */
	e->pos = vector_add(e->pos, e->dir);
}

#endif
