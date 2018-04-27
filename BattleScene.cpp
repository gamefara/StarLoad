#include "BattleScene.h"
#include "DxLib.h"


BattleScene::BattleScene(ISceneManager* changer) : BaseScene(changer)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void BattleScene::Initialize(){
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void BattleScene::Update(){
	if(CheckHitKey(KEY_INPUT_DOWN) == TRUE){
		m_pSceneChanger->ChangeScene(SCENE_REPLAY);	//シーン移動
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void BattleScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Battle");
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void BattleScene::Finalize(){
}
