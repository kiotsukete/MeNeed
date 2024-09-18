#pragma once
#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include<Windows.h>

extern const int winWidth;
extern const int winHeight;

struct music {
	std::string filePath;
	music* previous = nullptr;
	music* subsequent = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = { 0,0,0,0 };
};

class MeAudioPlayer {
public:
	MeAudioPlayer() = default;
	~MeAudioPlayer() = default;
	void LoadMusicList(SDL_Renderer* renderer) {
		TTF_Font* music_font = TTF_OpenFont("./Resource/Font/NotoSerifCJKsc-VF.ttf", 60);
		for (const auto& entry : std::filesystem::directory_iterator("./Resource/MusicList")) {
			music* newMusic = new music;
			if (entry.path().extension() == ".mp3") {
#ifdef _WIN32
				std::wstring wPath = entry.path().wstring();//获取宽字符串文件路径
				newMusic->filePath = UTF16_To_UTF8(wPath);//将宽字符文件路径转换成UTF8
#else
				newMusic->filePath = entry.path().string();//为Windows系统下直接使用
#endif
				std::u16string name_o = entry.path().filename().replace_extension().u16string();
				std::vector<Uint16> name(name_o.begin(), name_o.end());
				name.push_back('\0');
				SDL_Surface* music_s = TTF_RenderUNICODE_Blended(music_font, name.data(), SDL_Color{ 0,0,0,255 });
				newMusic->texture = SDL_CreateTextureFromSurface(renderer, music_s);
				newMusic->rect = { (winWidth - music_s->w) / 2,0,music_s->w,music_s->h };
				SDL_FreeSurface(music_s);
				musicList.push_back(newMusic);
				if (!head) {
					head = newMusic; head->previous = newMusic;
					tail = newMusic; tail->subsequent = newMusic;
				}
				else {
					head->previous = newMusic; tail->subsequent = newMusic;
					newMusic->previous = tail; newMusic->subsequent = head;
					tail = newMusic;
				}
			}
		}
		current = head;
		playMusic = Mix_LoadMUS(current->filePath.c_str());
		TTF_CloseFont(music_font);

		instance = this;
		Mix_HookMusicFinished(MusicFinishedCallBack);
	}
	void SwitchPlay(SDL_Event event) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
			if (!Mix_PlayingMusic())
				Mix_PlayMusic(playMusic, 0);
			else {
				if (Mix_PausedMusic())
					Mix_ResumeMusic();
				else
					Mix_PauseMusic();
			}
		}
		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_LCTRL] && keyState[SDL_SCANCODE_LEFT])
			PlayPrevious();
		else if (keyState[SDL_SCANCODE_LCTRL] && keyState[SDL_SCANCODE_RIGHT])
			PlaySubsequent();

	}
	void RenderMessage(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, current->texture, NULL, &current->rect);
	}

	void ListDestroy() {
		for (const auto music : musicList) {
			SDL_DestroyTexture(music->texture);
			delete music;
		}
	}
private:
	std::vector<music*> musicList;
	music* current = nullptr;
	music* head = nullptr;
	music* tail = nullptr;
	Mix_Music* playMusic = nullptr;
	//instance实例，将this类的实例地址赋值给这个类的静态成员并由这个类的实例调用类的成员函数
	static MeAudioPlayer* instance;
	void PlayPrevious() {
		Mix_FreeMusic(playMusic);
		current = current->previous;
		playMusic = Mix_LoadMUS(current->filePath.c_str());
		if (!playMusic)
			std::cerr << "未能加载音乐" << Mix_GetError() << std::endl;
		Mix_PlayMusic(playMusic, 0);
	}
	void PlaySubsequent() {
		Mix_FreeMusic(playMusic);
		current = current->subsequent;
		playMusic = Mix_LoadMUS(current->filePath.c_str());
		if (!playMusic)
			std::cerr << "未能加载音乐" << Mix_GetError() << std::endl;
		Mix_PlayMusic(playMusic, 0);
	}
	std::string UTF16_To_UTF8(const std::wstring& wPath) {
		if (wPath.empty())
			return std::string();
		int needSize = WideCharToMultiByte(CP_UTF8, 0, &wPath[0], (int)wPath.size(), NULL, 0, NULL, NULL);
		std::string Path(needSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wPath[0], (int)wPath.size(), &Path[0], needSize, NULL, NULL);
		return Path;
	}
	//静态成员函数中，由于无法直接访问非静态成员，所以我们需要使用类的静态成员变量 instance 来获取当前对象的指针this，从而调用非静态成员函数
	static void MusicFinishedCallBack() {
		if (instance)
			instance->PlaySubsequent();
	}
};
//类的静态成员：不能把静态成员的初始化放置在类的定义中，但是可以在类的外部通过使用范围解析运算符 :: 来重新声明静态变量从而对它进行初始化

MeAudioPlayer* MeAudioPlayer::instance = nullptr;
#endif