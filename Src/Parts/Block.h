#pragma once
#include "../Task.h"
#include "../Common/Animation.h"

class Block : public Task
{
public:
	Block(int nX, int nY, int nSpeed, int nPos, int nHandle, Animation*& pAnime, const int& nIndex);
	~Block();

	void Update() override;
	void DrawLoop() override;
	
	int GetBlockPositionX() const { return m_pStatusSet->m_nBlockX; }
	int GetBlockPositionY() const { return m_pStatusSet->m_nBlockY; }
	void SetBlockHandle(const int& nHandle) { m_pStatusSet->m_nHandle = nHandle; }
	int GetBlockHandle() const { return m_pStatusSet->m_nHandle; }
	void SetCollision() { m_pStatusSet->m_bCollision = TRUE; }
	int GetCollision() const { return m_pStatusSet->m_bCollision; }
	int GetBlockIndex() const { return m_pStatusSet->m_nPos; }
private:
	struct StructStatus{
		int m_nBlockX;			//ブロックX座標
		int m_nBlockY;			//ブロックY座標
		int m_nPos;				//ブロック要素番号
		int m_nSpeed;			//ブロック速さ
		int m_nAlphaCount;	//ブロック透過度カウンタ
		int m_bCollision;		//ブロック当たり判定
		int m_nHandle;			//ブロック画像
		int m_nIndex;			//ブロックアニメーション操作要素
	};

	StructStatus* m_pStatusSet;
	Animation* m_pAnime;
};

