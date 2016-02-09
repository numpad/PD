#define DEBUG
#undef DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "QW/qw.h"
#include "path.h"
#include "enemy.h"
#include "vector.h"

int main(int argc, char *argv[]) {
	qw_screen(800, 600, 0, "Projekt Defense");
	
	/* lädt hintergrund und wegpunkte TODO: level struct */
	qw_image background = qw_loadimage("assets/levels/level_1/background.png");
	ai_path waypoints = load_waypoints("assets/levels/level_1");

	int enemies_len = 0;
	enemy enemies[128];
	
	unsigned long long ticks = 0;
	while (++ticks, qw_running()) {
		qw_drawimage(background);
		
		if (qw_tick_count == 1 || ticks == 120) {
			enemies[enemies_len++] = enemy_new(waypoints);
			ticks = 0;
		}

		if (qw_keydown(QW_KEY(LEFT))) {
		}

		if (qw_keydown(QW_KEY(UP))) {
			qw_color(200, 100, 120, 255);
			SDL_RenderDrawLines(qw_renderer, waypoints.points, waypoints.points_count);
		}

		if (qw_keydown(QW_KEY(DOWN))) {
			qw_color(0, 0, 255, 255);
			SDL_RenderDrawPoints(qw_renderer, waypoints.points, waypoints.points_count);
		}
		
		for (int i = 0; i < enemies_len; ++i) {
			move_ai(&enemies[i], waypoints, 1.5f);
			enemy_draw(&enemies[i]);
		}

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	delete_waypoints(waypoints);
	qw_destroyimage(background);
	
	return 0;
}

