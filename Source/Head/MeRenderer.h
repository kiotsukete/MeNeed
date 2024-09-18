#pragma once
#ifndef RENDER_H
#define RENDER_H
#include"MeSceneManager.h"
#include"MeAudioPlayer.h"

extern MeSceneManager scene_manager;
extern MeAudioPlayer Player;

class Render {
public:
	Render() = default;
	~Render() = default;

	void AllRender(SDL_Renderer* renderer) {
		SDL_RenderClear(renderer);

		scene_manager.RenderScene(renderer);
		Player.RenderMessage(renderer);

		SDL_RenderPresent(renderer);
	}

private:


};
#endif