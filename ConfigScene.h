#pragma once
#include "BaseScene.h"

class ConfigScene : public BaseScene
{
public:
	//コンストラクタ
	ConfigScene(ISceneManager* changer);

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;
};

