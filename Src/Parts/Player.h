#pragma once
#include "../Task.h"
#include "../Common/Animation.h"

class Player : public Task
{
public:
	Player(int nX, int nY, const int& nHandle, const int& nWhiteHandle, const int& nIndex, Animation*& pAnime, const int& nAnimeIndex);
	~Player();

	void Update() override;
	void DrawLoop() override;

	int GetPlayerPositionX() const { return m_pStatusSet->m_nPlayerX; }
	int GetPlayerPositionY() const { return m_pStatusSet->m_nPlayerY; }

	void SetCollision() { m_pStatusSet->m_bCollision = TRUE; }
	int GetCollision() const { return m_pStatusSet->m_bCollision; }
	void ReverseCollision(){ ReverseFlag(m_pStatusSet->m_bCollision); }
	void SetFlash(const int& nIndex){ 
		if(m_pStatusSet->m_bCollision) return;
		if(nIndex == m_pStatusSet->m_nIndex) m_pStatusSet->m_bFlash = TRUE;
		else m_pStatusSet->m_bFlash = FALSE;
	}
	int IsFlash() const { return m_pStatusSet->m_bFlash; }
private:
	struct StructStatus{
		int m_nPlayerX;					//プレイヤーX座標
		int m_nPlayerY;					//プレイヤーY座標
		int m_nAlphaCount = 0;			//プレイヤー透過度カウンタ
		int m_nFlashCount = 0;			//プレイヤー点滅カウンタ
		int m_nFlashCycleCount = 8;	//1フラッシュに必要なカウント
		int m_bCollision =  FALSE;		//プレイヤーバーのあたり判定
		int m_nFlashHandle;				//プレイヤー画像
		int m_nStopHandle;				//プレイヤー画像2
		int m_nIndex;						//プレイヤー番号
		int m_bFlash = FALSE;			//プレイヤーバー点灯
		int m_nAnimeIndex;				//アニメーション要素番号
	};

	StructStatus* m_pStatusSet;
	Animation* m_pAnime;
};

