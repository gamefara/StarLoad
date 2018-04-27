#pragma once

enum Scene{
	SCENE_LOAD,
	SCENE_MENU,
	SCENE_PRACTICE,
	SCENE_BATTLE,
	SCENE_REPLAY,
	SCENE_CONFIG,
	SCENE_EXIT,

	SCENE_UNKNOWN
};

enum Phase{
	PHASE_LOAD = -1,		//ロード中
	PHASE_TITLE = 0,		//メニュー選択
	PHASE_PRACTICE = 1,	//曲選択
	PHASE_LEVEL = 2,		//レベル選択
	PHASE_REPLAY = 3,		//リプレイ選択
	PHASE_CONFIG = 4,		//コンフィグ選択
	PHASE_EXIT = 5,		//ゲーム終了
	PHASE_GAMEPLAY = 6,	//ゲーム開始
	PHASE_PAUSE = 7,		//一時停止
	PHASE_STORAGE = 8,	//保存

	PHASE_UNKNOWN = 99
};

class ISceneManager
{
public:
	ISceneManager();
	virtual ~ISceneManager() = 0;
	virtual void ChangeScene(Scene eScene) = 0;	//指定シーンに変更する
	virtual void SetPhase(Phase ePhase) = 0;	//指定フェーズに変更する
	virtual int GetPhase() = 0;
};

