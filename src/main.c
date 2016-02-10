#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "QW/qw.h"
#include "path.h"
#include "enemy.h"
#include "vector.h"

int load_spawns(const char *folder, int *spawntime) {
	/* Lädt /path.csv aus dem level ordner */
	char file[128];
	strcpy(file, folder);
	strcat(file, "/spawn.csv");
	
	/* Öffnet datei und prüft auf erfolg */
	FILE *fp = fopen(file, "r");
	if (!fp) {
		printf("Laden der Datei '%s' fehlgeschlagen!\n", file);
		return 0;
	}
	
	char a[64], b[64];
	fscanf(fp, "%s,%s\n", a, b);
	
	int spawntime_i = 0,
	    spawntime_max = 64;

	int time;
	char name[64];
	while (fscanf(fp, "%d,%s\n", &time, name) != EOF) {
		spawntime[spawntime_i++] = time;
		if (spawntime_i == spawntime_max) {
			spawntime_max += 8;
			spawntime = realloc(spawntime, sizeof(int) * spawntime_max);
		}
	}
	
	fclose(fp);

	return spawntime_i;
}

int main(int argc, char *argv[]) {
	qw_screen(800, 600, 0, "Projekt Defense");
	
	/* lädt hintergrund und wegpunkte TODO: level struct */
	qw_image background = qw_loadimage("assets/levels/level_1/background.png");
	ai_path waypoints = load_waypoints("assets/levels/level_1");
	
	int *spawn_times = malloc(sizeof(int) * 64);
	int spawn_times_count = load_spawns("assets/levels/level_1", spawn_times);
	int spawn_times_i = 0;

	int enemies_len = 0;
	enemy enemies[128];
	
	while (qw_running()) {
		qw_drawimage(background);
		
		if (spawn_times_i < spawn_times_count && spawn_times[spawn_times_i] == qw_tick_count) {
			enemies[enemies_len++] = enemy_new(waypoints);
			++spawn_times_i;
		}

		if (qw_keydown(QW_KEY(LEFT))) {
		}

#ifdef DEBUG
		qw_color(200, 100, 120, 255);
		SDL_RenderDrawLines(qw_renderer, waypoints.points, waypoints.points_count);

		qw_color(0, 0, 255, 255);
		SDL_RenderDrawPoints(qw_renderer, waypoints.points, waypoints.points_count);
#endif

		for (int i = 0; i < enemies_len; ++i) {
			enemy_move(&enemies[i], waypoints, 1.5f);
			enemy_draw(&enemies[i]);
		}

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	free(spawn_times);
	delete_waypoints(waypoints);
	qw_destroyimage(background);
	
	return 0;
}

