#include "ConfigScene.h"
#include "DxLib.h"

ConfigScene::ConfigScene(ISceneManager* changer) : BaseScene(changer)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void ConfigScene::Initialize(){
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void ConfigScene::Update(){
	if(CheckHitKey(KEY_INPUT_RIGHT) == TRUE){
		m_pSceneChanger->ChangeScene(SCENE_MENU);	//シーン移動
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void ConfigScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Result");
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void ConfigScene::Finalize(){
}
