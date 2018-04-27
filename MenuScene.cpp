#define _USE_MATH_DEFINES
#include "MenuScene.h"
#include "DxLib.h"
#include <math.h>

MenuScene::MenuScene(ISceneManager* pChanger) : BaseScene(pChanger)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::Initialize(){
	m_bLeftKeyPush = FALSE;
	m_bRightKeyPush = FALSE;
	m_bUpKeyPush = FALSE;
	m_bDownKeyPush = FALSE;
	m_bOKKeyPush = FALSE;
	m_bCancelKeyPush = FALSE;
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::Update(){
	UpdateAlphaCount();
	UpdateMoveCursorAction();
	UpdateGoKeyAction();
	UpdateCancelKeyAction();
}

void MenuScene::UpdateAlphaCount(){
	if(m_nLoopAlphaNowCount == 0) m_nAnimeLoopIndex = m_pAnime->SetAnimationCount(m_nLoopAlphaMaxCount);
	m_nLoopAlphaNowCount = (m_nLoopAlphaNowCount + 1) % m_nLoopAlphaMaxCount;
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::Finalize(){
	m_anCursorPos.clear();
	m_anCursorMaxPos.clear();
}

/// <summary>
/// カーソル移動した時の処理を行います。
/// </summary>
void MenuScene::UpdateMoveCursorAction(){
	if(m_bOKKeyPush == TRUE || m_bCancelKeyPush == TRUE) return;

	if(m_bLeftKeyPush == FALSE && m_bRightKeyPush == FALSE && m_bUpKeyPush == FALSE && m_bDownKeyPush == FALSE){
		MoveCursorKeyPushCheck();
	}
	else{
		int nPhase = GetPhase();
		//回転移動のフェーズ時に使用
		if(nPhase == PHASE_TITLE){
			float fDivAngle = 360.0f / m_anCursorMaxPos.at(PHASE_TITLE);
			float fFromAngle = fDivAngle * m_anCursorPos.at(PHASE_TITLE);
			float fToAngle = fDivAngle * (m_anCursorPos.at(PHASE_TITLE) + (m_bLeftKeyPush == TRUE ? -1 : 1));
			m_nDrawMenuNowAngle = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, fFromAngle, fToAngle);
		}

		if(m_pAnime->GetAnimationCount(m_nAnimeIndex) != m_nMoveCursorCount) return;

		MoveCursorProcess(nPhase);
	}
}

void MenuScene::MoveCursorKeyPushCheck(){
	if(CheckHitKey(KEY_INPUT_LEFT) == TRUE){
		m_bLeftKeyPush = TRUE;
		m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMoveCursorCount);
	}
	else if(CheckHitKey(KEY_INPUT_RIGHT) == TRUE){
		m_bRightKeyPush = TRUE;
		m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMoveCursorCount);
	}
	else if(CheckHitKey(KEY_INPUT_UP) == TRUE){
		m_bUpKeyPush = TRUE;
		m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMoveCursorCount);
	}
	else if(CheckHitKey(KEY_INPUT_DOWN) == TRUE){
		m_bDownKeyPush = TRUE;
		m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMoveCursorCount);
	}
}

void MenuScene::MoveCursorProcess(int& nPhase){
	int& nNowUDPos = m_anCursorPos.at(nPhase);
	int& nMaxUDPos = m_anCursorMaxPos.at(nPhase);
	int& nNowLRPos = (nPhase == PHASE_REPLAY ? m_nReplayCursorPos : (nPhase == PHASE_CONFIG ? m_anConfigCursorPos.at(nNowUDPos) : m_anCursorPos.at(nPhase)));
	int& nMaxLRPos = (nPhase == PHASE_REPLAY ? m_nReplayCursorMaxPos : (nPhase == PHASE_CONFIG ? m_anConfigCursorMaxPos.at(nNowUDPos) : m_anCursorMaxPos.at(nPhase)));

	if(m_bLeftKeyPush == TRUE){
		m_bLeftKeyPush = FALSE;
		if(nPhase == PHASE_CONFIG && nNowUDPos != CURSOR_CONFIG_SELECT_BGM){
			if(nNowLRPos <= 0) return;
			nNowLRPos--;
		}
		else nNowLRPos = (nNowLRPos + (nMaxLRPos - 1)) % nMaxLRPos;
	}
	else if(m_bRightKeyPush == TRUE){
		m_bRightKeyPush = FALSE;
		if(nPhase == PHASE_CONFIG && nNowUDPos != CURSOR_CONFIG_SELECT_BGM){
			if(nNowLRPos >= nMaxLRPos) return;
			nNowLRPos++;
		}
		else nNowLRPos = (nNowLRPos + 1) % nMaxLRPos;
	}
	else if(m_bUpKeyPush == TRUE){
		m_bUpKeyPush = FALSE;
		nNowUDPos = (nNowUDPos + (nMaxUDPos - 1)) % nMaxUDPos;
	}
	else if(m_bDownKeyPush == TRUE){
		m_bDownKeyPush = FALSE;
		nNowUDPos = (nNowUDPos + 1) % nMaxUDPos;
	}
}

