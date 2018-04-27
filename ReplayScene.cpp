#include "ReplayScene.h"
#include "DxLib.h"


ReplayScene::ReplayScene(ISceneManager* changer) : BaseScene(changer)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void ReplayScene::Initialize(){
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void ReplayScene::Update(){
	if(CheckHitKey(KEY_INPUT_LEFT) == TRUE){
		m_pSceneChanger->ChangeScene(SCENE_CONFIG);	//シーン移動
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void ReplayScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Replay");
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void ReplayScene::Finalize(){
}
