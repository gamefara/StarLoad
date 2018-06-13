#include "MenuScene.h"

MenuScene::MenuScene(ISceneManager* pChanger, IResourceManager* pResource) : BaseScene(pChanger, pResource)
{
	for(int i = 0; i < static_cast<int>(CURSOR_LEVEL_COUNT); i++) m_apDataSet[i] = new StructReplayData();
	m_pCursorSet = new StructCursor();
	m_pPushSet = new StructPushKey();
	m_pIndexSet = new StructIndex();
	m_pCountSet =  new StructCount();
}

MenuScene::~MenuScene(){
	for(int i = 0; i < static_cast<int>(CURSOR_LEVEL_COUNT); i++) DeletePtr(m_apDataSet[i]);
	DeletePtr(m_pCursorSet);
	DeletePtr(m_pPushSet);
	DeletePtr(m_pIndexSet);
	DeletePtr(m_pCountSet);
}

/// <summary>
/// 初期化処理
/// </summary>
void MenuScene::Initialize(){
	m_pCursorSet->m_nConfigBGMCursorPos = GetGameDataVolumeBGM() / 5;
	m_pCursorSet->m_nConfigSECursorPos = GetGameDataVolumeSE() / 5;
	for(int i = 0; i <= static_cast<int>(LEVEL_HARD); i++){
		m_apDataSet[i]->m_sDateTime = GetGameDataDateTime(i);
		m_apDataSet[i]->m_nScore = GetGameDataScore(i);
		m_apDataSet[i]->m_sRank = GetGameDataRank(i);
	}
	const int& nHandle = GetSoundsHandle(SOUND_BGM_TITLE);
	if(!CheckSoundMem(nHandle)) PlaySoundMem(nHandle, DX_PLAYTYPE_LOOP);
}

/// <summary>
/// 更新処理
/// </summary>
void MenuScene::Update(){
	UpdateAlphaCount();
	UpdateMoveCursorAction();
	UpdateGoKeyAction();
	UpdateCancelKeyAction();
}

