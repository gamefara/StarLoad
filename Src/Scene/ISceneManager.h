#pragma once

enum Scene{
	SCENE_LOAD,
	SCENE_MENU,
	SCENE_PRACTICE,

	SCENE_UNKNOWN
};

enum Phase{
	PHASE_LOAD = -1,	//ロード中
	PHASE_TITLE,		//メニュー選択
	PHASE_LEVEL,		//レベル選択
	PHASE_RESULT,		//リザルト
	PHASE_CONFIG,		//コンフィグ選択
	PHASE_MANUAL,		//マニュアル
	PHASE_CREDIT,		//クレジット
	PHASE_EXIT,			//ゲーム終了
	PHASE_GAMEPLAY,	//ゲーム開始

	PHASE_COUNT
};

enum Level{
	LEVEL_BEGINNER,	//難易度低
	LEVEL_STANDARD,	//難易度中
	LEVEL_HARD,			//難易度高

	LEVEL_COUNT
};

class ISceneManager
{
public:
	virtual ~ISceneManager() = 0;
	virtual void ChangeScene(Scene eScene) = 0;	//指定シーンに変更する
	virtual void SetPhase(Phase ePhase) = 0;	//指定フェーズに変更する
	virtual int GetPhase() = 0;
	virtual void SetLevel(Level eLevel) = 0;
	virtual int GetLevel() = 0;
};

