#include "LoadingScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="changer"></param>
LoadingScene::LoadingScene(ISceneManager* pChanger, IResourceManager* pResource) : BaseScene(pChanger, pResource){
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void LoadingScene::Update(){
	if(m_pResource->IsLoadSounds()) m_nLoadCount++;
	else if(m_pResource->IsLoadGraphics()) m_nLoadCount++;
	else{
		//ゲージ100％から1秒してから次のシーンへ
		if(m_nWaitCount >= 60){
			m_pSceneChanger->ChangeScene(SCENE_MENU);
			SetPhase(PHASE_TITLE);
		}
		m_nWaitCount++;
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void LoadingScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Load");
	DrawLoadGauge();
}

void LoadingScene::DrawLoadGauge(){
	//5F毎にLoadingの後に「.」を表示 or 非表示
	std::string sLoadWord = "Now Loading ";
	int nAddDotCount = ((m_nLoadCount + m_nWaitCount) % 20) / 5;
	for(int i = 0; i < nAddDotCount; i++) sLoadWord.append(".");

	float fRatio = (float)m_nLoadCount / (float)m_nResourceCount;
	DrawString(0, 350, sLoadWord.c_str(), GetColor(255, 255, 255));
	DrawFormatString(280, 350, GetColor(255, 255, 255), "%.2f%%", fRatio * 100);
	DrawBox(0, 382, 300, 406, GetColor(255, 255, 255), TRUE);
	DrawBox(0, 382, m_nLoadCount * 300 / m_nResourceCount, 406, GetColor(255, 128, 255), TRUE);
}