/// <summary>
/// 決定キーを押したときの処理を行います。
/// </summary>
void MenuScene::UpdateGoKeyAction(){
	if(m_bLeftKeyPush == TRUE || m_bRightKeyPush == TRUE || m_bCancelKeyPush == TRUE) return;

	int nPhase = GetPhase();
	if(m_bOKKeyPush == FALSE){
		if(CheckHitKey(KEY_INPUT_Z) == TRUE){
			if(nPhase == PHASE_CONFIG && m_anCursorPos.at(nPhase) == CURSOR_CONFIG_EXIT){
				m_bCancelKeyPush = TRUE;
				SetPhase(PHASE_TITLE);
			}
			else m_bOKKeyPush = TRUE;
			m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMovePhaseCount);
		}
	}
	else{
		if(m_pAnime->GetAnimationCount(m_nAnimeIndex) != m_nMovePhaseCount) return;
		m_bOKKeyPush = FALSE;

		switch(nPhase){
		case PHASE_PRACTICE:
			SetPhase(PHASE_LEVEL);
			break;
		case PHASE_LEVEL:
			SetPhase(PHASE_GAMEPLAY);
			m_pSceneChanger->ChangeScene(SCENE_PRACTICE);
			break;
		case PHASE_REPLAY:
			SetPhase(PHASE_REPLAY);
			m_pSceneChanger->ChangeScene(SCENE_REPLAY);
			break;
		case PHASE_CONFIG:
			break;
		default:
			switch(m_anCursorPos.at(PHASE_TITLE)){
			case CURSOR_MENU_PRACTICE:
				SetPhase(PHASE_PRACTICE);
				break;
			case CURSOR_MENU_REPLAY:
				SetPhase(PHASE_REPLAY);
				break;
			case CURSOR_MENU_CONFIG:
				SetPhase(PHASE_CONFIG);
				break;
			case CURSOR_MENU_EXIT:
			default:
				SetPhase(PHASE_EXIT);
				break;
			}

			break;
		}
	}
}

