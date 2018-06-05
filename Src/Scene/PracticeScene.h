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
		int m_nLevel = Level::LEVEL_STANDARD;											//ゲームレベル
		int m_nGameScore = 0;																//ゲームスコア
		int m_nGameHiScore = 0;																//ゲームハイスコア
		int m_nGameMaxCount = 0;															//ステージ終了までの時間
		std::string m_sGameRank = "D";													//ゲームランク
		std::string m_sGameHiRank = "";													//ゲームハイスコア時のランク
		const int m_anRankingRate[4] = { 100000, 300000, 600000, 1000000 };	//ランク上げ数値一覧
		int m_nAddRate = 100;																//1F毎に増えるスコア値
		int m_nRateIndex = 0;																//ランク上げ要素番号
		const int m_nRateMaxIndex = 4;													//ランク上げ要素最大番号
		const int m_nFieldMinX = (WindowMaxX / 2) - 140;							//移動可能領域左端X座標
		const int m_nFieldMaxX = (WindowMaxX / 2) + 140;							//移動可能領域右端X座標
		const int m_nFieldUpY = WindowCenY;												//移動可能領域上Y座標
		const int m_nFieldDownY = (int)(WindowCenY * 1.5);							//移動可能領域下Y座標
	};

	struct StructBlock{
		int m_nBlockCount = Invalid;			//石を表示する個数
		int m_nBlockSpeed = 6;					//ブロック落下速度
		const int m_nBlockDefaultSpeed = 6;	//ブロック初期落下速度
		const int m_nBlockHeight = 10;		//ブロック表示の高さ
		const int m_nBlockWidth = 40;			//ブロック表示の幅
	};

	struct StructPlayer{
		const int m_nHeight = 10;			//プレイヤー表示の高さ
		const int m_nWidth = 40;			//プレイヤー表示の幅
		int m_nBarIndex = 3;					//プレイヤー現在地
		const int m_nBarCount = 7;			//プレイヤーのバーの数
	};

	StructIndex* m_pIndexSet;
	StructFlag* m_pFlagSet;
	StructStage* m_pStageSet;
	StructBlock* m_pBlockSet;
	StructPlayer* m_pPlayerSet;

	void InitializeSceneStatus();
	void InitializeBlocks();
	void InitializeBlockList();
	void CreateStageBlocks(int& nGameCount, int& nTimes);

	float CreateOneBeat(const float& fBpm){ return 60.0f * nFps / fBpm; }
	void InitializePlayer();
	int GetPlayerBar();
	void UpdateStageClearProcess();
	void UpdateGameOverProcess();
	void UpdateFadeOutProcess();
	void UpdateGamePlayProcess();
	void UpdateRank(std::string& sRank);
	void UpdateBlocks();
	void UpdatePlayers();
	void UpdateCollisionPlayers(const int& nIndex);
	void UpdateCollisionMovePlayers(const int& nIndex);
	void UpdateBarMovePlayers();
	void DrawLoopBlocks();
	void DrawLoopPlayers();
	int GetDrawMagicHandle();
	void FinalizeBlocks();
	void FinalizePlayers();

	int IsProcess(const int& nKey){ return m_pKey->IsKeyPush(nKey); }
	int IsFixedProcess(const int& nKey){ return m_pKey->IsKeyFixedPush(nKey, 15); }

	std::vector<Block*> m_apBlock;
	std::vector<Player*> m_apPlayer;
	std::vector<int> m_anBlocksXList;
	std::vector<int> m_anBlocksYList;

	int m_nSoundHandle;			//音楽ハンドル
};