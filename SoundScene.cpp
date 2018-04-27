#include "SoundScene.h"
#include "DxLib.h"

SoundScene::SoundScene(ISceneManager* changer) : BaseScene(changer)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void SoundScene::Initialize(){
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void SoundScene::Update(){
	if(CheckHitKey(KEY_INPUT_LEFT) == TRUE){
		m_pSceneChanger->ChangeScene(SCENE_LOAD);	//シーン移動
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void SoundScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Sound");
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void SoundScene::Finalize(){
}
