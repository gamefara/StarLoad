#include "MenuScene.h"

MenuScene::MenuScene(ISceneManager* pChanger, IResourceManager* pResource) : BaseScene(pChanger, pResource)
{
	for(int i = 0; i < (int)Level::LEVEL_UNKNOWN; i++) m_apDataSet[i] = new StructReplayData();
	m_pCursorSet = new StructCursor();
	m_pPushSet = new StructPushKey();
	m_pIndexSet = new StructIndex();
	m_pCountSet =  new StructCount();
}

MenuScene::~MenuScene(){
	for(int i = 0; i < (int)Level::LEVEL_UNKNOWN; i++) DeletePtr(m_apDataSet[i]);
	DeletePtr(m_pCursorSet);
	DeletePtr(m_pPushSet);
	DeletePtr(m_pIndexSet);
	DeletePtr(m_pCountSet);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::Initialize(){
	m_pCursorSet->m_nConfigBGMCursorPos = GetGameDataVolumeBGM() / 5;
	m_pCursorSet->m_nConfigSECursorPos = GetGameDataVolumeSE() / 5;
	for(int i = 0; i <= static_cast<int>(Level::LEVEL_HARD); i++){
		m_apDataSet[i]->m_sDateTime = GetGameDataDateTime(i);
		m_apDataSet[i]->m_nScore = GetGameDataScore(i);
		m_apDataSet[i]->m_sRank = GetGameDataRank(i);
	}
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
	if(m_pCountSet->m_nLoopAlphaNowCount == 0) m_pIndexSet->m_nAnimeLoopIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nLoopAlphaMaxCount);
	m_pCountSet->m_nLoopAlphaNowCount = (m_pCountSet->m_nLoopAlphaNowCount + 1) % m_pCountSet->m_nLoopAlphaMaxCount;
}

/// <summary>
/// カーソル移動した時の処理を行います。
/// </summary>
void MenuScene::UpdateMoveCursorAction(){
	if(m_pPushSet->m_bOKKeyPush || m_pPushSet->m_bCancelKeyPush) return;
	if(GetPhase() == static_cast<int>(Phase::PHASE_RESULT)) return;

	if(!m_pPushSet->m_bLeftKeyPush && !m_pPushSet->m_bRightKeyPush && !m_pPushSet->m_bUpKeyPush && !m_pPushSet->m_bDownKeyPush) MoveCursorKeyPushCheck();
	else{
		//回転移動のフェーズ時に使用
		if(GetPhase() == static_cast<int>(PHASE_TITLE)){
			int nMove = (m_pPushSet->m_bLeftKeyPush ? -1 : 1);
			float fDivAngle = 360.0f / m_pCursorSet->m_nMainCursorMaxPos;
			float fFromAngle = fDivAngle * m_pCursorSet->m_nMainCursorPos;
			float fToAngle = fDivAngle * (m_pCursorSet->m_nMainCursorPos + nMove);
			m_nDrawMenuNowAngle = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, fFromAngle, fToAngle);
		}

		if(!m_pAnime->IsMatchedAnimationCount(m_pIndexSet->m_nAnimeIndex, m_pCountSet->m_nMoveCursorCount)) return;
		MoveCursorProcess();
	}
}

