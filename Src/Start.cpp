#include "DxLib.h"
#include "Scene/SceneMgr.h"

extern int IsContinueGame();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	//ウィンドウモード変更と初期化と裏画面設定
	ChangeWindowMode(TRUE);
	SetGraphMode(WindowMaxX, WindowMaxY, 32);

	//多重起動禁止
	SetDoubleStartValidFlag(FALSE);
	if(DxLib_Init() == Invalid) return Invalid;

	SetDrawScreen(DX_SCREEN_BACK);

	//new宣言しないので終了時自動でdeleteされる
	SceneMgr sceneMgr;
	sceneMgr.Initialize();
	while(IsContinueGame() && sceneMgr.GetPhase() != PHASE_EXIT){
		sceneMgr.Update();
		sceneMgr.DrawLoop();
	}
	sceneMgr.Finalize();
	DxLib_End();
	return Success;
}

int IsContinueGame(){
	if(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) return TRUE;
	return FALSE;
}