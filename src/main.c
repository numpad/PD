#include <stdio.h>
#include <stdlib.h>
#include "qw.h"
#include "path.h"

typedef struct {
	int x, y;
	int waypoint;
} enemy;

enemy enemy_new(ai_path wps) {
	return (enemy) {
		.x = wps.points[0].x,
		.y = wps.points[0].y,
		.waypoint = 0
	};
}

void enemy_draw(enemy *enemy) {
	qw_color(120, 100, 220, 255);
	qw_drawrect(enemy->x - 15, enemy->y - 15, 31, 31);
	qw_drawline(enemy->x - 15, enemy->y - 15, enemy->x + 15, enemy->y + 15);
	qw_drawline(enemy->x + 15, enemy->y - 15, enemy->x - 15, enemy->y + 15);
}

void enemy_move(enemy *enemy, ai_path wps) {
	if (enemy->waypoint < wps.points_count - 1) {
		enemy->waypoint += 1;
		enemy->x = wps.points[enemy->waypoint].x;
		enemy->y = wps.points[enemy->waypoint].y;
	}
}

int main(int argc, char *argv[]) {
	qw_screen(800, 600, 0, "Projekt Defense");
	
	/* lädt hintergrund und wegpunkte TODO: level struct */
	qw_image background = qw_loadimage("assets/levels/level_1/background.png");
	ai_path waypoints = load_waypoints("assets/levels/level_1");
	
	enemy e = enemy_new(waypoints);

	while (qw_running()) {
		qw_drawimage(background);
		
		qw_color(200, 100, 120, 255);
		SDL_RenderDrawLines(qw_renderer, waypoints.points, waypoints.points_count);
		
		if (qw_keydown(QW_KEY(RIGHT))) {
			enemy_move(&e, waypoints);
		}
		
		enemy_draw(&e);

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	delete_waypoints(waypoints);
	qw_destroyimage(background);
	
	return 0;
}

