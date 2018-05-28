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

	enum BlockTempo{
		BLOCK_TEMPO_ALL_NOTE = 1,
		BLOCK_TEMPO_2_NOTE = 2,
		BLOCK_TEMPO_2_DOT_NOTE = 3,
		BLOCK_TEMPO_4_NOTE = 4,
		BLOCK_TEMPO_4_DOT_NOTE = 6,
		BLOCK_TEMPO_8_NOTE = 8,
		BLOCK_TEMPO_8_DOT_NOTE = 12,
		BLOCK_TEMPO_16_NOTE = 16,
		BLOCK_TEMPO_16_DOT_NOTE = 24,
		BLOCK_TEMPO_32_NOTE = 32,
		BLOCK_TEMPO_32_DOT_NOTE = 48
	};

	struct StructIndex{
		int m_nAnimeIndex = Invalid;		//ステージの現時間をカウントする要素番号
		int m_nMoveSceneIndex = Invalid;	//画面移動完了までの時間をカウントする要素番号
		int m_nFadeoutIndex = Invalid;	//フェードアウト完了までをカウントする要素番号
	};

	struct StructFlag{
		int m_bStageStart = FALSE;	//	曲開始フラグ
		int m_bStageClear = FALSE;	//ステージ終了フラグ
		int m_bReserve = TRUE;		//結果保存フラグ
		int m_bGameOver = FALSE;	//ゲームオーバーフラグ
		int m_bContinue = FALSE;	//コンティニューフラグ
		int m_bFadeout = FALSE;		//フェードアウトフラグ
	};

	struct StructStage{
		const int m_nStandMaxCount = 300;												//ゲーム待機時間
		const int m_nFadeoutMaxCount = 180;												//フェードアウト時間
		int m_nLevel = Level::LEVEL_STANDARD;											//ゲームレベル
		int m_nGameScore = 0;																//ゲームスコア
		int m_nGameHiScore = 0;																//ゲームハイスコア
		int m_nGameMaxCount = 0;															//ステージ終了までの時間
		std::string m_sGameRank = "D";													//ゲームランク
		std::string m_sGameHiRank = "";													//ゲームハイスコア時のランク
		const int m_anRankingRate[4] = { 100000, 300000, 600000, 1000000 };	//ランク上げ数値一覧
		int m_nAddRate = 100;																	//1F毎に増えるスコア値
		int m_nRateIndex = 0;																//ランク上げ要素番号
		const int m_nRateMaxIndex = 4;													//ランク上げ要素最大番号
		const int m_nFieldMinX = (WindowMaxX / 2) - 140;							//移動可能領域左端X座標
		const int m_nFieldMaxX = (WindowMaxX / 2) + 140;							//移動可能領域右端X座標
		const int m_nFieldUpY = WindowCenY;												//移動可能領域上Y座標
		const int m_nFieldDownY = (int)(WindowCenY * 1.5);							//移動可能領域下Y座標
		int m_nBegginerBarHandle = Invalid;												//難易度Begginer用バーハンドル
		int m_nStandardBarHandle = Invalid;												//難易度Standard用バーハンドル
		int m_nHardBarHandle = Invalid;													//難易度Hard用バーハンドル
	};

	struct StructBlock{
		int m_nBlockHandle = Invalid;			//ブロック画像ハンドル
		int m_nBlockCount = Invalid;			//石を表示する個数
		int m_nBlockSpeed = 6;					//ブロック落下速度
		const int m_nBlockDefaultSpeed = 6;	//ブロック初期落下速度
		const int m_nBlockHeight = 10;		//ブロック表示の高さ
		const int m_nBlockWidth = 40;			//ブロック表示の幅
	};

	struct StructPlayer{
		int m_nHandle = Invalid;			//プレイヤーバー画像ハンドル
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
	void InitializePlayerBar();
	void UpdateBlocks();
	void UpdatePlayers();
	void UpdateCollisionPlayers(const int& nIndex);
	void UpdateCollisionMovePlayers(const int& nIndex);
	void UpdateBarMovePlayers();
	void DrawLoopBlocks();
	void DrawLoopPlayers();
	void FinalizeBlocks();
	void FinalizePlayers();

	void PrepareRestartStage();
	void PrepareReturnTitle();

	int IsProcess(const int& nKey){ return m_pKey->IsKeyPush(nKey); }
	int IsFixedProcess(const int& nKey){ return m_pKey->IsKeyFixedPush(nKey, 15); }

	std::vector<Block*> m_apBlock;
	std::vector<Player*> m_apPlayer;
	std::vector<int> m_anBlocksXList;
	std::vector<int> m_anBlocksYList;

	int m_nSoundHandle;			//音楽ハンドル
};