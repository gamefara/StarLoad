#pragma once
#include "BaseScene.h"

class ReplayScene : public BaseScene
{
public:
	//コンストラクタ
	ReplayScene(ISceneManager* changer);

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;
};

