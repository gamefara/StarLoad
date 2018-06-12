#include "DxLib.h"
#include "Scene/SceneMgr.h"

extern int IsContinueGame();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	SetWindowText("Star Load");
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetGraphMode(WindowMaxX, WindowMaxY, 32);

	//多重起動禁止
	SetDoubleStartValidFlag(FALSE);
	if(DxLib_Init() == Invalid) return Invalid;

	SetDrawScreen(DX_SCREEN_BACK);

	//new宣言しないので終了時自動でdeleteされる
	SceneMgr m_pManager;
	m_pManager.Initialize();
	while(IsContinueGame() && m_pManager.GetPhase() != PHASE_EXIT){
		m_pManager.Update();
		m_pManager.DrawLoop();
	}
	m_pManager.Finalize();
	DxLib_End();
	return Success;
}

/// <summary>
/// ゲームを続けるかどうかフラグをチェックします。
/// </summary>
/// <returns>ゲームを続けるかどうか</returns>
int IsContinueGame(){
	if(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) return TRUE;
	return FALSE;
}