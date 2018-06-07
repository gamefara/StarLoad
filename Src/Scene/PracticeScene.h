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
		int m_bReserve = TRUE;		//結果保存フラグ
	};

	struct StructStage{
		const int m_nStandMaxCount = 300;												//ゲーム待機時間
		const int m_nFinishStageCount = 60;												//ゲーム終了までの時間
		const int m_nFadeoutMaxCount = 180;												//フェードアウト時間
		int m_nGameScore = 0;																//ゲームスコア
		int m_nGameHiScore = 0;																//ゲームハイスコア
		const int m_nGameMaxCount = 10800;												//ステージ終了までの時間
		std::string m_sGameRank = "D";													//ゲームランク
		std::string m_sGameHiRank = "";													//ゲームハイスコア時のランク
		const int m_anRankingRate[4] = { 100000, 300000, 600000, 1000000 };	//ランク上げ数値一覧
		int m_nAddRate = 100;																//1F毎に増えるスコア値
		int m_nRateIndex = 0;																//ランク上げ要素番号
		const int m_nRateMaxIndex = 4;													//ランク上げ要素最大番号
	};

	struct StructBlock{
		const int m_nBlockCount = 32;	//石を表示する個数
		const int m_nInterval = 60;	//石作成カウンタ
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
	void UpdateCollisionMovePlayers(const int& nIndex);
	void UpdateBarMovePlayers();
	void ReverseAllCollision();
	void DrawLoopBlocks();
	void DrawLoopPlayers();
	int GetDrawMagicHandle();
	void FinalizeBlocks();
	void FinalizePlayers();

	int IsProcess(const int& nKey){ return m_pKey->IsKeyPush(nKey); }
	int IsFixedProcess(const int& nKey){ return m_pKey->IsKeyFixedPush(nKey, 15); }
};