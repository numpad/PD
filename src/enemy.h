#ifndef ENEMY_H
#define ENEMY_H

#include "QW/qw.h"
#include "level_data.h"
#include "vector.h"

/* Gegner haben position & wegpunkt information */
typedef struct {
	/* Position und richtung der bewegung*/
	vector_t pos,
	         dir;
	/* Momentaner wegpunkt */
	int waypoint;
	/* Aussehen */
	qw_image *sprite;
	/* Lebt gegner noch */
	int dead;
	int health;
	
	/* Geschwindigkeit */
	float speed;

	/* Breite und Höhe */
	int width, height;
	
	unsigned int ticks_alive;
	int frame, frames_count;
} enemy;

/* Erstelle neuen gegner */
enemy enemy_new(level_data *ld, spawn_data *sd) {
	enemy e = (enemy) {
		.pos = vector_new(ld->waypoints.points[0].x, ld->waypoints.points[0].y),
		.dir = vector_new(0, 0),
		.waypoint = 0,
		.sprite = sd->sprite, //qw_loadimage("assets/levels/level_1/enemies/blob.png"),
		.dead = 0,
		.speed = sd->speed,
		.health = sd->health,
		.width = sd->width * sd->scale,
		.height = sd->height * sd->scale,
		.ticks_alive = 0,
		.frame = 0,
		.frames_count = sd->frames
	};
	
	qw_image_setsrc(*(e.sprite), 0, 0, sd->width, sd->height);
	qw_image_setsize(*(e.sprite), e.width, e.height);
	qw_image_setcenter(e.sprite, e.width / 2, e.height / 2);

	return e;
}


/* Rendere gegner */
void enemy_draw(enemy *enemy) {
	if (enemy->dead) {
#ifdef DEBUG
		qw_color(255, 0, 0, 255);
		qw_write("DEAD", enemy->pos.x, enemy->pos.y);
#endif
		return;
	}
	
	//const float angle = vector_angle(enemy->pos, vector_add(enemy->pos, enemy->dir));
	const float angle = 0;

	qw_imagerotation(enemy->sprite, angle);
#ifdef DEBUG
	char text[128];
	sprintf(text, "%d", enemy->health);
	
	qw_color(90, 60, 230, 255);
	qw_write(text, enemy->pos.x, enemy->pos.y - 50);
#endif
	qw_image_srcpos(*enemy->sprite, 60 * enemy->frame, 0);

	qw_placeimage(*enemy->sprite, enemy->pos.x - enemy->width / 2, enemy->pos.y - (enemy->height / 3 * 2));
	
	/* bild horizontal spiegeln je nachdem in welche richtung `enemy` sich bewegt */
	if (enemy->dir.x < 0)
		qw_image_setflip(enemy->sprite, 1, 0);
	else if (enemy->dir.x > 0)
		qw_image_setflip(enemy->sprite, 0, 0);
		
	qw_drawimage(*enemy->sprite);
	
	if (enemy->ticks_alive % 10 == 0) {
		++enemy->frame;
	}

	if (enemy->frame >= enemy->frames_count)
		enemy->frame = 0;
	
	++(enemy->ticks_alive);
}

/* Bewege gegner `e` entlang dem pfad `waypoints` mit einer geschwindigkeit von `speed`  */
void enemy_move(enemy *e, ai_path waypoints) {
	/* ignoriere tote gegner */
	if (e->dead)
		return;
	
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
	if (distance < e->speed) {
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
	e->dir = vector_smult(e->dir, e->speed);
	
	/* Finally update the enemy's position */
	e->pos = vector_add(e->pos, e->dir);
}

#endif
