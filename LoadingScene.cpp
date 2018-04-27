#include "LoadingScene.h"
#include "DxLib.h"

//string maFileList = {
//	"Picture/001.jpg",
//	"Picture/002.jpg"
//};

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="changer"></param>
LoadingScene::LoadingScene(ISceneManager* pChanger) : BaseScene(pChanger){
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void LoadingScene::Initialize(){
	m_nLoadCount = 0;
	m_nWaitCount = 0;
	//ロード画面
	m_pAnime->SetAnimationCount(450);
	m_anImageHandle.resize(m_nImageHandleCount);

}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void LoadingScene::Update(){
	if(m_nLoadCount < m_nImageHandleCount){
		//とりあえず同じ画像読み込みで時間がかかるように見せる
		m_anImageHandle.at(m_nLoadCount) = LoadGraph("Picture/001.jpg");
		m_nLoadCount++;
	}
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

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void LoadingScene::Finalize(){
	m_nLoadCount = 0;
}

void LoadingScene::DrawLoadGauge(){
	//5F毎にLoadingの後に「.」を表示 or 非表示
	std::string loadWord = "Now Loading ";
	int addDotCount = ((m_nLoadCount + m_nWaitCount) % 20) / 5;
	for(int i = 0; i < addDotCount; i++) loadWord.append(".");

	DrawString(0, 350, loadWord.c_str(), GetColor(255, 255, 255));
	DrawFormatString(280, 350, GetColor(255, 255, 255), "%.2f%%", (m_nLoadCount / (float)(m_nImageHandleCount)) * 100.0f);
	DrawBox(0, 382, 300, 406, GetColor(255, 255, 255), TRUE);
	DrawBox(0, 382, m_nLoadCount * 300 / m_nImageHandleCount, 406, GetColor(255, 128, 255), TRUE);
}
