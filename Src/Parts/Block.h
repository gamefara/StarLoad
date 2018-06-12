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
	
	/// <summary>
	/// 中心X座標を取得します。
	/// </summary>
	/// <returns>中心X座標</returns>
	int GetBlockPositionX() const { return m_pStatusSet->m_nBlockX; }

	/// <summary>
	/// 中心Y座標を取得します。
	/// </summary>
	/// <returns>中心Y座標</returns>
	int GetBlockPositionY() const { return m_pStatusSet->m_nBlockY; }

	/// <summary>
	/// 画像ハンドルを設定します。
	/// </summary>
	/// <param name="nHandle">画像ハンドル</param>
	void SetBlockHandle(const int& nHandle) { m_pStatusSet->m_nHandle = nHandle; }

	/// <summary>
	/// 画像ハンドルを取得します。
	/// </summary>
	/// <returns>画像うハンドル</returns>
	int GetBlockHandle() const { return m_pStatusSet->m_nHandle; }

	/// <summary>
	/// プレイヤーとの衝突済状態にします。
	/// </summary>
	void SetCollision() { m_pStatusSet->m_bCollision = TRUE; }

	/// <summary>
	/// プレイヤーとの衝突状態を取得します。
	/// </summary>
	/// <returns>衝突済みかどうか</returns>
	int GetCollision() const { return m_pStatusSet->m_bCollision; }

	/// <summary>
	/// 要素番号を取得します。
	/// </summary>
	/// <returns>要素番号(0～6)</returns>
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