void MenuScene::MoveCursorKeyPushCheck(){
	int bConfigExit = (m_pCursorSet->m_nConfigCursorPos == static_cast<int>(ConfigCursor::CURSOR_CONFIG_EXIT));
	if(CheckHitKey(KEY_INPUT_LEFT)){
		if(GetPhase() == static_cast<int>(Phase::PHASE_CONFIG) && bConfigExit) return;
		m_pPushSet->m_bLeftKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);

		SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		m_pResource->SetSoundsBGMVolume(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
		m_pResource->SetSoundsSEVolume(m_pCursorSet->m_nConfigSECursorPos * 5);
	}
	else if(CheckHitKey(KEY_INPUT_RIGHT)){
		if(GetPhase() == static_cast<int>(Phase::PHASE_CONFIG) && bConfigExit) return;
		m_pPushSet->m_bRightKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);

		SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		m_pResource->SetSoundsBGMVolume(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
		m_pResource->SetSoundsSEVolume(m_pCursorSet->m_nConfigSECursorPos * 5);
	}
	else if(CheckHitKey(KEY_INPUT_UP)){
		if(GetPhase() != static_cast<int>(Phase::PHASE_CONFIG)) return;
		m_pPushSet->m_bUpKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);
	}
	else if(CheckHitKey(KEY_INPUT_DOWN)){
		if(GetPhase() != static_cast<int>(Phase::PHASE_CONFIG)) return;
		m_pPushSet->m_bDownKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);
	}
}

void MenuScene::MoveCursorProcess(){
	int& nCursorPos = GetCursorPos();
	const int& nCursorMaxPos = GetCursorMaxPos();
	int& nConfigCursorPos = GetVolumeCursorPos();
	const int& nConfigCursorMaxPos = GetVolumeCursorMaxPos();

	if(m_pPushSet->m_bLeftKeyPush){
		m_pPushSet->m_bLeftKeyPush = FALSE;
		if(GetPhase() != static_cast<int>(Phase::PHASE_CONFIG)) nCursorPos = (nCursorPos + (nCursorMaxPos - 1)) % nCursorMaxPos;
		else if(nConfigCursorPos > 0) nConfigCursorPos--;
	}
	else if(m_pPushSet->m_bRightKeyPush){
		m_pPushSet->m_bRightKeyPush = FALSE;
		if(GetPhase() != static_cast<int>(Phase::PHASE_CONFIG)) nCursorPos = (nCursorPos + 1) % nCursorMaxPos;
		else if(nConfigCursorPos < nConfigCursorMaxPos) nConfigCursorPos++;
	}
	else if(m_pPushSet->m_bUpKeyPush){
		m_pPushSet->m_bUpKeyPush = FALSE;
		nCursorPos = (nCursorPos + (nCursorMaxPos - 1)) % nCursorMaxPos;
	}
	else if(m_pPushSet->m_bDownKeyPush){
		m_pPushSet->m_bDownKeyPush = FALSE;
		nCursorPos = (nCursorPos + 1) % nCursorMaxPos;
	}
}

int& MenuScene::GetCursorPos(){
	switch(GetPhase()){
	case static_cast<int>(Phase::PHASE_TITLE):
		return m_pCursorSet->m_nMainCursorPos;
	case static_cast<int>(Phase::PHASE_LEVEL):
		return m_pCursorSet->m_nLevelCursorPos;
	case static_cast<int>(Phase::PHASE_CONFIG):
		return m_pCursorSet->m_nConfigCursorPos;
	default:
		return m_pCursorSet->m_nManualCursorPos;
	}
}

const int& MenuScene::GetCursorMaxPos(){
	switch(GetPhase()){
	case static_cast<int>(Phase::PHASE_TITLE):
		return m_pCursorSet->m_nMainCursorMaxPos;
	case static_cast<int>(Phase::PHASE_LEVEL):
		return m_pCursorSet->m_nLevelCursorMaxPos;
	case static_cast<int>(Phase::PHASE_CONFIG):
		return m_pCursorSet->m_nConfigCursorMaxPos;
	default:
		return m_pCursorSet->m_nManualCursorMaxPos;
	}
}

int& MenuScene::GetVolumeCursorPos(){
	switch(m_pCursorSet->m_nConfigCursorPos){
	case static_cast<int>(ConfigCursor::CURSOR_CONFIG_VOLUME_BGM):
		return m_pCursorSet->m_nConfigBGMCursorPos;
	case static_cast<int>(ConfigCursor::CURSOR_CONFIG_VOLUME_SE):
		return m_pCursorSet->m_nConfigSECursorPos;
	default:
		return m_pCursorSet->m_nConfigCursorPos;
	}
}

