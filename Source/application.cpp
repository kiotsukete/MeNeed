#include<iostream>

#include"MeSceneManager.h"
#include"MeStartScene.h"
#include"MeGameScene.h"
#include"MeAudioPlayer.h"
#include"MeUI.h"
#include"MeRender.h"

const int winWidth = 1280;
const int winHeight = 720;

int mouseX;
int mouseY;
Uint32 mouseState;

void MeAllInit();
void MeAllDestroy(SDL_Window* window, SDL_Renderer* renderer);

MeUI setUI;
MeSceneManager SceneManager;
MeScene* StartScene = nullptr;
MeScene* GameScene = nullptr;
MeSceneManager scenemanager;
MeAudioPlayer AudioPlayer;

int main(int argc, char* argv[]) {
	MeAllInit();
	bool WindowClose = false;
	SDL_Window* Window = SDL_CreateWindow("MeNeed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_BORDERLESS);
	SDL_Renderer* renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	MeRender render;
	StartScene = new MeStartScene();
	GameScene = new MeGameScene();

	setUI.loadUI("./Resource/Texture/UIwidget/UI.png" ,"./Resource/Texture/UIwidget/UI_Position.txt" ,renderer);
	setUI.setBackground("./Resource/Texture/background/background.png", renderer, SDL_Rect{ 610,217,1280,720 });
	StartScene->InitScene(renderer);
	GameScene->InitScene(renderer);
	SceneManager.SetCurrentScene(StartScene);
	AudioPlayer.LoadMusicList(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Event event;
	while (!WindowClose){
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				WindowClose = true;
			setUI.eventUI(event);
			SceneManager.SceneEvent(event);
			AudioPlayer.SwitchPlay(event);
		}

		render.AllRender(renderer);
	}

	MeAllDestroy(Window,renderer);
	return 0;
}
void MeAllInit() {
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
void MeAllDestroy(SDL_Window* window, SDL_Renderer* renderer) {
	StartScene->SceneDestory(renderer);
	GameScene->SceneDestory(renderer);
	AudioPlayer.ListDestroy();
	delete StartScene, GameScene;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}