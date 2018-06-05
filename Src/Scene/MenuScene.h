#pragma once
#include "BaseScene.h"

class MenuScene : public BaseScene
{
public:
	//コンストラクタ
	MenuScene(ISceneManager* pChanger, IResourceManager* pResource);
	virtual ~MenuScene();

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;

private:
	enum MenuCursor{
		CURSOR_MENU_PRACTICE,
		CURSOR_MENU_RESULT,
		CURSOR_MENU_CONFIG,
		CURSOR_MENU_MANUAL,
		CURSOR_MENU_EXIT,
		CURSOR_MENU_COUNT
	};

	enum LevelCursor{
		CURSOR_LEVEL_BEGINNER,
		CURSOR_LEVEL_STANDARD,
		CURSOR_LEVEL_HARD,
		CURSOR_LEVEL_COUNT
	};

	enum ConfigCursor{
		CURSOR_CONFIG_VOLUME_BGM,
		CURSOR_CONFIG_VOLUME_SE,
		CURSOR_CONFIG_EXIT,
		CURSOR_CONFIG_COUNT,
		CURSOR_CONFIG_BGM_COUNT = 20,
		CURSOR_CONFIG_SE_COUNT = 20,
	};

	struct StructReplayData{
		std::string m_sDateTime;
		int m_nScore;
		std::string m_sRank;
	};

	struct StructCursor{
		int m_nMainCursorPos = 0;
		int m_nLevelCursorPos = 0;
		int m_nConfigCursorPos = 0;
		int m_nConfigBGMCursorPos = 12;		//コンフィグファイルから読み取って値変更する
		int m_nConfigSECursorPos = 8;
		const int m_nMainCursorMaxPos = (int)MenuCursor::CURSOR_MENU_COUNT;
		const int m_nLevelCursorMaxPos = (int)LevelCursor::CURSOR_LEVEL_COUNT;
		const int m_nConfigCursorMaxPos = (int)ConfigCursor::CURSOR_CONFIG_COUNT;
		const int m_nConfigBGMCursorMaxPos = (int)ConfigCursor::CURSOR_CONFIG_BGM_COUNT;
		const int m_nConfigSECursorMaxPos = (int)ConfigCursor::CURSOR_CONFIG_SE_COUNT;
	};

	int m_nBackGroundHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BACKGROUND);

	StructReplayData* m_apData[static_cast<int>(Level::LEVEL_UNKNOWN)];
	StructCursor* m_pCursor;

	//メニュー画面
	int m_bLeftKeyPush = FALSE;
	int m_bRightKeyPush = FALSE;
	int m_bUpKeyPush = FALSE;
	int m_bDownKeyPush = FALSE;
	int m_bOKKeyPush = FALSE;
	int m_bCancelKeyPush = FALSE;
	int m_nAnimeIndex = Invalid;
	int m_nAnimeLoopIndex = Invalid;

	float m_nDrawMenuNowAngle = 0.0f;

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
	void MoveCursorProcess();
	int& GetCursorPos();
	const int& GetCursorMaxPos();
	int& GetVolumeCursorPos();
	const int& GetVolumeCursorMaxPos();

	void DrawPhaseTitle();
	int GetDrawMenuHandle(int nIndex);
	int GetDrawMenuDescriptionHandle(int nIndex);
	void DrawPhaseLevel();
	int GetDrawLevelHandle(int nIndex);
	void DrawPhaseResult();
	void DrawPhaseConfig();
	void DrawPhaseManual();
};
