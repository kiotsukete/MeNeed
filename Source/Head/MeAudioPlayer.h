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
				std::wstring wPath = entry.path().wstring();//��ȡ���ַ����ļ�·��
				newMusic->filePath = UTF16_To_UTF8(wPath);//�����ַ��ļ�·��ת����UTF8
#else
				newMusic->filePath = entry.path().string();//ΪWindowsϵͳ��ֱ��ʹ��
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
	//instanceʵ������this���ʵ����ַ��ֵ�������ľ�̬��Ա����������ʵ��������ĳ�Ա����
	static MeAudioPlayer* instance;
	void PlayPrevious() {
		Mix_FreeMusic(playMusic);
		current = current->previous;
		playMusic = Mix_LoadMUS(current->filePath.c_str());
		if (!playMusic)
			std::cerr << "δ�ܼ�������" << Mix_GetError() << std::endl;
		Mix_PlayMusic(playMusic, 0);
	}
	void PlaySubsequent() {
		Mix_FreeMusic(playMusic);
		current = current->subsequent;
		playMusic = Mix_LoadMUS(current->filePath.c_str());
		if (!playMusic)
			std::cerr << "δ�ܼ�������" << Mix_GetError() << std::endl;
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
	//��̬��Ա�����У������޷�ֱ�ӷ��ʷǾ�̬��Ա������������Ҫʹ����ľ�̬��Ա���� instance ����ȡ��ǰ�����ָ��this���Ӷ����÷Ǿ�̬��Ա����
	static void MusicFinishedCallBack() {
		if (instance)
			instance->PlaySubsequent();
	}
};
//��ľ�̬��Ա�����ܰѾ�̬��Ա�ĳ�ʼ����������Ķ����У����ǿ���������ⲿͨ��ʹ�÷�Χ��������� :: ������������̬�����Ӷ��������г�ʼ��

MeAudioPlayer* MeAudioPlayer::instance = nullptr;
#endif