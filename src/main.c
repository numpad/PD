#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "level_data.h"
#include "QW/qw.h"
#include "path.h"
#include "enemy.h"
#include "vector.h"
#include "tower.h"

int load_spawns(const char *folder, int *spawntime, int *spawnindex) {
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
	int enemy_index;
	while (fscanf(fp, "%g,%d\n", &time, &enemy_index) != EOF) {
		int time_to_spawn = (int)(time * 60.f);
		
		/* qw_tick_count beginnt mit dem wert 1 */
		if ((int)time == 0)
			time_to_spawn = 1;
#ifdef DEBUG
		printf("Spawntime [%d]: %d index '%d'\n", spawntime_i, time_to_spawn, enemy_index);
#endif
		spawnindex[spawntime_i] = enemy_index;
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

spawn_data *spawn_data_loadall(level_data *ld, int *count) {
	spawn_data *spawnables = malloc(sizeof(spawn_data) * 3);
	
	/* benutze level pfad und lade ./load.enemies */
	char enemies_load_path[128];
	strcpy(enemies_load_path, ld->path);
	strcat(enemies_load_path, "/load.enemies");

	FILE *fp = fopen(enemies_load_path, "r");
	if (fp == NULL) {
		printf("Laden der Datei '%s' fehlgeschlagen!\n", enemies_load_path);
		return NULL;
	}
	
	
	char name[64];
	while (fscanf(fp, "%s\n", name) != EOF) {
		spawnables[*count] = spawn_data_load(ld->enemy_path, name);
		++(*count);
	}

	fclose(fp);

	return spawnables;
}

void spawn_data_destroyall(spawn_data *sd, int count) {
	for (int i = 0; i < count; ++i) {
		spawn_data_destroy(&sd[i]);
	}
	free(sd);
}

float distance_to_closest_tower(int x, int y, tower *towers, int towers_count) {
	return 1000;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		puts("use: ./main <level>");
		return 1;
	}
	
	/* Fenster erstellen */
	qw_screen(800, 600, 0, "Projekt Defense");
	
	
	/* Level laden */
	level_data current_level = level_data_new(argv[1]); /* z.B. "assets/levels/level_1" */
	
	/* Gegner und spawnzeiten laden */
	int enemies_count = 0;
	spawn_data *sd_all = spawn_data_loadall(&current_level, &enemies_count);
	int *spawn_index = malloc(sizeof(int) * 64);

	int *spawn_times = malloc(sizeof(int) * 64);
	int spawn_times_count = load_spawns(current_level.path, spawn_times, spawn_index);
	int spawn_times_i = 0;
	
	int enemies_len = 0;
	enemy enemies[128];
	
	int towers_count = 0;
	tower towers[64];
	
	/* Bilder für türme laden */
	qw_image tower_1_image = qw_loadimage("assets/towers/tower_1.png");
	qw_image tower_2_image = qw_loadimage("assets/towers/tower_2.png");
	tower_data tower_1 = (tower_data) {&tower_1_image, 25, 3, 180., 50};
	tower_data tower_2 = (tower_data) {&tower_2_image, 60, 1, 120., 40};
	
	qw_image game_logo = qw_loadimage("assets/logo.png");
	
	int money = 200;
	int gamestate = 0;
	while (qw_running()) {
		switch (gamestate) {
			case 0: { /* gamestate: menu */
				qw_fill(100, 130, 230);
				qw_drawimage(game_logo);
				qw_color(0, 0, 0, 255);
				qw_write("Druecke <Enter> zum starten", qw_width / 2 - 100, qw_height - 30);
#ifdef DEBUG
				qw_write("Project Defense - DEBUG VERSION 1.0", 10, 10);
#endif
				if (qw_keydown(QW_KEY(RETURN))) {
					gamestate = 1;
					qw_resetticks();
				}

				break;
			}
			case 1: { /* gamestate: game */
				level_data_drawbackground(&current_level);
			
				/* Gegner spawn logik */
				if (spawn_times_i < spawn_times_count && spawn_times[spawn_times_i] == qw_tick_count) {
					enemy_spawn(&current_level, &sd_all[spawn_index[spawn_times_i]], enemies, &enemies_len);
					++spawn_times_i;
				}
				
				if (qw_mousedown(SDL_BUTTON_LEFT)) {
					int xa = qw_mousex;
					int ya = qw_mousey;
					if (distance_to_closest_tower(xa, ya, towers, towers_count) > 100) {
						if (money >= tower_1.price) {
							towers[towers_count++] = tower_place(qw_mousex, qw_mousey, tower_1);
							money -= tower_1.price;
						}
					}
				}
				
				if (qw_mousedown(SDL_BUTTON_RIGHT)) {
					int xa = qw_mousex;
					int ya = qw_mousey;
					if (distance_to_closest_tower(xa, ya, towers, towers_count) > 100) {
						if (money >= tower_2.price) {
							towers[towers_count++] = tower_place(qw_mousex, qw_mousey, tower_2);
							money -= tower_2.price;
						}
					}
				}
				
				for (int i = 0; i < towers_count; ++i) {
					int has_killed = tower_attack(&towers[i], enemies, enemies_len);
					if (has_killed)
						money += 25;
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
				
				for (int i = 0; i < towers_count; ++i) {
					tower_draw(&towers[i]);
				}
				
				/* Geld in die obere linke ecke schreiben */
				qw_color(210, 150, 90, 255);
				
				char money_string[16];
				sprintf(money_string, "Geld: $%d", money);
				qw_write(money_string, 10, 10);

				break;
			}
		}

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}

	/* Gebe ressourcen wieder frei */
	free(spawn_times);
	free(spawn_index);
	spawn_data_destroyall(sd_all, enemies_count);
	level_data_destroy(&current_level);
	
	return 0;
}

