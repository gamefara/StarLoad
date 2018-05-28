#include "Player.h"

Player::Player(int nPlayerX, int nPlayerY, int nHeight, int nWidth, int nHandle, int nWhiteHandle, int nIndex, Animation*& pAnime)
{
	m_pStatusSet = new StructStatus();
	m_pStatusSet->m_nPlayerX = nPlayerX;
	m_pStatusSet->m_nPlayerY = nPlayerY;
	m_pStatusSet->m_nPlayerHeight = nHeight;
	m_pStatusSet->m_nPlayerWidth = nWidth;
	m_pStatusSet->m_nPlayerFlashHandle = nHandle;
	m_pStatusSet->m_nPlayerStopHandle = nWhiteHandle;
	m_pStatusSet->m_nPlayerIndex = nIndex;
	
	m_pAnime = pAnime;
}


Player::~Player()
{
	delete m_pStatusSet;
}

void Player::Update(){
	if(m_pStatusSet->m_bPlayerCollision){
		if(m_pStatusSet->m_nPlayerAlphaCount >= 100) return;
		m_pStatusSet->m_nPlayerAlphaCount += 2;
	}
	else{
		if(m_pAnime->IsAllStoppedAnimation()) return;
		int& nCount = m_pStatusSet->m_nPlayerFlashCount;
		const int& nCycleCount = m_pStatusSet->m_nPlayerFlashCycleCount;
		nCount = (nCount + 1) % nCycleCount;
	}
}

void Player::DrawLoop(){
	int nDrawMinX = GetPlayerPositionMinX();
	int nDrawMinY = GetPlayerPositionMinY();
	int nDrawMaxX = GetPlayerPositionMaxX();
	int nDrawMaxY = GetPlayerPositionMaxY();
	//Õ“ËÏ‚Ý‚È‚ç“_–Å•`ŽÊ‚È‚µ
	if(GetCollision()){
		DrawExtendGraph(nDrawMinX, nDrawMinY, nDrawMaxX, nDrawMaxY, m_pStatusSet->m_nPlayerStopHandle, FALSE);

		float fAlpha = (255.0f / 100.0f) * m_pStatusSet->m_nPlayerAlphaCount;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (int)fAlpha);
	}
	else if(IsFlash()){
		int& nCycleCount = m_pStatusSet->m_nPlayerFlashCycleCount;
		int nHalfCount = nCycleCount / 2;
		int& nCount = m_pStatusSet->m_nPlayerFlashCount;
		int nAlphaCount = (nCount < nHalfCount ? nCount : nCycleCount - nCount);
		float fAlpha = (255.0f / nHalfCount) * nAlphaCount;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fAlpha);
	}
	DrawExtendGraph(nDrawMinX, nDrawMinY, nDrawMaxX, nDrawMaxY, m_pStatusSet->m_nPlayerFlashHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