/// <summary>
/// 戻るキーを押したときの処理を行います。
/// </summary>
void MenuScene::UpdateCancelKeyAction(){
	if(m_bLeftKeyPush == TRUE || m_bRightKeyPush == TRUE || m_bOKKeyPush == TRUE) return;

	if(m_bCancelKeyPush == FALSE){
		if(CheckHitKey(KEY_INPUT_X) == TRUE){
			int nPhase = GetPhase();
			//タイトル画面では処理をしない
			if(nPhase == PHASE_TITLE) return;

			m_bCancelKeyPush = TRUE;
			m_nAnimeIndex = m_pAnime->SetAnimationCount(m_nMovePhaseCount);

			//レベル選択以外はタイトルへ戻る
			if(nPhase == PHASE_LEVEL) SetPhase(PHASE_PRACTICE);
			else SetPhase(PHASE_TITLE);
		}
	}
	else{
		if(m_pAnime->GetAnimationCount(m_nAnimeIndex) != m_nMovePhaseCount) return;
		m_bCancelKeyPush = FALSE;
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::DrawLoop(){
	int nPhase = GetPhase();
	if(nPhase >= PHASE_EXIT) return;

	int nTitleX = 280;
	int nTitleY = 50;
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Menu");
	DrawFormatString(100, 100, GetColor(200, 200, 200), "%d", m_anCursorPos.at(nPhase));

	switch(nPhase){
	case PHASE_TITLE:
		DrawPhaseTitle();
		break;
	case PHASE_PRACTICE:
		DrawString(nTitleX, nTitleY, "Sound Select", GetColor(255, 0, 255));
		DrawPhasePractice();
		break;
	case PHASE_LEVEL:
		DrawString(nTitleX, nTitleY, "Level Select", GetColor(255, 0, 255));
		DrawPhaseLevel();
		break;
	case PHASE_REPLAY:
		DrawString(nTitleX, nTitleY, "Replay Select", GetColor(255, 0, 255));
		DrawPhaseReplay();
		break;
	case PHASE_CONFIG:
		DrawString(nTitleX, nTitleY, "Config", GetColor(255, 0, 255));
		DrawPhaseConfig();
		break;
	default:
		break;
	}
}

void MenuScene::DrawPhaseTitle(){
	//選択中のカーソルを円状の下側にするためにpi/2だけずらす
	float fPi = (float)M_PI;
	float fCycle = (2 * fPi) / 360;
	float fAngle = fCycle * m_nDrawMenuNowAngle + fPi / 2;
	std::string asDisplayWord[4] = { "Practice Mode", "Replay Mode", "Config", "Exit" };
	int nCursorPos = m_anCursorPos.at(PHASE_TITLE);

	float fMoveX = 0.0f;
	if(m_bOKKeyPush == TRUE) fMoveX = m_pAnime->GetAnimationSmooth(m_nAnimeIndex, 0.0f, 500.0f);
	else if(m_bCancelKeyPush == TRUE) fMoveX = m_pAnime->GetAnimationSmooth(m_nAnimeIndex, 500.0f, 0.0f);
	DrawStringF(320.0f + fMoveX, 250.0f, "▼", GetColor(255, 64, 255));

	for(int i = 0; i < m_anCursorMaxPos.at(PHASE_TITLE); i++){
		float fX = 280.0f + 10 * i + 200.0f * cosf(fAngle - fPi / 2 * i);
		float fY = 240.0f + 30.0f * sinf(fAngle - fPi / 2 * i);
		const char* sData = asDisplayWord[i].c_str();

		float fAlpha = 1.0f;
		if(m_bOKKeyPush == TRUE){
			if(i == nCursorPos) fAlpha = m_pAnime->GetAnimationSlowDownLaps(m_nAnimeLoopIndex, 0.0, 1.0f, 0.0f);
			else fAlpha = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, 1.0f, 0.0f);
		}
		else if(m_bCancelKeyPush == TRUE) fAlpha = m_pAnime->GetAnimationAccelerate(m_nAnimeIndex, 0.0f, 1.0f);
		unsigned int nColor = GetColor((int)(255 * fAlpha), (int)(64 * fAlpha), (int)(255 * fAlpha));
		DrawStringF(fX, fY, sData, nColor);
	}
	DrawStringF(320.0f - fMoveX, 290.0f, "▲", GetColor(255, 64, 255));
}

void MenuScene::DrawPhasePractice(){
	std::string asSound[10] = { "Stage1", "Stage2", "Stage3", "Stage4", "Stage5", "Stage6", "Stage7", "Stage8", "Stage9", "Stage10" };
	DrawSlideMenu(PHASE_PRACTICE, asSound);
}

void MenuScene::DrawPhaseLevel(){
	std::string asLevel[5] = { "Beginner", "Standard", "Hard", "Expert", "Chaos" };
	DrawSlideMenu(PHASE_LEVEL, asLevel);
}

void MenuScene::DrawPhaseReplay(){
	//各Stageの最高点を表示、再現
	std::string a = GetCurrentDateTime();
	DrawString(200, 0, a.c_str(), GetColor(255, 255, 255));

	std::string sStageName = "Stage" + std::to_string(m_nReplayCursorPos + 1);
	DrawString(280, 100, sStageName.c_str(), GetColor(255, 255, 255));

	std::string aasReplay[10][6] = {
		{ "No.1" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.2" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.3" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.4" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.5" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.6" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.7" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.8" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.9" ,"NoName", "--/--/-- --:--:--", "-------", "-" },
		{ "No.10","NoName", "--/--/-- --:--:--", "-------", "-" }
	};
	std::string aaasReplay;

	std::vector<int> anListX = { 50, 110, 190, 380, 470, 490 };

	int nNowPos = m_anCursorPos.at(PHASE_REPLAY);
	int nMaxPos = m_anCursorMaxPos.at(PHASE_REPLAY);
	for(int i = 0; i < 10; i++){
		int nAlpha = (nNowPos == (i % nMaxPos) ? 51 * (GetNowCount() / 20) % 6: 255);
		unsigned int nColor = GetColor(nAlpha, nAlpha, nAlpha);
		for(int j = 0; j < 6; j++){
			DrawString(anListX[j], 100 + 30 * (i + 1), aasReplay[i][j].c_str(), nColor);
		}
	}

	//	DrawSlideMenu(PHASE_REPLAY, asReplay);
}

void MenuScene::DrawPhaseConfig(){
	DrawString(180, 150 + 30 * m_anCursorPos[PHASE_CONFIG], "->", GetColor(255, 255, 255));
	DrawString(200, 150, "<                    >", GetColor(255, 255, 255));
	DrawFormatString(280, 150, GetColor(255, 255, 255), "Stage%d", m_anConfigCursorPos.at(0) + 1);

	//DrawBoxでゲージ表示
	int nGaugeMinX = 200;
	int nGaugeMaxX = 350;
	float fBGMPercent = (float)m_anConfigCursorPos.at(CURSOR_CONFIG_VOLUME_BGM) / (float)m_anConfigCursorMaxPos.at(CURSOR_CONFIG_VOLUME_BGM);
	int nBGMGaugeX = nGaugeMinX + (int)((nGaugeMaxX - nGaugeMinX) * fBGMPercent);
	DrawString(50, 180, "Volume BGM", GetColor(255, 255, 255));
	DrawBox(200, 180, 350, 200, GetColor(255, 255, 255), FALSE);
	DrawBox(200, 180, nBGMGaugeX, 200, GetColor(255, 255, 255), TRUE);
	DrawFormatString(370, 180, GetColor(255, 255, 255), "%.2f%%", fBGMPercent * 100);

	float fSEPercent = (float)m_anConfigCursorPos.at(CURSOR_CONFIG_VOLUME_SE) / (float)m_anConfigCursorMaxPos.at(CURSOR_CONFIG_VOLUME_SE);
	int nSEGaugeX = nGaugeMinX + (int)((nGaugeMaxX - nGaugeMinX) * fSEPercent);
	DrawString(50, 210, "Volume SE", GetColor(255, 255, 255));
	DrawBox(200, 210, 350, 230, GetColor(255, 255, 255), FALSE);
	DrawBox(200, 210, nSEGaugeX, 230, GetColor(255, 255, 255), TRUE);
	DrawFormatString(370, 210, GetColor(255, 255, 255), "%.2f%%", fSEPercent * 100);

	DrawString(200, 240, "Back", GetColor(255, 255, 255));
}

void MenuScene::DrawSlideMenu(int nPhase, std::string asList[]){
	int nNowPos = m_anCursorPos.at(nPhase);
	int nMaxPos = m_anCursorMaxPos.at(nPhase);
	int nHalfPos = (nMaxPos - 1) / 2;	//nMaxPosが偶数の時は切り捨てnMaxPos=4 => nHalfPos=1)
	float fDiffX = 15.0f;
	float fDiffY = 20.0f;
	float fMoveX = (m_bLeftKeyPush == TRUE ? fDiffX : (m_bRightKeyPush == TRUE ? -fDiffX : 0.0f));
	float fMoveY = (m_bLeftKeyPush == TRUE ? fDiffY : (m_bRightKeyPush == TRUE ? -fDiffY : 0.0f));

	//nCursorPos>0とするために、0<=i<nMaxPosではなく、nMaxPos<=i<2*nMaxPosとする
	for(int i = nMaxPos; i < 2 * nMaxPos; i++){
		//初期描画は上から順にnHalfPos,nHalfPos+1,…,nMaxPos,0,1,…,nHalfPos-1,nHalfPosと表示
		int nDrawCursorPos = (i + nHalfPos - nNowPos) % nMaxPos;

		float fStartX = 200.0f + fDiffX * nDrawCursorPos;
		float fStartY = 200.0f + fDiffY * nDrawCursorPos;
		float fEndX = fStartX + fMoveX;
		float fEndY = fStartY + fMoveY;
		float fAlpha = (nNowPos == (i % nMaxPos) ? m_pAnime->GetAnimationSlowDownLaps(m_nAnimeLoopIndex, 0.0f, 1.0f, 0.0f) : 1.0f);
		float fAnimeX = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, fStartX, fEndX);
		float fAnimeY = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, fStartY, fEndY);

		if(
			(m_bLeftKeyPush == TRUE && nDrawCursorPos == nMaxPos - 1) ||
			(m_bRightKeyPush == TRUE && nDrawCursorPos == 0)
			){
			//画面外にフェードアウト
			fAlpha = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, 1.0f, 0.0f);

			//同時に同じ文字(画像)を上or下からフェードイン
			float fNewStartX = (m_bLeftKeyPush == TRUE ? 200.0f - fDiffX : 200.0f + nMaxPos * fDiffX);
			float fNewStartY = (m_bLeftKeyPush == TRUE ? 200.0f - fDiffY : 200.0f + nMaxPos * fDiffY);
			float fNewEndX = fNewStartX + fMoveX;
			float fNewEndY = fNewStartY + fMoveY;
			float fNewAlpha = 1.0f - fAlpha;
			float fNewAnimeX = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, fNewStartX, fNewEndX);
			float fNewAnimeY = m_pAnime->GetAnimationSlowDown(m_nAnimeIndex, fNewStartY, fNewEndY);
			unsigned int nNewColor = GetColor((int)(255 * fNewAlpha), (int)(255 * fNewAlpha), (int)(255 * fNewAlpha));
			DrawStringF(fNewAnimeX, fNewAnimeY, asList[i % nMaxPos].c_str(), nNewColor);
		}

		unsigned int nColor = GetColor((int)(255 * fAlpha), (int)(255 * fAlpha), (int)(255 * fAlpha));
		DrawStringF(fAnimeX, fAnimeY, asList[i % nMaxPos].c_str(), nColor);
	}
}

void MenuScene::DrawFixMenu(int nPhase, std::string asList[]){
	int nNowPos = m_anCursorPos.at(nPhase);
	int nMaxPos = m_anCursorMaxPos.at(nPhase);

}