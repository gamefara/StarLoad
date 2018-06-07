#include "Block.h"

Block::Block(int nX, int nY, int nSpeed, int nPos, int nHandle, Animation*& pAnime, const int& nIndex)
{
	m_pStatusSet = new StructStatus();
	m_pStatusSet->m_nBlockX = nX;
	m_pStatusSet->m_nBlockY = nY;
	m_pStatusSet->m_nPos = nPos;
	m_pStatusSet->m_nHandle = nHandle;
	m_pStatusSet->m_nIndex = nIndex;

	m_pStatusSet->m_nSpeed = nSpeed;
	m_pAnime = pAnime;
}

Block::~Block()
{
	DeletePtr(m_pStatusSet);
}

void Block::Update(){
	if(m_pAnime->IsAllStoppedAnimation()) return;
	m_pStatusSet->m_nBlockY += m_pStatusSet->m_nSpeed;
}

void Block::DrawLoop(){
	if(m_pStatusSet->m_nBlockY < WindowMinY - 20 || m_pStatusSet->m_nBlockY > WindowMaxY + 20) return;

	const int& nX = GetBlockPositionX();
	const int& nY = GetBlockPositionY();
	double dAngle = m_pAnime->GetAnimationCount(m_pStatusSet->m_nIndex) / 10;
	DrawRotaGraph(nX, nY, 0.5, dAngle, m_pStatusSet->m_nHandle, TRUE);
}
