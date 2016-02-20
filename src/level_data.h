#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include "QW/qw.h"
#include "path.h"
#include <stdlib.h>

typedef struct {
	const char *path;
	char *bg_path;
	
	qw_image background;
	ai_path waypoints;
} level_data;

/* Initialisiert level daten */
level_data level_data_new(const char *lvl_path) {
	level_data ld = (level_data) {
		.path = lvl_path,
		.bg_path = malloc(sizeof(char) * strlen(lvl_path) + 15 /* 'background.png' */),
		.waypoints = load_waypoints(lvl_path)
	};
	
	strcpy(ld.bg_path, lvl_path);
	strcat(ld.bg_path, "/background.png");
	
	ld.background = qw_loadimage(ld.bg_path);

	return ld;
}

/* Gibt resourcen wieder frei */
void level_data_destroy(level_data *ld) {
	qw_destroyimage(ld->background);
	delete_waypoints(ld->waypoints);
}

/* Rendert level hintergrund */
void level_data_drawbackground(level_data *ld) {
	qw_drawimage(ld->background);
}

#endif
