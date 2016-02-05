#include <stdio.h>
#include <stdlib.h>
#include "qw.h"

/* Lädt wegpunkte für Gegner, setzt wert von count = anzahl von punkten */
SDL_Point *load_waypoints(const char *level_folder, int *count) {
	/* Lädt /path.csv aus dem level ordner */
	char file[128];
	strcpy(file, level_folder);
	strcat(file, "/path.csv");
	
	/* Öffnet datei und prüft auf erfolg */
	FILE *fp = fopen(file, "r");
	if (!fp) {
		printf("Failed to open '%s'\n", file);
		return NULL;
	}
	
	/* 1. Zeile der datei: anzahl von punkten */
	fscanf(fp, "%d\n", count);
	
	/* Reserviert genügend speicherplatz für alle punkte (keine überprüfung ob anzahl korrekt ist!) */
	SDL_Point *points = malloc(sizeof(SDL_Point) * *count);

	/* Lädt x,y koordinaten */
	for (int i = 0; i < *count; ++i) {
		int xp, yp;
		fscanf(fp, "%d,%d\n", &xp, &yp);
		points[i] = (SDL_Point){xp, yp};
	}

	fclose(fp);

	return points;
}

int main(int argc, char *argv[]) {
	qw_screen(800, 600, 0, "Projekt Defense");
	
	/* lädt hintergrund und wegpunkte TODO: level struct */
	qw_image background = qw_loadimage("assets/levels/level_1/background.png");
	int waypoints_count;
	SDL_Point *waypoints = load_waypoints("assets/levels/level_1", &waypoints_count);
	
	while (qw_running()) {
		qw_drawimage(background);
		
		qw_color(200, 100, 120, 255);
		SDL_RenderDrawLines(qw_renderer, waypoints, waypoints_count);
		
		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	free(waypoints);
	qw_destroyimage(background);
	
	return 0;
}

