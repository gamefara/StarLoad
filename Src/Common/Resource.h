#pragma once
#include "../Task.h"
#include "IResourceManager.h"

class Resource : public IResourceManager{
public:
	Resource(){}
	~Resource();

	int IsLoadGraphics() override;
	int IsLoadSounds() override;

	int GetGraphicsHandle(ResourceImage eImage) override;
	int GetSoundsHandle() override;
	int GetGraphicsLoadingCount() override { return m_nGraphicLoadingCount; }
	int GetSoundsLoadingCount() override { return m_nSoundsLoadingCount; }

private:
	int m_anGraphicsHandleList[(int)IMAGE_UNKNOWN];
	int m_anSoundsHandleList[(int)SOUND_UNKNOWN];
	int m_nGraphicLoadingCount = 0;
	int m_nSoundsLoadingCount = 0;

	const char* m_asGraphicsList[(int)IMAGE_UNKNOWN] = {
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Resource/Graphics/practice.png",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Resource/Graphics/aquablue.png",
		"Resource/Graphics/black.png",
		"Resource/Graphics/blue.png",
		"Resource/Graphics/green.png",
		"Resource/Graphics/orange.png",
		"Resource/Graphics/purple.png",
		"Resource/Graphics/red.png",
		"Resource/Graphics/white.png",
		"Resource/Graphics/yellow.png",
		"",
		"",
		"",
		"",
		""
	};
	const char* m_asSoundsList[(int)SOUND_UNKNOWN] = {
		"Resource/Sounds/BGM/title.ogg",
		"Resource/Sounds/BGM/stage.ogg",
		"",
		"",
		"",
		"",
		""
	};

};
