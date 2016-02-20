#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "QW/qw.h"

SDL_Point point_new(int x, int y) {
	return (SDL_Point) {
		.x = x,
		.y = y
	};
}
float point_distance(SDL_Point a, SDL_Point b) {
	return sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

void export_points(const char *level_folder, SDL_Point *ps, int ps_count) {
	char data_path[128];
	strcpy(data_path, level_folder);
	strcat(data_path, "/path.csv");
	
	FILE *fp = fopen(data_path, "w+");
	fprintf(fp, "%d\n", ps_count);

	for (int i = 0; i < ps_count; ++i) {
		fprintf(fp, "%d,%d\n", ps[i].x, ps[i].y);
	}
	
	fclose(fp);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		puts("Use: level_edit <level_folder>");
		return 1;
	}

	qw_screen(800, 600, 0, "Projekt Defense");
	
	/* get background image from level folder */
	char lvl_bg[128] = {0};
	strcpy(lvl_bg, argv[1]);
	strcat(lvl_bg, "/background.png");
	qw_image background = qw_loadimage(lvl_bg);
	

	int max_points = 128,
	    points_i = 0;
	SDL_Point *points = malloc(sizeof(SDL_Point) * max_points);
	
	while (qw_running()) {
		qw_drawimage(background);
		
		qw_color(200, 100, 120, 255);
		qw_fillrect(qw_mousex - 2, qw_mousey - 2, 4, 4);
		
		/* waypoint placement */
		if (qw_mousedown(SDL_BUTTON_LEFT)) {
			if (points_i == 0) {
				points[points_i++] = point_new(qw_mousex, qw_mousey);
			} else {
				/* if not the first point placed: check if current point is to close to the last one */
				SDL_Point np = point_new(qw_mousex, qw_mousey);
				
				if (point_distance(points[points_i - 1], np) > 7.f)
					points[points_i++] = np;
			}
			
			/* we need more points? */
			if (points_i == max_points) {
				max_points += 128;
				points = realloc(points, sizeof(SDL_Point) * max_points);
			}
		}
		
		if (qw_keydown(QW_KEY(E))) {
			if (points_i > 1) {
				export_points(argv[1], points, points_i);
				puts("Exported!");
				qw_quit();
			}
		}

		qw_color(100, 120, 200, 255);
		SDL_RenderDrawLines(qw_renderer, points, points_i);

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	free(points);
	qw_destroyimage(background);
	
	return 0;
}

