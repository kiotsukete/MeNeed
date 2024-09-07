#include<iostream>
#include"SDL.h"
#include"SDL_image.h"
#include"SDL_mixer.h"
#include"SDL_ttf.h"

const int winWidth = 1280;
const int winHeight = 720;

void All_init();
void All_destroy();

int main(int argc, char* argv[]) {
	All_init();
	bool WindowClose = false;
	SDL_Window* Window = SDL_CreateWindow("MeNeed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_BORDERLESS);
	SDL_Renderer* renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Event event;
	while (!WindowClose){
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				WindowClose = true;
		}
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}

	All_destroy();
	return 0;
}
void All_init() {
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		std::cerr << "Failed to Init SDL" << SDL_GetError() << std::endl;
		exit(-1);
	}
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_WEBP)) {
		std::cerr << "Failed to Init SDL_image" << IMG_GetError() << std::endl;
		exit(-1);
	}
#ifdef _WIN32
	SDL_AudioInit("dsound");
#endif
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096)) {
		std::cerr << "Mix Failed to open audio" << Mix_GetError() << std::endl;
		exit(-1);
	}
	if (!Mix_Init(MIX_INIT_FLAC | MIX_INIT_WAVPACK | MIX_INIT_MP3)) {
		std::cerr << "Failed to Init SDL_mixer" << Mix_GetError() << std::endl;
		exit(-1);
	}
	if (TTF_Init()) {
		std::cerr << "Failed to Init SDL_ttf" << TTF_GetError() << std::endl;
		exit(-1);
	}
}
void All_destroy() {


	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}