#include "PracticeScene.h"
#include "DxLib.h"

PracticeScene::PracticeScene(ISceneManager* pChanger) : BaseScene(pChanger)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Initialize(){
	m_anBlocksList = { 0,10,20,30,40,50,100,200,300,400,500,600 };
	//▲=Player, ■=Block
	m_asBlocksMarkList = { "▲", "■", ">\n■", "<\n■"};
	m_nGameCount = 3600;
	m_nGameNowCount = 0;
	m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nGameCount);
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Update(){
	if(m_nGameNowCount < m_nGameCount){
		for(int i = 0; i < m_anBlocksList.size(); i++){
			if(m_nGameNowCount != m_anBlocksList.at(i)) continue;

			//ブロック生成
		}

		m_nGameNowCount++;
	}
	else{
		//終了
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Practice");
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Finalize(){
}
