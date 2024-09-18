#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include"MeScene.h"

extern MeScene* StartScene;
extern MeScene* GameScene;

class MeSceneManager {

public:
	MeSceneManager() = default;
	~MeSceneManager() = default;

	enum class SceneType {
		S_START,
		S_GAME
	};
	void SetCurrentScene(MeScene* scene) {
		current_scene = scene;
	}
	void SwitchScene(SceneType type) {
		if (type == SceneType::S_START)
			current_scene = StartScene;
		else if (type == SceneType::S_GAME)
			current_scene = GameScene;
	}
	void SceneEvent(SDL_Event& event) {
		current_scene->SceneEvent(event);
	}
	void RenderScene(SDL_Renderer* renderer) {
		current_scene->SceneRender(renderer);
	}
private:
	MeScene* current_scene = nullptr;
};
#endif