const int& MenuScene::GetVolumeCursorMaxPos(){
	switch(m_pCursorSet->m_nConfigCursorPos){
	case static_cast<int>(ConfigCursor::CURSOR_CONFIG_VOLUME_BGM):
		return m_pCursorSet->m_nConfigBGMCursorMaxPos;
	case static_cast<int>(ConfigCursor::CURSOR_CONFIG_VOLUME_SE):
		return m_pCursorSet->m_nConfigSECursorMaxPos;
	default:
		return m_pCursorSet->m_nConfigCursorPos;
	}
}

/// <summary>
/// 決定キーを押したときの処理を行います。
/// </summary>
void MenuScene::UpdateGoKeyAction(){
	if(m_pPushSet->m_bLeftKeyPush || m_pPushSet->m_bRightKeyPush || m_pPushSet->m_bCancelKeyPush) return;

	const int& nPhase = GetPhase();
	if(!m_pPushSet->m_bOKKeyPush){
		if(!CheckHitKey(KEY_INPUT_Z)) return;
		if(nPhase == static_cast<int>(Phase::PHASE_CONFIG) && m_pCursorSet->m_nConfigCursorPos == (int)ConfigCursor::CURSOR_CONFIG_EXIT){
			m_pPushSet->m_bCancelKeyPush = TRUE;
			SetPhase(PHASE_TITLE);
		}
		else m_pPushSet->m_bOKKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMovePhaseCount);
	}
	else{
		if(!m_pAnime->IsMatchedAnimationCount(m_pIndexSet->m_nAnimeIndex, m_pCountSet->m_nMovePhaseCount)) return;
		m_pPushSet->m_bOKKeyPush = FALSE;

		switch(nPhase){
		case static_cast<int>(Phase::PHASE_TITLE):
			//CursorPos-> 0:Level, 1:Result, 2:Config, 3:Exit
			SetPhase((Phase)(m_pCursorSet->m_nMainCursorPos + 1));
			break;
		case static_cast<int>(Phase::PHASE_LEVEL):
			SetPhase(PHASE_GAMEPLAY);
			SetLevel(static_cast<Level>(m_pCursorSet->m_nLevelCursorPos));
			m_pSceneChanger->ChangeScene(SCENE_PRACTICE);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 戻るキーを押したときの処理を行います。
/// </summary>
void MenuScene::UpdateCancelKeyAction(){
	if(m_pPushSet->m_bLeftKeyPush || m_pPushSet->m_bRightKeyPush || m_pPushSet->m_bOKKeyPush) return;
	if(!m_pPushSet->m_bCancelKeyPush){
		if(!CheckHitKey(KEY_INPUT_X) || GetPhase() == static_cast<int>(Phase::PHASE_TITLE)) return;
		m_pPushSet->m_bCancelKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMovePhaseCount);
		SetPhase(PHASE_TITLE);
	}
	else{
		if(!m_pAnime->IsMatchedAnimationCount(m_pIndexSet->m_nAnimeIndex, m_pCountSet->m_nMovePhaseCount)) return;
		m_pPushSet->m_bCancelKeyPush = FALSE;
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::DrawLoop(){
	const int& nPhase = GetPhase();
	if(nPhase >= PHASE_EXIT) return;

	//背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawGraph(0, 0, m_nBackGroundHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	switch(nPhase){
	case static_cast<int>(Phase::PHASE_TITLE):
		DrawPhaseTitle();
		break;
	case static_cast<int>(Phase::PHASE_LEVEL):
		DrawPhaseLevel();
		break;
	case static_cast<int>(Phase::PHASE_RESULT):
		DrawPhaseResult();
		break;
	case static_cast<int>(Phase::PHASE_CONFIG):
		DrawPhaseConfig();
		break;
	case static_cast<int>(Phase::PHASE_MANUAL):
		DrawPhaseManual();
		break;
	default:
		break;
	}
}

void MenuScene::DrawPhaseTitle(){
	int& nCursorPos = m_pCursorSet->m_nMainCursorPos;
	float fMoveX = 0.0f;
	if(m_pPushSet->m_bOKKeyPush) fMoveX = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, 0.0f, 500.0f);
	else if(m_pPushSet->m_bCancelKeyPush) fMoveX = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, 500.0f, 0.0f);

	//楕円表示
	float fPi = static_cast<float>(M_PI);
	float fCycle = (2 * fPi) / 360;
	float fRadian = fCycle * m_nDrawMenuNowAngle + fPi / 2;
	float fDiffRadian = (2 * fPi) / static_cast<int>(MenuCursor::CURSOR_MENU_COUNT);

	//選択肢
	for(int i = 0; i < m_pCursorSet->m_nMainCursorMaxPos; i++){
		float fMoveRadian = fRadian - fDiffRadian * i;
		float fX = WindowCenX + 200.0f * cosf(fMoveRadian);
		float fY = WindowCenY + 40.0f * sinf(fMoveRadian);

		int nAlpha = 255;
		if(m_pPushSet->m_bOKKeyPush){
			if(i == nCursorPos) nAlpha = (GetNowCount() % 2 == 0 ? 255 : 0);
			else nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
		}
		else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		double dRatio = 1 - 0.2 * sin(fMoveRadian + fPi);
		int nHandle = GetDrawMenuHandle(i);
		DrawRotaGraphF(fX, fY, dRatio, 0, nHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//カーソル
	double dAddAngle = 0;
	if(m_pPushSet->m_bLeftKeyPush || m_pPushSet->m_bRightKeyPush) dAddAngle = m_pAnime->GetAnimationSlowDownLaps(m_pIndexSet->m_nAnimeIndex, 0, (m_pPushSet->m_bLeftKeyPush ? 1 : -1) * 90, 0);
	double dRadian = (M_PI / 360) * dAddAngle;
	int nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_CURSOR);
	DrawRotaGraphF(WindowCenX - fMoveX, WindowCenY + 20.0f, 1, M_PI - dRadian, nHandle, TRUE);
	DrawRotaGraphF(WindowCenX + fMoveX, WindowCenY + 60.0f, 1, dRadian, nHandle, TRUE);

	//説明
	nHandle = GetDrawMenuDescriptionHandle(m_pCursorSet->m_nMainCursorPos);
	int nAlpha = 255;
	if(m_pPushSet->m_bOKKeyPush) nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
	else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

int MenuScene::GetDrawMenuHandle(int nIndex){
	int nHandle;
	switch(nIndex){
	case static_cast<int>(MenuCursor::CURSOR_MENU_PRACTICE) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_RESULT):
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_RESULT);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_CONFIG):
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_CONFIG);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_MANUAL) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_MANUAL);
		break;
	default:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_EXIT);
		break;
	}
	return nHandle;
}

