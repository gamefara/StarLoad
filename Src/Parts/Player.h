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

	/// <summary>
	/// 中心X座標を取得します。
	/// </summary>
	/// <returns>中心X座標</returns>
	int GetPlayerPositionX() const { return m_pStatusSet->m_nPlayerX; }

	/// <summary>
	/// 中心Y座標を取得します。
	/// </summary>
	/// <returns>中心Y座標</returns>
	int GetPlayerPositionY() const { return m_pStatusSet->m_nPlayerY; }

	/// <summary>
	/// 衝突済み情報を設定します。
	/// </summary>
	void SetCollision() { m_pStatusSet->m_bCollision = TRUE; }

	/// <summary>
	/// 衝突情報を取得します。
	/// </summary>
	/// <returns>衝突したかどうか</returns>
	int GetCollision() const { return m_pStatusSet->m_bCollision; }

	/// <summary>
	/// 衝突情報を反転します。
	/// </summary>
	void ReverseCollision(){ ReverseFlag(m_pStatusSet->m_bCollision); }

	/// <summary>
	/// 操作中の星を点滅させます。
	/// </summary>
	/// <param name="nIndex">操作中の要素番号</param>
	void SetFlash(const int& nIndex){ 
		if(m_pStatusSet->m_bCollision) return;
		if(nIndex == m_pStatusSet->m_nIndex) m_pStatusSet->m_bFlash = TRUE;
		else m_pStatusSet->m_bFlash = FALSE;
	}

	/// <summary>
	/// 点滅情報を取得します。
	/// </summary>
	/// <returns>点滅しているかどうか</returns>
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

