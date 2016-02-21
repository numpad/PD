#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "level_data.h"
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
	
	float time;
	char name[64];
	while (fscanf(fp, "%g,%s\n", &time, name) != EOF) {
		int time_to_spawn = (int)(time * 60.f);
		
		/* qw_tick_count beginnt mit dem wert 1 */
		if ((int)time == 0)
			time_to_spawn = 1;
#ifdef DEBUG
		printf("Spawntime [%d]: %d '%s'\n", spawntime_i, time_to_spawn, name);
#endif
		spawntime[spawntime_i++] = time_to_spawn;
		
		/* Alloziert mehr Speicher für Spawnzeiten */
		if (spawntime_i == spawntime_max) {
			spawntime_max += 8;
			spawntime = realloc(spawntime, sizeof(int) * spawntime_max);
		}
	}
	
	fclose(fp);

	return spawntime_i;
}

void enemy_spawn(level_data *ld, spawn_data *sd, enemy *enemies, int *enemies_len) {
	enemies[(*enemies_len)++] = enemy_new(ld, sd);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		puts("use: ./main <level>");
		return 1;
	}

	qw_screen(800, 600, 0, "Projekt Defense");
	
	level_data current_level = level_data_new(argv[1]); /* z.B. "assets/levels/level_1" */
	
	spawn_data sd_blob = spawn_data_load(current_level.enemy_path, "trump");
	spawn_data sd_ship = spawn_data_load(current_level.enemy_path, "trump");
	
	int *spawn_times = malloc(sizeof(int) * 64);
	int spawn_times_count = load_spawns(current_level.path, spawn_times);
	int spawn_times_i = 0;
	
	int enemies_len = 0;
	enemy enemies[128];
	
	while (qw_running()) {
		level_data_drawbackground(&current_level);

		if (spawn_times_i < spawn_times_count && spawn_times[spawn_times_i] == qw_tick_count) {
			enemy_spawn(&current_level, &sd_blob, enemies, &enemies_len);
			++spawn_times_i;
		}

		if (qw_keydown(QW_KEY(LEFT))) {
		}

#ifdef DEBUG
		qw_color(200, 100, 120, 255);
		SDL_RenderDrawLines(qw_renderer, current_level.waypoints.points, current_level.waypoints.points_count);

		qw_color(0, 0, 255, 255);
		SDL_RenderDrawPoints(qw_renderer, current_level.waypoints.points, current_level.waypoints.points_count);
#endif
		
		for (int i = 0; i < enemies_len; ++i) {
			enemy_move(&enemies[i], current_level.waypoints);
			enemy_draw(&enemies[i]);
		}
		
		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	free(spawn_times);
	level_data_destroy(&current_level);	
	spawn_data_destroy(&sd_blob);
	spawn_data_destroy(&sd_ship);
	return 0;
}

