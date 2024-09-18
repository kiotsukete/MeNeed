#pragma once
#ifndef SCENE_H
#define SCENE_H
#include<iostream>
#include<vector>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

class MeScene{
public:
	MeScene() = default;
	~MeScene() = default;
	virtual void InitScene(SDL_Renderer* renderer) {}
	virtual void SceneEvent(SDL_Event& event) {}
	virtual void SceneRender(SDL_Renderer* renderer) {}
	virtual void SceneDestory(SDL_Renderer* renderer) {}
private:

};
#endif