/// <summary>
/// カーソル移動した時の処理をします。
/// </summary>
void MenuScene::UpdateMoveCursorAction(){
	if(m_pPushSet->m_bOKKeyPush || m_pPushSet->m_bCancelKeyPush) return;
	const int& nPhase = GetPhase();
	if(nPhase == static_cast<int>(PHASE_RESULT) || nPhase == static_cast<int>(PHASE_CREDIT)) return;

	if(!m_pPushSet->m_bLeftKeyPush && !m_pPushSet->m_bRightKeyPush && !m_pPushSet->m_bUpKeyPush && !m_pPushSet->m_bDownKeyPush) MoveCursorKeyPushCheck();
	else{
		//回転移動のフェーズ時に使用
		if(nPhase == static_cast<int>(PHASE_TITLE)){
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

/// <summary>
/// カーソル移動チェックをします。
/// </summary>
void MenuScene::MoveCursorKeyPushCheck(){
	const int& bConfigExit = (m_pCursorSet->m_nConfigCursorPos == static_cast<int>(CURSOR_CONFIG_EXIT));
	if(CheckHitKey(KEY_INPUT_LEFT)){
		if(GetPhase() == static_cast<int>(PHASE_CONFIG) && bConfigExit) return;
		m_pPushSet->m_bLeftKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);

		SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		m_pResource->SetSoundsBGMVolume(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
		m_pResource->SetSoundsSEVolume(m_pCursorSet->m_nConfigSECursorPos * 5);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
	else if(CheckHitKey(KEY_INPUT_RIGHT)){
		if(GetPhase() == static_cast<int>(PHASE_CONFIG) && bConfigExit) return;
		m_pPushSet->m_bRightKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);

		SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		m_pResource->SetSoundsBGMVolume(m_pCursorSet->m_nConfigBGMCursorPos * 5);
		SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
		m_pResource->SetSoundsSEVolume(m_pCursorSet->m_nConfigSECursorPos * 5);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
	else if(CheckHitKey(KEY_INPUT_UP)){
		if(GetPhase() != static_cast<int>(PHASE_CONFIG)) return;
		m_pPushSet->m_bUpKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
	else if(CheckHitKey(KEY_INPUT_DOWN)){
		if(GetPhase() != static_cast<int>(PHASE_CONFIG)) return;
		m_pPushSet->m_bDownKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMoveCursorCount);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
}

/// <summary>
/// カーソル移動処理をします。
/// </summary>
void MenuScene::MoveCursorProcess(){
	int& nCursorPos = GetCursorPos();
	const int& nCursorMaxPos = GetCursorMaxPos();
	int& nConfigCursorPos = GetVolumeCursorPos();
	const int& nConfigCursorMaxPos = GetVolumeCursorMaxPos();

	if(m_pPushSet->m_bLeftKeyPush){
		m_pPushSet->m_bLeftKeyPush = FALSE;
		if(GetPhase() != static_cast<int>(PHASE_CONFIG)) nCursorPos = (nCursorPos + (nCursorMaxPos - 1)) % nCursorMaxPos;
		else if(nConfigCursorPos > 0) nConfigCursorPos--;
	}
	else if(m_pPushSet->m_bRightKeyPush){
		m_pPushSet->m_bRightKeyPush = FALSE;
		if(GetPhase() != static_cast<int>(PHASE_CONFIG)) nCursorPos = (nCursorPos + 1) % nCursorMaxPos;
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

/// <summary>
/// 操作するカーソルを取得します。
/// </summary>
/// <returns>操作するカーソル</returns>
int& MenuScene::GetCursorPos(){
	switch(GetPhase()){
	case static_cast<int>(PHASE_TITLE):
		return m_pCursorSet->m_nMainCursorPos;
	case static_cast<int>(PHASE_LEVEL):
		return m_pCursorSet->m_nLevelCursorPos;
	case static_cast<int>(PHASE_CONFIG):
		return m_pCursorSet->m_nConfigCursorPos;
	default:
		return m_pCursorSet->m_nManualCursorPos;
	}
}

/// <summary>
/// 操作するカーソルの最大値を取得します。
/// </summary>
/// <returns>操作するカーソル最大値</returns>
const int& MenuScene::GetCursorMaxPos(){
	switch(GetPhase()){
	case static_cast<int>(PHASE_TITLE):
		return m_pCursorSet->m_nMainCursorMaxPos;
	case static_cast<int>(PHASE_LEVEL):
		return m_pCursorSet->m_nLevelCursorMaxPos;
	case static_cast<int>(PHASE_CONFIG):
		return m_pCursorSet->m_nConfigCursorMaxPos;
	default:
		return m_pCursorSet->m_nManualCursorMaxPos;
	}
}

/// <summary>
/// 操作するコンフィグ用カーソルを取得します。
/// </summary>
/// <returns>操作するコンフィグ用カーソル</returns>
int& MenuScene::GetVolumeCursorPos(){
	switch(m_pCursorSet->m_nConfigCursorPos){
	case static_cast<int>(CURSOR_CONFIG_VOLUME_BGM):
		return m_pCursorSet->m_nConfigBGMCursorPos;
	case static_cast<int>(CURSOR_CONFIG_VOLUME_SE):
		return m_pCursorSet->m_nConfigSECursorPos;
	default:
		return m_pCursorSet->m_nConfigCursorPos;
	}
}

/// <summary>
/// 操作するコンフィグ用カーソルの最大値を取得します。
/// </summary>
/// <returns>操作するコンフィグ用カーソル最大値</returns>
const int& MenuScene::GetVolumeCursorMaxPos(){
	switch(m_pCursorSet->m_nConfigCursorPos){
	case static_cast<int>(CURSOR_CONFIG_VOLUME_BGM):
		return m_pCursorSet->m_nConfigBGMCursorMaxPos;
	case static_cast<int>(CURSOR_CONFIG_VOLUME_SE):
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
	if(nPhase > static_cast<int>(PHASE_LEVEL) && nPhase != static_cast<int>(PHASE_CONFIG)) return;

	if(!m_pPushSet->m_bOKKeyPush){
		if(!CheckHitKey(KEY_INPUT_Z)) return;
		if(nPhase == static_cast<int>(PHASE_CONFIG) && m_pCursorSet->m_nConfigCursorPos == static_cast<int>(CURSOR_CONFIG_EXIT)){
			m_pPushSet->m_bCancelKeyPush = TRUE;
			SetPhase(PHASE_TITLE);
			PlaySoundMem(GetSoundsHandle(SOUND_SE_CANCEL), DX_PLAYTYPE_BACK);
		}
		else{
			m_pPushSet->m_bOKKeyPush = TRUE;
			if(nPhase == static_cast<int>(PHASE_LEVEL)) PlaySoundMem(GetSoundsHandle(SOUND_SE_LOAD_OK), DX_PLAYTYPE_BACK);
			else if(nPhase == static_cast<int>(PHASE_TITLE)) PlaySoundMem(GetSoundsHandle(SOUND_SE_TITLE_OK), DX_PLAYTYPE_BACK);
		}
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMovePhaseCount);
	}
	else{
		//ゲーム開始前にBGM音量フェードアウト
		const int& nHandle = GetSoundsHandle(SOUND_BGM_TITLE);
		if(nPhase == static_cast<int>(PHASE_LEVEL) || nPhase == static_cast<int>(PHASE_EXIT)){
			const int& nBegin = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeBGM());
			const int& nVolume = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, nBegin, 0);
			if(CheckSoundMem(nHandle)) ChangeVolumeSoundMem(nVolume, nHandle);
		}
		if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nAnimeIndex)) return;
		m_pPushSet->m_bOKKeyPush = FALSE;

		switch(nPhase){
		case static_cast<int>(PHASE_TITLE):
			SetPhase(static_cast<Phase>(m_pCursorSet->m_nMainCursorPos + 1));
			break;
		case static_cast<int>(PHASE_LEVEL):
			SetPhase(PHASE_GAMEPLAY);
			SetLevel(static_cast<Level>(m_pCursorSet->m_nLevelCursorPos));
			m_pSceneChanger->ChangeScene(SCENE_PRACTICE);
			if(CheckSoundMem(nHandle)) StopSoundMem(nHandle);
			break;
		case static_cast<int>(PHASE_EXIT):
			if(CheckSoundMem(nHandle)) StopSoundMem(nHandle);
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
		if(!CheckHitKey(KEY_INPUT_X) || GetPhase() == static_cast<int>(PHASE_TITLE)) return;
		m_pPushSet->m_bCancelKeyPush = TRUE;
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pCountSet->m_nMovePhaseCount);
		SetPhase(PHASE_TITLE);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CANCEL), DX_PLAYTYPE_BACK);
	}
	else{
		if(!m_pAnime->IsMatchedAnimationCount(m_pIndexSet->m_nAnimeIndex, m_pCountSet->m_nMovePhaseCount)) return;
		m_pPushSet->m_bCancelKeyPush = FALSE;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void MenuScene::DrawLoop(){
	const int& nPhase = GetPhase();
	if(nPhase >= static_cast<int>(PHASE_EXIT)) return;

	//背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawGraph(0, 0, m_nBackGroundHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	switch(nPhase){
	case static_cast<int>(PHASE_TITLE):
		DrawPhaseTitle();
		break;
	case static_cast<int>(PHASE_LEVEL):
		DrawPhaseLevel();
		break;
	case static_cast<int>(PHASE_RESULT):
		DrawPhaseResult();
		break;
	case static_cast<int>(PHASE_CONFIG):
		DrawPhaseConfig();
		break;
	case static_cast<int>(PHASE_MANUAL):
		DrawPhaseManual();
		break;
	case static_cast<int>(PHASE_CREDIT):
		DrawPhaseCredit();
		break;
	default:
		break;
	}
}

/// <summary>
/// タイトル画面を描画します。
/// </summary>
void MenuScene::DrawPhaseTitle(){
	int& nCursorPos = m_pCursorSet->m_nMainCursorPos;
	float fMoveX = 0.0f;
	if(m_pPushSet->m_bOKKeyPush) fMoveX = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, 0.0f, 500.0f);
	else if(m_pPushSet->m_bCancelKeyPush) fMoveX = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, 500.0f, 0.0f);
	float fPi = static_cast<float>(M_PI);
	float fCycle = (2 * fPi) / 360;
	float fRadian = fCycle * m_nDrawMenuNowAngle + (fPi / 2);

	//タイトル
	int nHandle = GetGraphicsHandle(IMAGE_TITLE_DISPLAY);
	int nAlpha = 255;
	if(m_pPushSet->m_bOKKeyPush) nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
	else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);
	const int anX[] = { WindowCenX / 2, WindowCenX / 2 * 3, WindowCenX / 2 * 3, WindowCenX / 2 };
	const int anY[] = { 50, 50, WindowCenY - 100, WindowCenY - 100 };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	DrawExtendGraph(anX[0], anY[0], anX[2], anY[2], nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//タイトル周りの星
	const int& nBaseHandle = static_cast<int>(IMAGE_PRACTICE_STAR_RED);
	for(int i = 0; i < 4; i++){
		nHandle = GetGraphicsHandle(static_cast<ResourceImage>(nBaseHandle + i));
		nAlpha = 255;
		if(m_pPushSet->m_bOKKeyPush) nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
		else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		DrawRotaGraph(anX[i], anY[i] - 10, 0.5, fCycle * (GetNowCount() / 2), nHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//楕円表示
	float fDiffRadian = (2 * fPi) / static_cast<int>(CURSOR_MENU_COUNT);

	//選択肢
	for(int i = 0; i < m_pCursorSet->m_nMainCursorMaxPos; i++){
		float fMoveRadian = fRadian - fDiffRadian * i;
		float fX = WindowCenX + 200.0f * cosf(fMoveRadian);
		float fY = WindowCenY + 40.0f * sinf(fMoveRadian);

		nAlpha = 255;
		if(m_pPushSet->m_bOKKeyPush){
			if(i == nCursorPos) nAlpha = (GetNowCount() % 2 == 0 ? 255 : 0);
			else nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
		}
		else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		double dRatio = 1 - 0.4 * sin(fMoveRadian + fPi);
		nHandle = GetDrawMenuHandle(i);
		DrawRotaGraphF(fX, fY, dRatio, 0, nHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//カーソル
	double dAddAngle = 0;
	if(m_pPushSet->m_bLeftKeyPush || m_pPushSet->m_bRightKeyPush) dAddAngle = m_pAnime->GetAnimationSlowDownLaps(m_pIndexSet->m_nAnimeIndex, 0, (m_pPushSet->m_bLeftKeyPush ? 1 : -1) * 90, 0);
	double dRadian = (M_PI / 360) * dAddAngle;
	nHandle = GetGraphicsHandle(IMAGE_TITLE_CURSOR);
	DrawRotaGraphF(WindowCenX - fMoveX, WindowCenY + 20.0f, 1, M_PI - dRadian, nHandle, TRUE);
	DrawRotaGraphF(WindowCenX + fMoveX, WindowCenY + 60.0f, 1, dRadian, nHandle, TRUE);

	//説明
	nHandle = GetDrawMenuDescriptionHandle(m_pCursorSet->m_nMainCursorPos);
	nAlpha = 255;
	if(m_pPushSet->m_bOKKeyPush) nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
	else if(m_pPushSet->m_bCancelKeyPush) nAlpha = m_pAnime->GetAnimationAccelerate(m_pIndexSet->m_nAnimeIndex, 0, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

/// <summary>
/// タイトル画面のメニュー選択画像ハンドルを取得します。
/// </summary>
/// <param name="nIndex">カーソル位置</param>
/// <returns>画像ハンドル</returns>
int MenuScene::GetDrawMenuHandle(const int& nIndex){
	int nHandle = Invalid;
	switch(nIndex){
	case static_cast<int>(CURSOR_MENU_PRACTICE):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE);
		break;
	case static_cast<int>(CURSOR_MENU_RESULT):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_RESULT);
		break;
	case static_cast<int>(CURSOR_MENU_CONFIG):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_CONFIG);
		break;
	case static_cast<int>(CURSOR_MENU_MANUAL):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_MANUAL);
		break;
	case static_cast<int>(CURSOR_MENU_CREDIT):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_CREDIT);
		break;
	default:
		nHandle = GetGraphicsHandle(IMAGE_TITLE_EXIT);
		break;
	}
	return nHandle;
}

/// <summary>
/// タイトル画面のメニュー選択説明画像ハンドルを取得します。
/// </summary>
/// <param name="nIndex">カーソル位置</param>
/// <returns>画像ハンドル</returns>
int MenuScene::GetDrawMenuDescriptionHandle(const int& nIndex){
	int nHandle = Invalid;
	switch(nIndex){
	case static_cast<int>(CURSOR_MENU_PRACTICE):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE_DESCRIPTION);
		break;
	case static_cast<int>(CURSOR_MENU_RESULT):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_RESULT_DESCRIPTION);
		break;
	case static_cast<int>(CURSOR_MENU_CONFIG):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_CONFIG_DESCRIPTION);
		break;
	case static_cast<int>(CURSOR_MENU_MANUAL):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_MANUAL_DESCRIPTION);
		break;
	case static_cast<int>(CURSOR_MENU_CREDIT):
		nHandle = GetGraphicsHandle(IMAGE_TITLE_CREDIT_DESCRIPTION);
		break;
	default:
		nHandle = GetGraphicsHandle(IMAGE_TITLE_EXIT_DESCRIPTION);
		break;
	}
	return nHandle;
}

