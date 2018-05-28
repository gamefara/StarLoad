#pragma once
#include "../Task.h"
#include "../Common/Animation.h"

class Player : public Task
{
public:
	Player(int nPlayerX, int nPlayerY, int nHeight, int nWidth, int nHandle, int nWhiteHandle, int nIndex, Animation*& pAnime);
	~Player();

	void Update() override;
	void DrawLoop() override;

	int GetPlayerPositionMinX() const { return m_pStatusSet->m_nPlayerX - m_pStatusSet->m_nPlayerWidth / 2; }
	int GetPlayerPositionMinY() const { return m_pStatusSet->m_nPlayerY - m_pStatusSet->m_nPlayerHeight / 2; }
	int GetPlayerPositionCenX() const { return m_pStatusSet->m_nPlayerX; }
	int GetPlayerPositionCenY() const { return m_pStatusSet->m_nPlayerY; }
	int GetPlayerPositionMaxX() const { return m_pStatusSet->m_nPlayerX + m_pStatusSet->m_nPlayerWidth / 2; }
	int GetPlayerPositionMaxY() const { return m_pStatusSet->m_nPlayerY + m_pStatusSet->m_nPlayerHeight / 2; }

	void SetCollision() { m_pStatusSet->m_bPlayerCollision = TRUE; }
	int GetCollision() const { return m_pStatusSet->m_bPlayerCollision; }
	void SetFlash(const int& nIndex){ 
		if(m_pStatusSet->m_bPlayerCollision) return;

		if(nIndex == m_pStatusSet->m_nPlayerIndex) m_pStatusSet->m_bPlayerFlash = TRUE;
		else m_pStatusSet->m_bPlayerFlash = FALSE;
	}
	int IsFlash() const { return m_pStatusSet->m_bPlayerFlash; }
private:
	struct StructStatus{
		int m_nPlayerX;							//プレイヤーX座標
		int m_nPlayerY;							//プレイヤーY座標
		int m_nPlayerHeight;						//プレイヤー画像高さ
		int m_nPlayerWidth;						//プレイヤー画像幅
		int m_nPlayerAlphaCount = 0;			//プレイヤー透過度カウンタ
		int m_nPlayerFlashCount = 0;			//プレイヤー点滅カウンタ
		int m_nPlayerFlashCycleCount = 8;	//1フラッシュに必要なカウント
		int m_bPlayerCollision =  FALSE;		//プレイヤーバーのあたり判定
		int m_nPlayerFlashHandle;				//プレイヤー画像
		int m_nPlayerStopHandle;				//プレイヤー画像2
		int m_nPlayerIndex;						//プレイヤー番号
		int m_bPlayerFlash = FALSE;			//プレイヤーバー点灯
	};

	StructStatus* m_pStatusSet;
	Animation* m_pAnime;
};

