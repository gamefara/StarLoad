#pragma once
#include "../Task.h"
#include "IResourceManager.h"

const int m_nGraphicsCount = static_cast<int>(ResourceImage::IMAGE_UNKNOWN);
const int m_nSoundsCount = static_cast<int>(ResourceSound::SOUND_UNKNOWN);
const int m_nBGMCount = static_cast<int>(ResourceSound::SOUND_BGM_STAGE) + 1;
const int m_nSECount = static_cast<int>(ResourceSound::SOUND_SE_BLOCK_COLLIDE) - m_nBGMCount + 1;

class Resource : public IResourceManager{
public:
	Resource(){}
	~Resource();

	int IsLoadGraphics() override;
	int IsLoadSounds() override;

	int GetGraphicsHandle(ResourceImage eImage) override;
	int GetSoundsHandle(ResourceSound eSound) override;
	int GetGraphicsLoadingCount() override { return m_nGraphicLoadingCount; }
	int GetSoundsLoadingCount() override { return m_nSoundsLoadingCount; }

	void SetSoundsBGMVolume(int nPercent){
		if(nPercent < 0) nPercent = 0;
		else if(nPercent > 100) nPercent = 100;
		int nVolume = static_cast<int>(255 * (nPercent / 100.0f));
		for(int i = 0; i < m_nBGMCount; i++) ChangeVolumeSoundMem(nVolume, m_anSoundsHandleList[i]);
	}
	void SetSoundsSEVolume(int nPercent){
		if(nPercent < 0) nPercent = 0;
		else if(nPercent > 100) nPercent = 100;
		int nVolume = static_cast<int>(255 * (nPercent / 100.0f));
		for(int i = m_nBGMCount; i < m_nSoundsCount; i++) ChangeVolumeSoundMem(nVolume, m_anSoundsHandleList[i]);
	}
private:
	int m_anGraphicsHandleList[m_nGraphicsCount];
	int m_anSoundsHandleList[m_nSoundsCount];
	int m_nGraphicLoadingCount = 0;
	int m_nSoundsLoadingCount = 0;

	const char* m_asGraphicsList[m_nGraphicsCount] = {
		"Resource/Graphics/load_title.png",
		"Resource/Graphics/loading.png",
		"Resource/Graphics/gauge_empty.png",
		"Resource/Graphics/gauge_fill.png",
		"Resource/Graphics/gamestart.png",
		"Resource/Graphics/cursor.png",
		"Resource/Graphics/practice.png",
		"Resource/Graphics/practice_description.png",
		"Resource/Graphics/result.png",
		"Resource/Graphics/result_description.png",
		"Resource/Graphics/config.png",
		"Resource/Graphics/config_description.png",
		"Resource/Graphics/manual.png",
		"Resource/Graphics/manual_description.png",
		"Resource/Graphics/exit.png",
		"Resource/Graphics/exit_description.png",
		"Resource/Graphics/begginer.png",
		"Resource/Graphics/standard.png",
		"Resource/Graphics/hard.png",
		"Resource/Graphics/level_description.png",
		"Resource/Graphics/result_display_description.png",
		"Resource/Graphics/bgm.png",
		"Resource/Graphics/se.png",
		"Resource/Graphics/config_back.png",
		"Resource/Graphics/config_display_description.png",
		"Resource/Graphics/manual_page1.png",
		"Resource/Graphics/manual_page2.png",
		"Resource/Graphics/manual_page3.png",
		"Resource/Graphics/manual_page4.png",
		"Resource/Graphics/manual_display_description.png",
		"Resource/Graphics/background.png",
		"Resource/Graphics/magic1.png",
		"Resource/Graphics/magic2.png",
		"Resource/Graphics/magic3.png",
		"Resource/Graphics/magic4.png",
		"Resource/Graphics/magic5.png",
		"Resource/Graphics/star1.png",
		"Resource/Graphics/star2.png",
		"Resource/Graphics/star3.png",
		"Resource/Graphics/star4.png"
	};
	const char* m_asSoundsList[m_nSoundsCount] = {
		"Resource/Sounds/BGM/title.ogg",
		"Resource/Sounds/BGM/stage.ogg",
		"Resource/Sounds/SE/ok.ogg",
		"Resource/Sounds/SE/cancel.ogg",
		"Resource/Sounds/SE/move.ogg",
		"Resource/Sounds/SE/colision.ogg"
	};

};
