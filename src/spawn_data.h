#ifndef SPAWN_DATA_H
#define SPAWN_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include "QW/qw.h"

/* Speichert daten zum spawnen von gegnern */
typedef struct {
	char *name;
	qw_image *sprite;
	int health;
	float speed;
} spawn_data;

/* Lädt daten zum spawnen eines bestimmten gegners */
spawn_data spawn_data_load(const char *level_folder, const char *name) {
	/* Pfad zur .dat datei */
	char enemy_dat[128];
	strcpy(enemy_dat, level_folder);
	strcat(enemy_dat, name);
	strcat(enemy_dat, ".dat");
	
	/* Pfad zum sprite */
	char enemy_png[128];
	strcpy(enemy_png, level_folder);
	strcat(enemy_png, name);
	strcat(enemy_png, ".png");
	
	/* Öffne .dat datei */
	FILE *fp = fopen(enemy_dat, "r");
	if (fp == NULL) {
		printf("Failed loading '%s' from '%s'!\n", name, level_folder);
	}
	
	/* Speicher für spawn informationen */
	spawn_data sd;
	
	/* Lese Daten aus einem Key/Value dict im format "<key> = <val>", whitespace muss beachtet werden*/
	char key[32];
	char val[32];
	while (fscanf(fp, "%s = %s\n", key, val) != EOF) {
		/* Suche bestimmten key um wert zuzuweisen */
		if (!strcmp(key, "health")) {
			sscanf(val, "%d", &sd.health);
		} else if (!strcmp(key, "speed")) {
			sscanf(val, "%g", &sd.speed);
		} else if (!strcmp(key, "name")) {
			sd.name = malloc(strlen(val) + 1);
			strcpy(sd.name, val);
		}
	}
	
	fclose(fp);
	
	/* Reserviere speicherplatz für sprite und lade ihn */
	sd.sprite = malloc(sizeof(qw_image));
	*sd.sprite = qw_loadimage(enemy_png);
	
	return sd;
}

/* Gibt speicher frei */
void spawn_data_destroy(spawn_data *sd) {
	free(sd->sprite);
	free(sd->name);
}

#endif
