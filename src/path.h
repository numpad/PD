#ifndef PATH_H
#define PATH_H

/* Weg für AI */
typedef struct {
	SDL_Point *points;
	int points_count;
} ai_path;

/* Gibt alle resourcen von ai_path frei */
void delete_waypoints(ai_path ap) {
	free(ap.points);
	ap.points_count = 0;
}

/* Lädt wegpunkte für Gegner, setzt wert von count = anzahl von punkten */
ai_path load_waypoints(const char *level_folder) {
	int count;

	/* Lädt /path.csv aus dem level ordner */
	char file[128];
	strcpy(file, level_folder);
	strcat(file, "/path.csv");
	
	/* Öffnet datei und prüft auf erfolg */
	FILE *fp = fopen(file, "r");
	if (!fp) {
		printf("Failed to open '%s'\n", file);
		return (ai_path) {
			.points = NULL,
			.points_count = -1
		};
	}
	
	/* 1. Zeile der datei: anzahl von punkten */
	fscanf(fp, "%d\n", &count);

	/* Reserviert genügend speicherplatz für alle punkte (keine überprüfung ob anzahl korrekt ist!) */
	ai_path path = (ai_path) {
		.points_count = count,
		.points = malloc(sizeof(SDL_Point) * count)
	};

	/* Lädt x,y koordinaten */
	for (int i = 0; i < count; ++i) {
		int xp, yp;
		fscanf(fp, "%d,%d\n", &xp, &yp);
		path.points[i] = (SDL_Point){xp, yp};
	}

	fclose(fp);

	return path;
}

#endif
