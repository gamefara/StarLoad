#include "Player.h"

Player::Player(int nX, int nY, const int& nHandle, const int& nWhiteHandle, const int& nIndex, Animation*& pAnime, const int& nAnimeIndex)
{
	m_pStatusSet = new StructStatus();
	m_pStatusSet->m_nPlayerX = nX;
	m_pStatusSet->m_nPlayerY = nY;
	m_pStatusSet->m_nFlashHandle = nHandle;
	m_pStatusSet->m_nStopHandle = nWhiteHandle;
	m_pStatusSet->m_nIndex = nIndex;
	m_pStatusSet->m_nAnimeIndex = nAnimeIndex;
	
	m_pAnime = pAnime;
}


Player::~Player()
{
	DeletePtr(m_pStatusSet);
}

/// <summary>
/// XVˆ—
/// </summary>
void Player::Update(){
	if(m_pStatusSet->m_bCollision){
		if(m_pStatusSet->m_nAlphaCount >= 100) return;
		m_pStatusSet->m_nAlphaCount += 2;
	}
	else{
		if(m_pAnime->IsAllStoppedAnimation()) return;
		int& nCount = m_pStatusSet->m_nFlashCount;
		const int& nCycleCount = m_pStatusSet->m_nFlashCycleCount;
		nCount = (nCount + 1) % nCycleCount;
	}
}

/// <summary>
/// •`‰æˆ—
/// </summary>
void Player::DrawLoop(){
	const int& nCenX = GetPlayerPositionX();
	const int& nCenY = GetPlayerPositionY();
	double dAngle = m_pAnime->GetAnimationCount(m_pStatusSet->m_nAnimeIndex) / 10;
	//Õ“ËÏ‚İ‚È‚ç“_–Å•`Ê‚È‚µ
	if(GetCollision()){
		DrawRotaGraph(nCenX, nCenY, 0.5, dAngle, m_pStatusSet->m_nStopHandle, TRUE);

		float fAlpha = (255.0f / 100.0f) * m_pStatusSet->m_nAlphaCount;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - static_cast<int>(fAlpha));
	}
	else if(IsFlash()){
		int& nCycleCount = m_pStatusSet->m_nFlashCycleCount;
		int nHalfCount = nCycleCount / 2;
		int& nCount = m_pStatusSet->m_nFlashCount;
		int nAlphaCount = (nCount < nHalfCount ? nCount : nCycleCount - nCount);
		float fAlpha = (255.0f / nHalfCount) * nAlphaCount;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fAlpha));
	}
	DrawRotaGraph(nCenX, nCenY, 0.5, dAngle, m_pStatusSet->m_nFlashHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
