#include "Block.h"

Block::Block(int nX, int nY, int nSpeed, int nHeight, int nWidth, int nHandle, Animation*& pAnime)
{
	m_pStatusSet = new StructStatus();
	m_pStatusSet->m_nBlockX = nX;
	m_pStatusSet->m_nBlockY = nY;
	m_pStatusSet->m_nBlockSpeed = nSpeed;
	m_pStatusSet->m_nBlockHeight = nHeight;
	m_pStatusSet->m_nBlockWidth = nWidth;
	m_pStatusSet->m_nBlockGraphHandle = nHandle;

	m_pAnime = pAnime;
}

Block::~Block()
{
	delete m_pStatusSet;
}

void Block::Update(){
	if(m_pAnime->IsAllStoppedAnimation()) return;
	m_pStatusSet->m_nBlockY += m_pStatusSet->m_nBlockSpeed;
	if(m_pStatusSet->m_bBlockCollision) m_pStatusSet->m_nAlphaCount++;
}

void Block::DrawLoop(){
	if(m_pStatusSet->m_nBlockY < 0 || m_pStatusSet->m_nBlockY > WindowMaxY) return;

	int nDrawMinX = GetBlockPositionMinX();
	int nDrawMinY = GetBlockPositionMinY();
	int nDrawMaxX = GetBlockPositionMaxX();
	int nDrawMaxY = GetBlockPositionMaxY();
	if(m_pStatusSet->m_bBlockCollision){
		int nCount = m_pStatusSet->m_nAlphaCount % 16;
		int nAlpha = (nCount < 8 ? 32 * (8 - nCount) : 32 * (nCount - 8));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	}
	DrawExtendGraph(nDrawMinX, nDrawMinY, nDrawMaxX, nDrawMaxY, m_pStatusSet->m_nBlockGraphHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
