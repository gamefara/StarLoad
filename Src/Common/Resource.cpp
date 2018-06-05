#include "Resource.h"

Resource::~Resource()
{
	InitGraph();
	InitSoundMem();
}

int Resource::IsLoadGraphics(){
	if(m_nGraphicLoadingCount >= static_cast<int>(ResourceImage::IMAGE_UNKNOWN)) return FALSE;

	m_anGraphicsHandleList[m_nGraphicLoadingCount] = LoadGraph(m_asGraphicsList[m_nGraphicLoadingCount]);
	m_nGraphicLoadingCount++;
	return TRUE;
}

int Resource::IsLoadSounds(){
	if(m_nSoundsLoadingCount >= static_cast<int>(ResourceSound::SOUND_UNKNOWN)) return FALSE;

	m_anSoundsHandleList[m_nSoundsLoadingCount] = LoadSoundMem(m_asSoundsList[m_nSoundsLoadingCount]);
	m_nSoundsLoadingCount++;
	return TRUE;
}

int Resource::GetGraphicsHandle(ResourceImage eImage){
	int nIndex = static_cast<int>(eImage);
	if(nIndex < 0 || nIndex >= static_cast<int>(ResourceImage::IMAGE_UNKNOWN)) return Invalid;
	return m_anGraphicsHandleList[nIndex];
}

int Resource::GetSoundsHandle(ResourceSound eSound){
	int nIndex = static_cast<int>(eSound);
	if(nIndex < 0 || nIndex >= static_cast<int>(ResourceSound::SOUND_UNKNOWN)) return Invalid;
	return m_anSoundsHandleList[nIndex];
}