int MenuScene::GetDrawMenuDescriptionHandle(int nIndex){
	int nHandle;
	switch(nIndex){
	case static_cast<int>(MenuCursor::CURSOR_MENU_PRACTICE) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_DESCRIPTION);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_RESULT) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_RESULT_DESCRIPTION);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_CONFIG) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_CONFIG_DESCRIPTION);
		break;
	case static_cast<int>(MenuCursor::CURSOR_MENU_MANUAL) :
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_MANUAL_DESCRIPTION);
		break;
	default:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_EXIT_DESCRIPTION);
		break;
	}
	return nHandle;
}

void MenuScene::DrawPhaseLevel(){
	int& nPos = m_pCursorSet->m_nLevelCursorPos;
	const int& nMaxPos = m_pCursorSet->m_nLevelCursorMaxPos;
	int nHalfPos = (nMaxPos - 1) / 2;	//nMaxPosが偶数の時は切り捨てnMaxPos=4 => nHalfPos=1)
	float fDiffX = 200.0f;
	float fMoveX = (m_pPushSet->m_bLeftKeyPush ? fDiffX : (m_pPushSet->m_bRightKeyPush ? -fDiffX : 0.0f));
	float fY = static_cast<float>(WindowCenY);
	int nHandle;

	//nPos>0とするために、0<=i<nMaxPosではなく、nMaxPos<=i<2*nMaxPosとする
	for(int i = nMaxPos; i < 2 * nMaxPos; i++){
		nHandle = GetDrawLevelHandle(i % nMaxPos);
		int j = (i + nHalfPos - nPos) % nMaxPos;

		float fStartX = WindowCenX + fDiffX * (j - 1);
		float fEndX = fStartX + fMoveX;
		int nFlash = m_pAnime->GetAnimationSlowDownLaps(m_pIndexSet->m_nAnimeLoopIndex, 0, 255, 0);
		int nAlpha = (nPos == (i % nMaxPos) ? nFlash : 255);
		float fAnimeX = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, fStartX, fEndX);

		int bFadeOut = (m_pPushSet->m_bLeftKeyPush && j == nMaxPos - 1) || (m_pPushSet->m_bRightKeyPush && j == 0);
		if(bFadeOut){
			//(-2), -1, 0, 1, (2)の-2,2の部分を表示
			float fNewStartX = (m_pPushSet->m_bLeftKeyPush ? WindowCenX - 2 * fDiffX : WindowCenX + (nMaxPos - 1) * fDiffX);
			float fNewEndX = fNewStartX + fMoveX;
			nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
			int nNewAlpha = 255 - nAlpha;
			float fNewAnimeX = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, fNewStartX, fNewEndX);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, nNewAlpha);
			DrawRotaGraphF(fNewAnimeX, fY, 1, 0, nHandle, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		DrawRotaGraphF(fAnimeX, fY, 1, 0, nHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//説明書き
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

int MenuScene::GetDrawLevelHandle(int nIndex){
	int nHandle;
	switch(nIndex){
	case static_cast<int>(LevelCursor::CURSOR_LEVEL_BEGINNER):
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_BEGINNER);
		break;
	case static_cast<int>(LevelCursor::CURSOR_LEVEL_STANDARD):
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_STANDARD);
		break;
	default:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_HARD);
		break;
	}
	return nHandle;
}

