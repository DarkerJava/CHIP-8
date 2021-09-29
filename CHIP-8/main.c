#define _CRT_SECURE_NO_WARNINGS
#include "opexecute.h"
#include "system.h"
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <stdio.h>
#include <omp.h>

int main() {
	srand((unsigned)time(NULL));
	omp_set_num_threads(12);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * 28, 32 * 28, SDL_WINDOW_SHOWN);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	printf(SDL_GetError());

	//Fill the surface black
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0));
	//Update the surface
	SDL_UpdateWindowSurface(window);

	struct system* state = makeSystem();

	uint32_t refresh_rate = 16667; //60hz
	uint32_t clock_rate = 50; //1000 hz

	int32_t refresh_timer = 0;
	int32_t clock_timer = 0;

	clock_t time = clock();

	uint32_t white = SDL_MapRGB(surface->format, 0x0, 0xFFF, 0x0);
	uint32_t black = SDL_MapRGB(surface->format, 0x0, 0x0, 0x0);
	while (true) {
		int32_t difference = (clock() - time) * 1000000 / CLOCKS_PER_SEC;
		time = clock();
		refresh_timer -= difference;
		clock_timer -= difference;

		if (clock_timer <= 0) {
			fde(state);
			clock_timer += clock_rate;
		}
		bool halve = true;
		if (refresh_timer <= 0) {
			if (state->delay > 0) --(state->delay);
			if (state->draw && halve) {
				halve = !halve;
#pragma omp parallel for
				for (int i = 0; i < 64; i++) {
					for (int j = 0; j < 32; j++) {
						SDL_Rect rect = { i * 28, j * 28, 28, 28 };
						SDL_FillRect(surface, &rect, (state->display[i][j]) ? white : black);
					}
				}
				SDL_UpdateWindowSurface(window);
				state->draw = false;
			}
			refresh_timer = refresh_rate;
		}

		SDL_Event e;
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			break;
		} else if (e.type == SDL_KEYDOWN) {
			//refresh_timer -= 20000;
			//clock_timer -= 20000;
			switch (e.key.keysym.sym) {
				case SDLK_1: state->keystates[1] = true; break;
				case SDLK_2: state->keystates[2] = true; break;
				case SDLK_3: state->keystates[3] = true; break;
				case SDLK_4: state->keystates[0xC] = true; break;
				case SDLK_q: state->keystates[4] = true; break;
				case SDLK_w: state->keystates[5] = true; break;
				case SDLK_e: state->keystates[6] = true; break;
				case SDLK_r: state->keystates[0xD] = true; break;
				case SDLK_a: state->keystates[7] = true; break;
				case SDLK_s: state->keystates[8] = true; break;
				case SDLK_d: state->keystates[9] = true; break;
				case SDLK_f: state->keystates[0xE] = true; break;
				case SDLK_z: state->keystates[0xA] = true; break;
				case SDLK_x: state->keystates[0] = true; break;
				case SDLK_c: state->keystates[0xB] = true; break;
				case SDLK_v: state->keystates[0xF] = true; break;
				case SDLK_SPACE: printf("Enter frequency: "); int32_t freq; scanf("%d", &freq); clock_rate = 100000 / abs(freq); clock_timer = 0;  break;
			}
		} else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
				case SDLK_1: state->keystates[1] = false; break;
				case SDLK_2: state->keystates[2] = false; break;
				case SDLK_3: state->keystates[3] = false; break;
				case SDLK_4: state->keystates[0xC] = false; break;
				case SDLK_q: state->keystates[4] = false; break;
				case SDLK_w: state->keystates[5] = false; break;
				case SDLK_e: state->keystates[6] = false; break;
				case SDLK_r: state->keystates[0xD] = false; break;
				case SDLK_a: state->keystates[7] = false; break;
				case SDLK_s: state->keystates[8] = false; break;
				case SDLK_d: state->keystates[9] = false; break;
				case SDLK_f: state->keystates[0xE] = false; break;
				case SDLK_z: state->keystates[0xA] = false; break;
				case SDLK_x: state->keystates[0] = false; break;
				case SDLK_c: state->keystates[0xB] = false; break;
				case SDLK_v: state->keystates[0xF] = false; break;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	free(state);
	return 0;
}