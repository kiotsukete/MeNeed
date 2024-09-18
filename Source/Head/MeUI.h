#pragma once
#ifndef MEUI_H
#define MEUI_H
#include"SDL.h"
#include"SDL_image.h"
#include"SDL_ttf.h"
#include<iostream>
#include<fstream>
#include<string>
#include<unordered_map>

extern int mouseX;
extern int mouseY;
extern Uint32 mouseState;

struct UI_Message {
	SDL_Rect clipRect{ 0,0,0,0 };
	SDL_Rect destinationRect{ 0,0,0,0 };
	bool render = false;
	std::string onName;
};

std::unordered_map<std::string, UI_Message> UImessage;
std::unordered_map<std::string, UI_Message> onUImessage;

class MeUI {
public:
	MeUI() = default;
	~MeUI() = default;

	void loadUI(const char* PNGpath, const char* configurationPath, SDL_Renderer* renderer) {
		SDL_Surface* UIsurface = IMG_Load(PNGpath);
		UItexture = SDL_CreateTextureFromSurface(renderer, UIsurface);
		std::string name, onName;
		int x, y, w, h, Dx, Dy, onX ,onY ,onW,onH, onDx, onDy;
		std::ifstream file(configurationPath);
		if (file.is_open()) {
			while (file >> name >> x >> y >> w >> h >> Dx >> Dy
				 >> onName >> onX >> onY >> onW >> onH >> onDx >> onDy) {
				UI_Message message, onMessage;
				message.clipRect = { x,y,w,h };
				message.destinationRect = { Dx,Dy,message.clipRect.w,message.clipRect.h };
				message.onName = onName;

				onMessage.clipRect = { onX,onY,onW,onH };
				onMessage.destinationRect = { onDx,onDy,onMessage.clipRect.w,onMessage.clipRect.h };
				UImessage[name] = message;
				onUImessage[onName] = onMessage;

			}
			file.close();
		}
		else
			std::cerr << "Failed to open configuration file" << std::endl;

		SDL_FreeSurface(UIsurface);

	}
	void setBackground(const char* path, SDL_Renderer* renderer, const SDL_Rect& clip) {
		SDL_Surface* bg_surface = IMG_Load(path);
		bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
		bg_clipRect = clip;

	}
	void eventUI(const SDL_Event& event) {
		mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		for (auto& iterators : UImessage) {
			auto& value = iterators.second;
			if (value.destinationRect.x < mouseX && mouseX < value.destinationRect.x + value.destinationRect.w
				&& value.destinationRect.y < mouseY && mouseY < value.destinationRect.y + value.destinationRect.h) {
				value.render = true;
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
					onUImessage[value.onName].render = true;
			}
			else {
				value.render = false;
				onUImessage[value.onName].render = false;
			}
		}
	}
	void renderUI(SDL_Renderer* renderer) {
		if (bg_texture)
			SDL_RenderCopy(renderer, bg_texture, &bg_clipRect, NULL);
		for (const auto& iterators : UImessage) {
			const auto& value = iterators.second;
			if (value.render)
				SDL_RenderCopy(renderer, UItexture, &value.clipRect, &value.destinationRect);
			if(onUImessage[value.onName].render)
				SDL_RenderCopy(renderer, UItexture, &onUImessage[value.onName].clipRect, &onUImessage[value.onName].destinationRect);
		}

	}


private:
	SDL_Texture* UItexture = nullptr;
	SDL_Texture* bg_texture = nullptr;
	SDL_Rect bg_clipRect{ 0,0,0,0 };
};
#endif