void MenuScene::DrawPhaseResult(){
	//☆マーク
	int nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_YELLOW);
	double dAngle = sin((M_PI / 180) * (GetNowCount() / 2));
	for(int i = 0; i < 3; i++) DrawRotaGraph(40, 50 * (i + 1) + 95, 0.5, dAngle, nHandle, TRUE);

	int nBaseX = 100;
	int nBaseY = 150;

	//レベル表示
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_BEGINNER);
	DrawRotaGraph(nBaseX, nBaseY, 1, 0, nHandle, TRUE);

	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_STANDARD);
	DrawRotaGraph(nBaseX, nBaseY + 50, 1, 0, nHandle, TRUE);

	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_PRACTICE_LEVEL_HARD);
	DrawRotaGraph(nBaseX, nBaseY + 100, 1, 0, nHandle, TRUE);

	//ヘッダ
	unsigned int nColor = GetColor(255, 255, 255);
	DrawString(nBaseX + 150, nBaseY - 60, "Date", nColor);
	DrawString(nBaseX + 310, nBaseY - 60, "Score", nColor);
	DrawString(nBaseX + 410, nBaseY - 60, "Rank", nColor);

	//実績
	for(int i = 0; i <= static_cast<int>(Level::LEVEL_HARD); i++){
		int nY = nBaseY + 50 * i - 10;
		DrawString(nBaseX + 80, nY, GetGameDataDateTime(i).c_str(), nColor);
		DrawFormatString(nBaseX + 300, nY, nColor, "%d", GetGameDataScore(i));
		DrawString(nBaseX + 420, nY, GetGameDataRank(i).c_str(), nColor);
	}

	//説明書き
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_RESULT_DISPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