/// <summary>
/// レベル選択画面を描画します。
/// </summary>
void MenuScene::DrawPhaseLevel(){
	const int& nPos = m_pCursorSet->m_nLevelCursorPos;
	const int& nMaxPos = m_pCursorSet->m_nLevelCursorMaxPos;
	const int& nHalfPos = (nMaxPos - 1) / 2;	//nMaxPosが偶数の時は切り捨てnMaxPos=4 => nHalfPos=1)
	const float& fDiffX = 200.0f;
	const float& fMoveX = (m_pPushSet->m_bLeftKeyPush ? fDiffX : (m_pPushSet->m_bRightKeyPush ? -fDiffX : 0.0f));
	const float& fY = static_cast<float>(WindowCenY);
	int nHandle = Invalid;

	//nPos>0とするために、0<=i<nMaxPosではなく、nMaxPos<=i<2*nMaxPosとする
	for(int i = nMaxPos; i < 2 * nMaxPos; i++){
		//レベル表示
		const int& nIndex = (i % nMaxPos);
		nHandle = GetDrawLevelHandle(nIndex);
		const int& j = (i + nHalfPos - nPos) % nMaxPos;

		const float& fStartX = WindowCenX + fDiffX * (j - 1);
		const float& fEndX = fStartX + fMoveX;
		const int& nFlash = m_pAnime->GetAnimationSlowDownLaps(m_pIndexSet->m_nAnimeLoopIndex, 0, 255, 0);
		int nAlpha = (nPos == (nIndex) ? nFlash : 255);
		const float& fAnimeX = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, fStartX, fEndX);

		const int& bFadeOut = (m_pPushSet->m_bLeftKeyPush && j == nMaxPos - 1) || (m_pPushSet->m_bRightKeyPush && j == 0);
		if(bFadeOut){
			//(-2), -1, 0, 1, (2)の-2,2の部分を表示
			const float& fNewStartX = (m_pPushSet->m_bLeftKeyPush ? WindowCenX - 2 * fDiffX : WindowCenX + (nMaxPos - 1) * fDiffX);
			const float& fNewEndX = fNewStartX + fMoveX;
			nAlpha = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, 255, 0);
			const float& fNewAnimeX = m_pAnime->GetAnimationSlowDown(m_pIndexSet->m_nAnimeIndex, fNewStartX, fNewEndX);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - nAlpha);
			DrawRotaGraphF(fNewAnimeX, fY, 1, 0, nHandle, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		DrawRotaGraphF(fAnimeX, fY, 1, 0, nHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	}

	//レベル説明
	nHandle = GetDrawLevelDescriptionHandle(nPos);
	DrawExtendGraph(50, 50, WindowMaxX - 50, WindowCenY + 50, nHandle, TRUE);

	//説明書き
	nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

/// <summary>
/// レベル選択画面のメニュー選択画像ハンドルを取得します。
/// </summary>
/// <param name="nIndex">カーソル位置</param>
/// <returns>画像ハンドル</returns>
int MenuScene::GetDrawLevelHandle(const int& nIndex){
	int nHandle = Invalid;
	switch(nIndex){
	case static_cast<int>(CURSOR_LEVEL_BEGINNER):
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_BEGINNER);
		break;
	case static_cast<int>(CURSOR_LEVEL_STANDARD):
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_STANDARD);
		break;
	default:
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_HARD);
		break;
	}
	return nHandle;
}

