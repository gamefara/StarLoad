#pragma once
#include "BaseScene.h"
class PracticeScene : public BaseScene
{
public:
	//コンストラクタ
	PracticeScene(ISceneManager* pChanger);

	std::vector<int> m_anBlocksList;
	std::vector<std::string> m_asBlocksMarkList;
	int m_nGameCount;
	int m_nGameNowCount;
	int m_nAnimeIndex;

	void Initialize() override;
	void Update() override;
	void DrawLoop() override;
	void Finalize() override;
};

