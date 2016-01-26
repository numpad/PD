#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
	
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
	}
	
	SDL_Window *window = SDL_CreateWindow("SDL funktioniert!",
										  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
										  800, 600, SDL_WINDOW_SHOWN);
	
	if (window == NULL) {
		printf("Window creation failed: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Delay(3000);
	
	SDL_DestroyWindow(window);

	return 0;
}