int MenuScene::GetDrawLevelDescriptionHandle(const int& nIndex){
	int nHandle = Invalid;
	switch(nIndex){
	case static_cast<int>(CURSOR_LEVEL_BEGINNER):
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_BEGINNER_DESCRIPTION);
		break;
	case static_cast<int>(CURSOR_LEVEL_STANDARD):
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_STANDARD_DESCRIPTION);
		break;
	default:
		nHandle = m_pResource->GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_HARD_DESCRIPTION);
		break;
	}
	return nHandle;
}

/// <summary>
/// リザルト画面を描画します。
/// </summary>
void MenuScene::DrawPhaseResult(){
	//☆マーク
	int nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_YELLOW);
	const double dAngle = sin((M_PI / 180) * (GetNowCount() / 2));
	for(int i = 0; i < 3; i++) DrawRotaGraph(40, 50 * (i + 1) + 95, 0.5, dAngle, nHandle, TRUE);

	const int nBaseX = 100;
	const int nBaseY = 150;

	//レベル表示
	nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_BEGINNER);
	DrawRotaGraph(nBaseX, nBaseY, 1, 0, nHandle, TRUE);

	nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_STANDARD);
	DrawRotaGraph(nBaseX, nBaseY + 50, 1, 0, nHandle, TRUE);

	nHandle = GetGraphicsHandle(IMAGE_TITLE_PRACTICE_LEVEL_HARD);
	DrawRotaGraph(nBaseX, nBaseY + 100, 1, 0, nHandle, TRUE);

	//ヘッダ
	const unsigned int& nColor = GetColor(255, 255, 255);
	DrawString(nBaseX + 150, nBaseY - 60, "Date", nColor);
	DrawString(nBaseX + 300, nBaseY - 60, "Score", nColor);
	DrawString(nBaseX + 410, nBaseY - 60, "Rank", nColor);

	//実績
	for(int i = 0; i <= static_cast<int>(Level::LEVEL_HARD); i++){
		const int nY = nBaseY + 50 * i - 10;
		DrawString(nBaseX + 80, nY, GetGameDataDateTime(i).c_str(), nColor);
		DrawFormatString(nBaseX + 300, nY, nColor, "%d", GetGameDataScore(i));
		DrawString(nBaseX + 420, nY, GetGameDataRank(i).c_str(), nColor);
	}

	//説明書き
	nHandle = GetGraphicsHandle(IMAGE_TITLE_RESULT_DISPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

/// <summary>
/// コンフィグ画面を描画します。
/// </summary>
void MenuScene::DrawPhaseConfig(){
	const int nGaugeMinX = 250;
	const int nGaugeMaxX = 500;
	const float fBGMPercent = static_cast<float>(m_pCursorSet->m_nConfigBGMCursorPos) / static_cast<float>(m_pCursorSet->m_nConfigBGMCursorMaxPos);
	const float fSEPercent = static_cast<float>(m_pCursorSet->m_nConfigSECursorPos) / static_cast<float>(m_pCursorSet->m_nConfigSECursorMaxPos);
	const int nBGMGaugeX = nGaugeMinX + static_cast<int>((nGaugeMaxX - nGaugeMinX) * fBGMPercent);
	const int nSEGaugeX = nGaugeMinX + static_cast<int>((nGaugeMaxX - nGaugeMinX) * fSEPercent);

	//ヘッダ
	const int nBaseY = 180;
	int nHandle = Invalid;
	int nImage = static_cast<int>(IMAGE_TITLE_CONFIG_BGM);
	for(int i = 0; i < 3; i++){
		//選択中
		if(i == m_pCursorSet->m_nConfigCursorPos){
			const double& dAngle = sin((M_PI / 180) * (GetNowCount() / 2));
			nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_YELLOW);
			DrawRotaGraph(nGaugeMinX - 110, nBaseY + i * 40, 0.5, dAngle, nHandle, TRUE);
		}
		nHandle = GetGraphicsHandle(static_cast<ResourceImage>(i + nImage));
		DrawRotaGraph(nGaugeMinX - 50, nBaseY + i * 40, 0.7, 0, nHandle, TRUE);
	}

	//音量ゲージ
	nHandle = GetGraphicsHandle(IMAGE_LOAD_GAUGE_EMPTY);
	DrawExtendGraph(nGaugeMinX, nBaseY - 10, nGaugeMaxX, nBaseY + 10, nHandle, TRUE);
	DrawExtendGraph(nGaugeMinX, nBaseY + 27, nGaugeMaxX, nBaseY + 47, nHandle, TRUE);
	nHandle = GetGraphicsHandle(IMAGE_LOAD_GAUGE_FILL);
	DrawExtendGraph(nGaugeMinX, nBaseY - 10, nBGMGaugeX, nBaseY + 10, nHandle, TRUE);
	DrawExtendGraph(nGaugeMinX, nBaseY + 27, nSEGaugeX, nBaseY + 47, nHandle, TRUE);

	//音量％表示
	const unsigned int& nColor = GetColor(255, 255, 255);
	DrawFormatString(nGaugeMaxX + 20, nBaseY - 10, nColor, "%.2f%%", fBGMPercent * 100);
	DrawFormatString(nGaugeMaxX + 20, nBaseY + 30, nColor, "%.2f%%", fSEPercent * 100);

	//説明書き
	nHandle = GetGraphicsHandle(IMAGE_TITLE_CONFIG_DISPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

/// <summary>
/// マニュアル画面を描画します。
/// </summary>
void MenuScene::DrawPhaseManual(){
	const int nPage = m_pCursorSet->m_nManualCursorPos + static_cast<int>(IMAGE_TITLE_MANUAL_PAGE1);
	int nHandle = GetGraphicsHandle(static_cast<ResourceImage>(nPage));
	DrawExtendGraph(50, 50, WindowMaxX - 50, WindowCenY + 50, nHandle, TRUE);

	//ページ数表示
	const int nY = static_cast<int>(WindowCenY * 1.65);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawFormatString(WindowCenX - 30, nY - 80, GetColor(255, 255, 255), "%d / %d", m_pCursorSet->m_nManualCursorPos + 1, m_pCursorSet->m_nManualCursorMaxPos);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//説明書き
	nHandle = GetGraphicsHandle(IMAGE_TITLE_MANUAL_DESPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, nY, 1, 0, nHandle, TRUE);
}

/// <summary>
/// クレジット画面を描画します。
/// </summary>
void MenuScene::DrawPhaseCredit(){
	int nHandle = GetGraphicsHandle(IMAGE_TITLE_CREDIT_PAGE);
	DrawExtendGraph(50, 50, WindowMaxX - 50, WindowCenY + 50, nHandle, TRUE);

	nHandle = GetGraphicsHandle(IMAGE_TITLE_CREDIT_DISPLAY_DESCRIPTION);
	DrawRotaGraph(WindowCenX, static_cast<int>(WindowCenY * 1.65), 1, 0, nHandle, TRUE);
}

/// <summary>
/// 終了処理
/// </summary>
void MenuScene::Finalize(){
	SetGameDataVolumeBGM(m_pCursorSet->m_nConfigBGMCursorPos * 5);
	SetGameDataVolumeSE(m_pCursorSet->m_nConfigSECursorPos * 5);
}
