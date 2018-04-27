#pragma once
#include "ISceneManager.h"
#include "BaseScene.h"

class SceneMgr : public ISceneManager, Task
{
private:
	//シーン管理変数
	BaseScene* m_pScene;
	Animation* m_pAnimeScene;

	//次のシーン管理変数
	Scene m_eNextScene;
	Phase m_eNextPhase;

public:
	SceneMgr();
	~SceneMgr();
	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;

	//引数nextSceneにシーンを変更する
	void ChangeScene(Scene eScene) override;

	void SetPhase(Phase ePhase) override;
	int GetPhase() override;
};

