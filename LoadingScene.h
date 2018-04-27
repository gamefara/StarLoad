#pragma once
#include "BaseScene.h"
#include <vector>
#include <string>


//メイン処理クラス
class LoadingScene : public BaseScene{
public:
	//コンストラクタ
	LoadingScene(ISceneManager* changer);

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;

private:
	int m_nLoadCount;
	int m_nWaitCount;

	void DrawLoadGauge();
};

