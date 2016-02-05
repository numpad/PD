#include <stdio.h>
#include <stdlib.h>
#include "qw.h"

int main(int argc, char *argv[]) {

	qw_screen(800, 600, 0, "Projekt Defense");
	
	qw_image background = qw_loadimage("assets/bg.png");

	while (qw_running()) {
		qw_fill(100, 120, 200);
		qw_color(200, 100, 120, 255);
		qw_fillrect(qw_mousex - 20, qw_mousey - 20, 40, 40);
		
		qw_drawimage(background);
		
		if (qw_mousedown(SDL_BUTTON_LEFT))
			qw_placeimage(background, qw_mousex, qw_mousey);

		if (qw_keydown(QW_KEY(RIGHT)))
			qw_moveimage(background, 3, 0);
		if (qw_keydown(QW_KEY(LEFT)))
			qw_moveimage(background, -3, 0);

		qw_redraw();
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}
	
	qw_destroyimage(background);
	
	return 0;
}

