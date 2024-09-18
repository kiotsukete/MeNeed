#pragma once
#ifndef MERENDER_H
#define MERENDER_H
#include"SDL.h"
#include"MeUI.h"

extern MeUI setUI;
extern MeSceneManager SceneManager;
extern MeAudioPlayer AudioPlayer;

class MeRender {
public:
	void AllRender(SDL_Renderer* renderer) {
		SDL_RenderClear(renderer);
		
		setUI.renderUI(renderer);
		SceneManager.RenderScene(renderer);
		AudioPlayer.RenderMessage(renderer);

		SDL_RenderPresent(renderer);
	}

private:

};
#endif