void MenuScene::DrawPhaseConfig(){
	int nGaugeMinX = 250;
	int nGaugeMaxX = 500;
	int nColor = GetColor(255, 255, 255);
	float fBGMPercent = static_cast<float>(m_pCursorSet->m_nConfigBGMCursorPos) / static_cast<float>(m_pCursorSet->m_nConfigBGMCursorMaxPos);
	float fSEPercent = static_cast<float>(m_pCursorSet->m_nConfigSECursorPos) / static_cast<float>(m_pCursorSet->m_nConfigSECursorMaxPos);
	int nBGMGaugeX = nGaugeMinX + static_cast<int>((nGaugeMaxX - nGaugeMinX) * fBGMPercent);
	int nSEGaugeX = nGaugeMinX + static_cast<int>((nGaugeMaxX - nGaugeMinX) * fSEPercent);

	//ヘッダ
	int nBaseY = 180;
	int nHandle = Invalid;
	int nImage = static_cast<int>(ResourceImage::IMAGE_TITLE_CONFIG_BGM);
	for(int i = 0; i < 3; i++){
		//選択中
		if(i == m_pCursorSet->m_nConfigCursorPos){
			double dAngle = sin((M_PI / 180) * (GetNowCount() / 2));
			nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_YELLOW);
			DrawRotaGraph(nGaugeMinX - 110, nBaseY + i * 40, 0.5, dAngle, nHandle, TRUE);
		}
		nHandle = GetGraphicsHandle(static_cast<ResourceImage>(i + nImage));
		DrawRotaGraph(nGaugeMinX - 50, nBaseY + i * 40, 0.7, 0, nHandle, TRUE);
	}

	//音量ゲージ
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_LOAD_GAUGE_EMPTY);
	DrawExtendGraph(nGaugeMinX, nBaseY - 10, nGaugeMaxX, nBaseY + 10, nHandle, TRUE);
	DrawExtendGraph(nGaugeMinX, nBaseY + 27, nGaugeMaxX, nBaseY + 47, nHandle, TRUE);
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_LOAD_GAUGE_FILL);
	DrawExtendGraph(nGaugeMinX, nBaseY - 10, nBGMGaugeX, nBaseY + 10, nHandle, TRUE);
	DrawExtendGraph(nGaugeMinX, nBaseY + 27, nSEGaugeX, nBaseY + 47, nHandle, TRUE);

	//音量％表示
	DrawFormatString(nGaugeMaxX + 20, nBaseY - 10, nColor, "%.2f%%", fBGMPercent * 100);
	DrawFormatString(nGaugeMaxX + 20, nBaseY + 35, nColor, "%.2f%%", fSEPercent * 100);

	//説明書き
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_CONFIG_DISPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

void MenuScene::DrawPhaseManual(){
	int nPage = m_pCursorSet->m_nManualCursorPos + static_cast<int>(ResourceImage::IMAGE_TITLE_MANUAL_PAGE1);
	int nHandle = GetGraphicsHandle(static_cast<ResourceImage>(nPage));
	DrawExtendGraph(WindowMinX + 50, WindowMinY + 50, WindowMaxX - 50, WindowCenY + 50, nHandle, TRUE);

	//ページ数表示
	int nY = static_cast<int>(WindowCenY * 1.65);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawFormatString(WindowCenX - 30, nY - 80, GetColor(255, 255, 255), "%d / %d", m_pCursorSet->m_nManualCursorPos + 1, m_pCursorSet->m_nManualCursorMaxPos);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//説明書き
	nHandle = GetGraphicsHandle(ResourceImage::IMAGE_TITLE_MANUAL_DESPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, nY, 1, 0, nHandle, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void MenuScene::Finalize(){
	SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
	SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
}
