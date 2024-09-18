#pragma once
#ifndef STARTSCENE_H
#define STARTSCENE_H
#include"MeScene.h"

extern MeSceneManager SceneManager;
extern const int winWidth;
extern const int winHeight;

class MeStartScene :public MeScene{
public:
	MeStartScene() = default;
	~MeStartScene() = default;
	void InitScene(SDL_Renderer* renderer) {
		TTF_Font* hello_font = TTF_OpenFont("./resource/font/NotoSerifCJKsc-VF.ttf", 80);
		std::u16string hello_O = u"welcome »¶Ó­ ¤è¤¦¤³¤½";
		std::vector<Uint16> hello_T(hello_O.begin(), hello_O.end());
		hello_T.push_back('\0');
		SDL_Surface* hello_s = TTF_RenderUNICODE_Blended(hello_font, hello_T.data(), SDL_Color{ 0,0,0,255 });
		helloOrigin = SDL_CreateTextureFromSurface(renderer, hello_s);
		hello = helloOrigin;
		hello_s = TTF_RenderUNICODE_Blended(hello_font, hello_T.data(), SDL_Color{ 255,255,255,255 });
		helloSelect = SDL_CreateTextureFromSurface(renderer, hello_s);

		he_rect = { (winWidth - hello_s->w) / 2,(winHeight - hello_s->h) / 2,hello_s->w,hello_s->h };
		SDL_FreeSurface(hello_s);
		TTF_CloseFont(hello_font);

	}
	void SceneEvent(SDL_Event& event) {
		mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		if (he_rect.x < mouseX && mouseX < he_rect.x + he_rect.w && he_rect.y < mouseY && mouseY < he_rect.y + he_rect.h) {
			hello = helloSelect;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				SceneManager.SwitchScene(MeSceneManager::SceneType::S_GAME);
			}
		}
		else
			hello = helloOrigin;
	}
	void SceneRender(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, hello, NULL, &he_rect);
	}
	void SceneDestory() {
		SDL_DestroyTexture(hello);
		SDL_DestroyTexture(helloOrigin);
		SDL_DestroyTexture(helloSelect);
	}
private:
	SDL_Texture* hello = nullptr;
	SDL_Texture* helloOrigin;
	SDL_Texture* helloSelect;
	SDL_Rect he_rect{ 0,0,0,0 };
	Uint32 mouseState = 0;
	int mouseX = 0, mouseY = 0;
};
#endif