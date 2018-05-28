#pragma once
#include "../Task.h"
#include "../Common/Animation.h"

class Block : public Task
{
public:
	Block(int nDrawX, int nDrawY, int nSpeed, int nHeight, int nWidth, int nHandle, Animation*& pAnime);
	~Block();

	void Update() override;
	void DrawLoop() override;
	
	int GetBlockPositionMinX() const { return m_pStatusSet->m_nBlockX - m_pStatusSet->m_nBlockWidth / 2; };
	int GetBlockPositionMinY() const { return m_pStatusSet->m_nBlockY - m_pStatusSet->m_nBlockHeight / 2; };
	int GetBlockPositionCenX() const { return m_pStatusSet->m_nBlockX; }
	int GetBlockPositionCenY() const { return m_pStatusSet->m_nBlockY; }
	int GetBlockPositionMaxX() const { return m_pStatusSet->m_nBlockX + m_pStatusSet->m_nBlockWidth / 2; };
	int GetBlockPositionMaxY() const { return m_pStatusSet->m_nBlockY + m_pStatusSet->m_nBlockHeight / 2; };
	void SetCollision() { m_pStatusSet->m_bBlockCollision = TRUE; };
	int GetCollision() const { return m_pStatusSet->m_bBlockCollision; };
private:
	struct StructStatus{
		int m_nBlockX;						//ブロックX座標
		int m_nBlockY;						//ブロックY座標
		int m_nBlockSpeed;				//ブロック速さ
		int m_nBlockHeight;				//ブロック画像高さ
		int m_nBlockWidth;				//ブロック画像幅
		int m_nAlphaCount;				//ブロック透過度カウンタ
		int m_bBlockCollision;			//ブロック当たり判定
		int m_nBlockGraphHandle;		//ブロック画像
	};

	StructStatus* m_pStatusSet;
	Animation* m_pAnime;
};

