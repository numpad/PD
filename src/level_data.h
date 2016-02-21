#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <stdlib.h>
#include "QW/qw.h"
#include "path.h"
#include "spawn_data.h"

/* Speichert daten die für ein level wichtig sind */
typedef struct {
	const char *path;
	char *bg_path;
	char *enemy_path;

	qw_image background;
	ai_path waypoints;
} level_data;

/* Initialisiert level daten */
level_data level_data_new(const char *lvl_path) {
	level_data ld = (level_data) {
		.path = lvl_path,
		.bg_path = malloc(sizeof(char) * strlen(lvl_path) + 15 /* 'background.png' */),
		.enemy_path = malloc(sizeof(char) * strlen(lvl_path) + 9 /* '/enemies/' */),
		.waypoints = load_waypoints(lvl_path)
	};
	
	/* Konstruiere bg_path */
	strcpy(ld.bg_path, lvl_path);
	strcat(ld.bg_path, "/background.png");
	ld.background = qw_loadimage(ld.bg_path);

	/* Konstriuere Pfad zum Ornder 'enemies' im momentanen level */
	strcpy(ld.enemy_path, lvl_path);
	strcat(ld.enemy_path, "/enemies/");
	
	return ld;
}

/* Gibt resourcen wieder frei */
void level_data_destroy(level_data *ld) {
	qw_destroyimage(ld->background);
	delete_waypoints(ld->waypoints);
	free(ld->bg_path);
	free(ld->enemy_path);
}

/* Rendert level hintergrund */
void level_data_drawbackground(level_data *ld) {
	qw_drawimage(ld->background);
}

#endif
