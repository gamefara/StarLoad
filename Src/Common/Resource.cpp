#include "Resource.h"

Resource::~Resource()
{
	InitGraph();
	InitSoundMem();
}

int Resource::IsLoadGraphics(){
	if(m_nGraphicLoadingCount >= (int)ResourceImage::IMAGE_UNKNOWN) return FALSE;

	m_anGraphicsHandleList[m_nGraphicLoadingCount] = LoadGraph(m_asGraphicsList[m_nGraphicLoadingCount]);
	m_nGraphicLoadingCount++;
	return TRUE;
}

int Resource::IsLoadSounds(){
	if(m_nSoundsLoadingCount >= ResourceSound::SOUND_UNKNOWN) return FALSE;

	m_anSoundsHandleList[m_nSoundsLoadingCount] = LoadSoundMem(m_asSoundsList[m_nSoundsLoadingCount]);
	m_nSoundsLoadingCount++;
	return TRUE;
}

int Resource::GetGraphicsHandle(ResourceImage eImage){
	int nIndex = (int)eImage;
	if(nIndex < 0 || nIndex >= (int)ResourceImage::IMAGE_UNKNOWN) return FALSE;
	return m_anGraphicsHandleList[nIndex];
}

inline int Resource::GetSoundsHandle(){
	int nIndex = static_cast<int>(ResourceSound::SOUND_BGM_STAGE);
	return m_anSoundsHandleList[nIndex];
}
