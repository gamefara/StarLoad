#pragma once
#include "BaseScene.h"

class MenuScene : public BaseScene
{
public:
	//コンストラクタ
	MenuScene(ISceneManager* pChanger);

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;

private:
	enum MenuCursor{
		CURSOR_MENU_PRACTICE,
		CURSOR_MENU_REPLAY,
		CURSOR_MENU_CONFIG,
		CURSOR_MENU_EXIT,
	};

	enum LevelCuesor{
		CURSOR_LEVEL_BEGINNER,
		CURSOR_LEVEL_STANDARD,
		CURSOR_LEVEL_HARD,
		CURSOR_LEVEL_EXPERT,
		CURSOR_LEVEL_CHAOS
	};

	enum ConfigCursor{
		CURSOR_CONFIG_SELECT_BGM,
		CURSOR_CONFIG_VOLUME_BGM,
		CURSOR_CONFIG_VOLUME_SE,
		CURSOR_CONFIG_EXIT
	};

	enum AnimationCursor{

	};

	struct ReplayData{
		std::string PlayerName;
		std::string DateTime;
		int Score;
		std::string Judge;
	};

	//メニュー画面
	int m_bLeftKeyPush;
	int m_bRightKeyPush;
	int m_bUpKeyPush;
	int m_bDownKeyPush;
	int m_bOKKeyPush;
	int m_bCancelKeyPush;
	int m_nAnimeIndex;
	int m_nAnimeLoopIndex;

	const int m_nPhaseCount = 5;
	std::vector<int> m_anCursorPos = { 0, 0, 0, 0, 0 };
	std::vector<int> m_anCursorMaxPos = { 4, 10, 5, 10, 4 };
	int m_nReplayCursorPos = 0;//Replay処理未実装
	int m_nReplayCursorMaxPos = m_anCursorMaxPos[1];//10曲分
	std::vector<int> m_anConfigCursorPos = { 0, 12, 8, 0 };	//曲名,BGM音量,SE音量,Exit
	std::vector<int> m_anConfigCursorMaxPos = { m_anCursorMaxPos[1], 20, 20, 0 };
	float m_nDrawMenuNowAngle;

	//他画面からメニュー画面へ遷移するときのフェードイン時間
	const int m_nFadeCount = 120;
	//カーソル移動時間
	const int m_nMoveCursorCount = 15;
	//フェーズ移動時間
	const int m_nMovePhaseCount = 30;
	//α値ループ時間
	const int m_nLoopAlphaMaxCount = 30;
	int m_nLoopAlphaNowCount = 0;

	void UpdateAlphaCount();
	void UpdateMoveCursorAction();
	void UpdateGoKeyAction();
	void UpdateCancelKeyAction();

	void MoveCursorKeyPushCheck();
	void MoveCursorProcess(int& nPhase);

	void DrawPhaseTitle();
	void DrawPhasePractice();
	void DrawPhaseLevel();
	void DrawPhaseReplay();
	void DrawPhaseConfig();

	void DrawSlideMenu(int nPhase, std::string asList[]);
	void DrawFixMenu(int nPhase, std::string asList[]);
};
