#pragma once
#include "BaseScene.h"
#include "../Parts/Block.h"
#include "../Parts/Player.h"

class PracticeScene : public BaseScene
{
public:
	//コンストラクタ
	PracticeScene(ISceneManager* pChanger, IResourceManager* pResource);
	virtual ~PracticeScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void DrawLoop() override;
	virtual void Finalize() override;

private:
	enum BlockPosition{
		BLOCK_POSITION_C,
		BLOCK_POSITION_D,
		BLOCK_POSITION_E,
		BLOCK_POSITION_F,
		BLOCK_POSITION_G,
		BLOCK_POSITION_A,
		BLOCK_POSITION_B
	};

	struct StructIndex{
		int m_nAnimeIndex = Invalid;		//ステージの現時間をカウント
		int m_nFinishStageIndex = Invalid;	//ゲーム進行が止まるまでのカウント
		int m_nMoveSceneIndex = Invalid;	//画面移動完了までの時間をカウント
		int m_nFadeoutIndex = Invalid;	//フェードアウト完了までをカウント
	};

	struct StructFlag{
		int m_bStageClear = FALSE;	//ステージ終了フラグ
		int m_bGameOver = FALSE;	//ゲームオーバーフラグ
		int m_bContinue = FALSE;	//コンティニューフラグ
		int m_bFadeout = FALSE;		//フェードアウトフラグ
		int m_bClearMusic = TRUE;	//クリア時SEフラグ
	};

	struct StructStage{
		const int m_nStandMaxCount = 300;												//ゲーム待機時間
		const int m_nFinishStageCount = 60;												//ゲーム終了までの時間
		const int m_nFadeoutMaxCount = 20;												//フェードアウト時間
		int m_nGameScore = 0;																//ゲームスコア
		int m_nGameHiScore = 0;																//ゲームハイスコア
		const int m_nGameMaxCount = 114 * 60;											//ステージ終了までの時間
		std::string m_sGameRank = "D";													//ゲームランク
		std::string m_sGameHiRank = "";													//ゲームハイスコア時のランク
		const int m_anRankingRate[4] = { 50000, 250000, 750000, 1500000 };	//ランク上げ数値一覧
		int m_nAddRate = 57;																	//1F毎に増えるスコア値
		int m_nRateIndex = 0;																//ランク上げ要素番号
		const int m_nRateMaxIndex = 4;													//ランク上げ要素最大番号
	};

	struct StructBlock{
		const int m_nBlockCount = 64;	//石を表示する個数
	};

	struct StructPlayer{
		const int m_nWidth = 40;									//プレイヤー同士の幅
		int m_nBarIndex = 3;											//プレイヤー現在地
		const int m_nBarCount = 7;									//プレイヤーのバーの数
		const int m_nFieldMinX = (WindowMaxX / 2) - 140;	//移動可能領域左端X座標
		const int m_nFieldY = WindowMaxY - 50;					//表示Y座標
	};

	StructIndex* m_pIndexSet;
	StructFlag* m_pFlagSet;
	StructStage* m_pStageSet;
	StructBlock* m_pBlockSet;
	StructPlayer* m_pPlayerSet;
	std::vector<Block*> m_apBlock;
	std::vector<Player*> m_apPlayer;

	int m_nSoundHandle;			//音楽ハンドル

	void InitializeSceneStatus();
	void InitializeBlocks();
	void InitializePlayer();
	void UpdateStageClearProcess();
	void UpdateGameOverProcess();
	void UpdateFadeOutProcess();
	void UpdateGamePlayProcess();
	void UpdateRank(std::string& sRank);
	void UpdateBlocks();
	void CreateBlocks();
	void DeleteBlocks();
	void UpdatePlayers();
	void UpdateCollisionPlayers();
	void UpdateCollisionMovePlayers();
	void UpdateBarMovePlayers();
	void ReverseAllCollision();
	void DrawLoopBlocks();
	void DrawLoopPlayers();
	void FinalizeBlocks();
	void FinalizePlayers();

	/// <summary>
	/// 指定キーを押したかどうかチェックします。
	/// </summary>
	/// <param name="nKey">指定キー</param>
	/// <returns>押したかどうか</returns>
	int IsPushProcess(const int& nKey){ return m_pKey->IsKeyPush(nKey); }

	/// <summary>
	/// 指定キーが15フレーム押しているかチェックします。
	/// </summary>
	/// <param name="nKey">指定キー</param>
	/// <returns>指定フレーム経過したかどうか</returns>
	int IsFixedProcess(const int& nKey){ return m_pKey->IsKeyFixedPush(nKey, 15); }
};