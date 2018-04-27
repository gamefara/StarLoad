#include "DxLib.h"
#include "SceneMgr.h"

extern int ContinueGame();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	//ウィンドウモード変更と初期化と裏画面設定
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	//new宣言しないので終了時自動でdeleteされる
	SceneMgr sceneMgr;
	sceneMgr.Initialize();

	while(ContinueGame() == TRUE && sceneMgr.GetPhase() != PHASE_EXIT){
		sceneMgr.Update();
		sceneMgr.DrawLoop();
	}

	sceneMgr.Finalize();
	DxLib_End();
	return SUCCESS;
}

int ContinueGame(){
	if(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) return TRUE;
	return FALSE;
}