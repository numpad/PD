#include <stdio.h>
#include <stdlib.h>
#include "qw.h"

int main(int argc, char *argv[]) {
	
	qw_screen(800, 600, 0, "Projekt Defense");

	while (qw_running()) {
		qw_fill(100, 120, 200);
		qw_color(200, 100, 120, 255);
		qw_fillrect(qw_mousex - 20, qw_mousey - 20, 40, 40);
		
		qw_redraw();
		
		if (qw_keydown(QW_KEY(ESCAPE))) {
			qw_quit();
		}
	}

	return 0;
}

