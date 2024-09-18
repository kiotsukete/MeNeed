#pragma once
#ifndef GAMESCENE_H
#define GAMESCENE_H
#include"MeScene.h"

extern MeSceneManager SceneManager;

class MeGameScene :public MeScene {
public:
	MeGameScene() = default;
	~MeGameScene() = default;
	void InitScene(SDL_Renderer* renderer) {
		SDL_Surface* G_bg_s = IMG_Load("./Resource/Texture/background/gamechu.png");
		backGround = SDL_CreateTextureFromSurface(renderer, G_bg_s);
		bg_rect = { 15,0,1280,720 };
		SDL_FreeSurface(G_bg_s);
	}
	void SceneEvent(SDL_Event& event) {
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
			SceneManager.SwitchScene(MeSceneManager::SceneType::S_START);
	}
	void SceneRender(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, backGround, &bg_rect, NULL);
	}
	void SceneDestory(SDL_Renderer* renderer) {
		SDL_DestroyTexture(backGround);
	}

private:
	SDL_Texture* backGround = nullptr;
	SDL_Rect bg_rect = { 0,0,0,0 };
};
#endif