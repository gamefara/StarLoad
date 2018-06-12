#include "PracticeScene.h"

PracticeScene::PracticeScene(ISceneManager* pChanger, IResourceManager* pResource) : BaseScene(pChanger, pResource){
	m_pIndexSet = new StructIndex();
	m_pFlagSet = new StructFlag();
	m_pStageSet = new StructStage();
	m_pBlockSet = new StructBlock();
	m_pPlayerSet = new StructPlayer();
}

PracticeScene::~PracticeScene(){
	DeletePtr(m_pIndexSet);
	DeletePtr(m_pFlagSet);
	DeletePtr(m_pStageSet);
	DeletePtr(m_pBlockSet);
	DeletePtr(m_pPlayerSet);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Initialize(){
	InitializeSceneStatus();
	InitializeBlocks();
	InitializePlayer();
}

/// <summary>
/// ゲーム情報を設定します。
/// </summary>
void PracticeScene::InitializeSceneStatus(){
	const int& nLevel = GetLevel();
	m_pStageSet->m_nGameHiScore = GetGameDataScore(nLevel);
	m_pStageSet->m_sGameHiRank = GetGameDataRank(nLevel);
	m_nSoundHandle = GetSoundsHandle(SOUND_BGM_STAGE);
}

/// <summary>
/// ブロック初期化処理をします。
/// </summary>
void PracticeScene::InitializeBlocks(){
	m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationDelayCount(m_pStageSet->m_nGameMaxCount, m_pStageSet->m_nStandMaxCount);
	m_apBlock.resize(m_pBlockSet->m_nBlockCount);
}

/// <summary>
/// プレイヤー初期化処理をします。
/// </summary>
void PracticeScene::InitializePlayer(){
	const int& nWidth = m_pPlayerSet->m_nWidth;
	const int& nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_GREEN);
	const int& nMaxIndex = m_pPlayerSet->m_nBarCount;
	const int& nWhiteHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_WHITE);
	const int nDiffIndex = nMaxIndex / 2;

	m_apPlayer.clear();
	m_apPlayer.resize(nMaxIndex);
	for(int i = (-1) * nDiffIndex; i < nMaxIndex - nDiffIndex; i++){
		const int nX = WindowCenX + i * nWidth;
		const int& nY = m_pPlayerSet->m_nFieldY;
		const int nIndex = i + nDiffIndex;
		const int& nAnimeIndex = m_pIndexSet->m_nAnimeIndex;
		m_apPlayer.at(nIndex) = new Player(nX, nY, nHandle, nWhiteHandle, nIndex, m_pAnime, nAnimeIndex);
	}

	//移動不可エリアを生成
	int anForbidIndex[2] = { GetLevel(), nMaxIndex - 1 - GetLevel() };
	for(int i = 0; i < 2; i++) m_apPlayer.at(anForbidIndex[i])->SetCollision();
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Update(){
	UpdatePlayers();
	if(!m_pAnime->IsFinishedAnimationDelayCount(m_pIndexSet->m_nAnimeIndex)) return;

	if(m_pFlagSet->m_bStageClear) UpdateStageClearProcess();
	else if(m_pFlagSet->m_bGameOver) UpdateGameOverProcess();
	else UpdateGamePlayProcess();
}

/// <summary>
/// ステージクリア時の処理をします。
/// </summary>
void PracticeScene::UpdateStageClearProcess(){
	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)){
		//BGMフェードアウト
		const int nVolume = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nFinishStageIndex, GetGameDataVolumeBGM(), 0);
		m_pResource->SetSoundsBGMVolume(nVolume);

		UpdateBlocks();
		return;
	}
	else if(m_pFlagSet->m_bClearMusic){
		m_pFlagSet->m_bClearMusic = FALSE;
		const int& nHandle = GetSoundsHandle(SOUND_SE_CLEAR);
		int nVolume = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeSE());
		ChangeVolumeSoundMem(nVolume, nHandle);
		PlaySoundMem(nHandle, DX_PLAYTYPE_BACK);
	}
	if(m_pFlagSet->m_bFadeout) UpdateFadeOutProcess();

	if(CheckSoundMem(m_nSoundHandle)) StopSoundMem(m_nSoundHandle);
	if(IsFixedProcess(KEY_INPUT_LEFT) || IsFixedProcess(KEY_INPUT_RIGHT)){
		ReverseFlag(m_pFlagSet->m_bContinue);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
	if(IsPushProcess(KEY_INPUT_Z)){
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFadeoutMaxCount);
		m_pFlagSet->m_bFadeout = TRUE;
		PlaySoundMem(GetSoundsHandle(SOUND_SE_TITLE_OK), DX_PLAYTYPE_BACK);
	}
}

/// <summary>
/// ゲームオーバー時の処理をします。
/// </summary>
void PracticeScene::UpdateGameOverProcess(){
	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)){
		//BGMフェードアウト
		const int nVolume = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nFinishStageIndex, GetGameDataVolumeBGM(), 0);
		m_pResource->SetSoundsBGMVolume(nVolume);

		UpdateBlocks();
		return;
	}
	if(m_pFlagSet->m_bFadeout) UpdateFadeOutProcess();

	if(CheckSoundMem(m_nSoundHandle)) StopSoundMem(m_nSoundHandle);
	if(IsFixedProcess(KEY_INPUT_LEFT) || IsFixedProcess(KEY_INPUT_RIGHT)){
		ReverseFlag(m_pFlagSet->m_bContinue);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_CURSOR_MOVE), DX_PLAYTYPE_BACK);
	}
	if(IsPushProcess(KEY_INPUT_Z)){
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFadeoutMaxCount);
		m_pFlagSet->m_bFadeout = TRUE;
		PlaySoundMem(GetSoundsHandle(SOUND_SE_TITLE_OK), DX_PLAYTYPE_BACK);
	}
}

/// <summary>
/// フェードアウト処理をします。
/// </summary>
void PracticeScene::UpdateFadeOutProcess(){
	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nAnimeIndex)) return;

	m_pResource->AllSoundsStopBGMVolume();
	m_pResource->AllSoundsStopSEVolume();
	//ハイスコア
	if(m_pStageSet->m_nGameScore > m_pStageSet->m_nGameHiScore){
		SetGameDataDateTime(GetLevel(), GetNowDateTime());
		SetGameDataScore(GetLevel(), m_pStageSet->m_nGameScore);
		SetGameDataRank(GetLevel(), m_pStageSet->m_sGameRank);
		FinalizeLoadResultStream();
	}
	m_pAnime->RestartAllAnimation();
	if(m_pFlagSet->m_bContinue) m_pSceneChanger->ChangeScene(SCENE_PRACTICE);
	else{
		m_pSceneChanger->ChangeScene(SCENE_MENU);
		SetPhase(PHASE_TITLE);
	}
}

/// <summary>
/// ゲーム中の処理をします。
/// </summary>
void PracticeScene::UpdateGamePlayProcess(){
	//ゲームオーバー時
	if(!m_pFlagSet->m_bStageClear && m_pPlayerSet->m_nBarIndex == Invalid){
		if(m_pFlagSet->m_bGameOver) return;
		m_pFlagSet->m_bGameOver = TRUE;
		m_pIndexSet->m_nFinishStageIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFinishStageCount);
		PlaySoundMem(GetSoundsHandle(SOUND_SE_PLAYER_BLOCK_COLLISION), DX_PLAYTYPE_BACK);
	}
	//ゲームクリア時
	else if(!m_pFlagSet->m_bGameOver && m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nAnimeIndex)){
		if(m_pFlagSet->m_bStageClear) return;
		m_pFlagSet->m_bStageClear = TRUE;
		m_pPlayerSet->m_nBarIndex = Invalid;
		m_pIndexSet->m_nFinishStageIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFinishStageCount);
	}
	if(!CheckSoundMem(m_nSoundHandle)) PlaySoundMem(m_nSoundHandle, DX_PLAYTYPE_LOOP, TRUE);
	UpdateBlocks();

	//スコア
	int& nScore = m_pStageSet->m_nGameScore;
	int& nIndex = m_pStageSet->m_nRateIndex;
	const int& nMaxIndex = m_pStageSet->m_nRateMaxIndex;
	int& nAddRate = m_pStageSet->m_nAddRate;
	nScore += nAddRate;

	if(nIndex >= nMaxIndex) return;
	const int& nRate = m_pStageSet->m_anRankingRate[nIndex];
	if(nScore >= nRate){
		nIndex++;
		nAddRate *= 2;
		UpdateRank(m_pStageSet->m_sGameRank);
		UpdateRank(m_pStageSet->m_sGameHiRank);
	}
}

/// <summary>
/// ランクを上げる処理をします。
/// </summary>
/// <param name="sRank">現在のランク</param>
void PracticeScene::UpdateRank(std::string& sRank){
	if(sRank == "D") sRank = "C";
	else if(sRank == "C") sRank = "B";
	else if(sRank == "B") sRank = "A";
	else if(sRank == "A") sRank = "S";
}

/// <summary>
/// ブロック更新処理をします。
/// </summary>
void PracticeScene::UpdateBlocks(){
	DeleteBlocks();
	CreateBlocks();
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->Update();
		int nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_GREEN);
		if(m_apBlock.at(i)->GetBlockPositionY() < WindowCenY) continue;
		if(m_apBlock.at(i)->GetBlockHandle() != nHandle) continue;
		//緑→黄 or 赤
		nHandle = GetGraphicsHandle((GetRand(TRUE) ? IMAGE_PRACTICE_STAR_YELLOW : IMAGE_PRACTICE_STAR_RED));
		m_apBlock.at(i)->SetBlockHandle(nHandle);
	}
}

/// <summary>
/// ブロック生成処理をします。
/// </summary>
void PracticeScene::CreateBlocks(){
	const int bHard = (GetLevel() == static_cast<int>(LEVEL_HARD));
	const int bHalfCount = (m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex) >= m_pStageSet->m_nGameMaxCount / 2);
	const int nInterval = static_cast<int>((bHard ? 30 : 60) * (bHalfCount ? (2.0f / 3.0f) : 1));
	const int& nGameCount = m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex);
	if(nGameCount % nInterval != 0 || nGameCount > m_pStageSet->m_nGameMaxCount - 180) return;
	
	int nIndex = 0;
	while(nIndex < m_pBlockSet->m_nBlockCount){
		if(m_apBlock.at(nIndex) == nullptr) break;
		nIndex++;
	}

	//レベルデザイン
	const int nBlock = 7;
	const int nYellowMin = 2 + (1 - GetLevel() / 2);
	const int nRandom = nYellowMin + GetRand(3 - GetLevel());
	//nRandomの数だけ黄色星を生成
	std::vector<int> anTempNum = { 0, 1, 2, 3, 4, 5, 6 };
	std::vector<int> anYellowPos;
	anYellowPos.resize(nRandom);
	for(int i = nRandom - 1; i >= 0; i--){
		const int nSize = static_cast<int>(anTempNum.size());
		anYellowPos.at(i) = anTempNum.at(GetRand(nSize - 1));

		auto itrFactor = std::find(anTempNum.begin(), anTempNum.end(), anTempNum.at(i));
		const int nFindIndex = std::distance(anTempNum.begin(), itrFactor);
		anTempNum.erase(anTempNum.begin() + nFindIndex);
	}

	const int& nAnimeIndex = m_pIndexSet->m_nAnimeIndex;
	int bWhite = FALSE;
	const int nWhitePatternLine = 3;
	for(int i = nIndex; i < nIndex + nBlock; i++){
		if(m_apBlock.at(i) != nullptr) continue;
		const int nBlockIndex = i % nBlock;
		const int nX = m_pPlayerSet->m_nFieldMinX + static_cast<int>(m_pPlayerSet->m_nWidth * (nBlockIndex + 0.5f));
		const int nPattern = 99;
		int nChange = GetRand(nPattern - nWhitePatternLine * bWhite);
		//黄色指定の場所は黄色に
		for(int j = 0; j < nRandom; j++){
			if(anYellowPos.at(j) != nBlockIndex) continue;
			nChange = nPattern - nWhitePatternLine;	//黄色に
			break;
		}

		int nHandle = Invalid;
		//白は2つ以上まとめて落ちない
		if(nChange > nPattern - nWhitePatternLine){
			nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_WHITE);
			bWhite = TRUE;
		}
		else if(nChange >= nPattern / 3 * 2) nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_YELLOW);
		else if(nChange >= nPattern / 3) nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_GREEN);
		else nHandle = GetGraphicsHandle(IMAGE_PRACTICE_STAR_RED);

		m_apBlock.at(i) = new Block(nX, -20, GetLevel() + 3, nBlockIndex, nHandle, m_pAnime, nAnimeIndex);
	}
}

/// <summary>
/// ブロック削除処理をします。
/// </summary>
void PracticeScene::DeleteBlocks(){
	//削除チェック
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		if(m_apBlock.at(i)->GetBlockPositionY() < WindowMaxY + 20) continue;
		DeletePtr(m_apBlock.at(i));
	}
}

/// <summary>
/// プレイヤー更新処理をします。
/// </summary>
void PracticeScene::UpdatePlayers(){
	const int& nIndex = m_pPlayerSet->m_nBarIndex;
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		int j = (i <= nIndex ? nIndex - i : i);
		if(m_apPlayer.at(j) == nullptr) continue;
		m_apPlayer.at(j)->Update();

		const int& nRefIndex = m_pPlayerSet->m_nBarIndex;
		if(nRefIndex == Invalid) continue;

		if(m_apPlayer.at(j)->GetCollision()) continue;
		m_apPlayer.at(j)->SetFlash(nRefIndex);
		UpdateCollisionPlayers();
	}
	UpdateBarMovePlayers();
}

/// <summary>
/// プレイヤーとブロックの衝突チェックをします。
/// </summary>
void PracticeScene::UpdateCollisionPlayers(){
	const int& nIndex = m_pPlayerSet->m_nBarIndex;
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		//ブロックカラーによって処理変更
		if(m_apBlock.at(i) == nullptr || m_apBlock.at(i)->GetCollision()) continue;
		const int& nPlayerX = m_apPlayer.at(nIndex)->GetPlayerPositionX();
		const int& nPlayerY = m_apPlayer.at(nIndex)->GetPlayerPositionY();
		const int& nBlockX = m_apBlock.at(i)->GetBlockPositionX();
		const int& nBlockY = m_apBlock.at(i)->GetBlockPositionY();
		if(nBlockY < nPlayerY) continue;

		const int& nBlockHandle = m_apBlock.at(i)->GetBlockHandle();
		const int& nSoundHandle = GetSoundsHandle(SOUND_SE_PLAYER_BLOCK_COLLISION);
		if(nBlockHandle == GetGraphicsHandle(IMAGE_PRACTICE_STAR_RED)){
			//赤
			if(nPlayerX == nBlockX){
				m_apPlayer.at(nIndex)->SetCollision();
				UpdateCollisionMovePlayers();
				PlaySoundMem(nSoundHandle, DX_PLAYTYPE_BACK);
			}
		}
		//白
		else if(nBlockHandle == GetGraphicsHandle(IMAGE_PRACTICE_STAR_WHITE)) ReverseAllCollision();
		//黄、緑は特にない

		//通過した石は通過後衝突判定しない
		m_apBlock.at(i)->SetCollision();
		return;
	}
}

/// <summary>
/// プレイヤーとブロックの衝突後の処理をします。
/// </summary>
void PracticeScene::UpdateCollisionMovePlayers(){
	//現在地から一番近い移動先を見つける
	int& nIndex = m_pPlayerSet->m_nBarIndex;
	if(nIndex == Invalid) return;

	const int& nCount = m_pPlayerSet->m_nBarCount;
	for(int i = 2; i < nCount * 2; i++){
		//現在地の左、右の順でチェック
		const int nCheckIndex = nIndex + (i / 2) * (i % 2 == 0 ? -1 : 1);
		if(nCheckIndex < 0 || nCheckIndex >= nCount) continue;
		if(m_apPlayer.at(nCheckIndex) == nullptr || m_apPlayer.at(nCheckIndex)->GetCollision()) continue;

		const int& nPlayerY = m_apPlayer.at(nCheckIndex)->GetPlayerPositionY();
		for(int j = 0; j < m_pBlockSet->m_nBlockCount; j++){
			if(m_apBlock.at(j) == nullptr) continue;
			const int& nBlockY = m_apBlock.at(j)->GetBlockPositionY();
			const int& nBlockIndex = m_apBlock.at(j)->GetBlockIndex();
			if(nBlockY < nPlayerY || nBlockIndex != nCheckIndex) continue;

			//移動先が赤なら無視
			const int& nBlockHandle = m_apBlock.at(j)->GetBlockHandle();
			if(nBlockHandle == GetGraphicsHandle(IMAGE_PRACTICE_STAR_RED)) break;
			nIndex = nBlockIndex;
			return;
		}
	}

	//移動先がなければプレイヤー石を全て赤に
	nIndex = Invalid;
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++) m_apPlayer.at(i)->SetCollision();
}

/// <summary>
/// プレイヤー移動処理をします。
/// </summary>
void PracticeScene::UpdateBarMovePlayers(){
	int& nIndex = m_pPlayerSet->m_nBarIndex;
	if(nIndex == Invalid) return;
	const int& nMaxIndex = m_pPlayerSet->m_nBarCount;
	const int anKey[7] = { KEY_INPUT_S, KEY_INPUT_D, KEY_INPUT_F, KEY_INPUT_SPACE, KEY_INPUT_J, KEY_INPUT_K, KEY_INPUT_L };
	const int nDefIndex = nIndex;
	//対応キーを押したとき
	for(int i = 0; i < nMaxIndex; i++){
		if(IsPushProcess(anKey[i]) && !m_apPlayer.at(i)->GetCollision()){
			nIndex = i;
			PlaySoundMem(GetSoundsHandle(SOUND_SE_BLOCK_MOVE), DX_PLAYTYPE_BACK);
		}
	}
	//左右キーを押したとき
	if(IsFixedProcess(KEY_INPUT_LEFT) && nIndex > 0){
		while(nIndex >= 0){
			nIndex--;
			if(nIndex < 0){
				nIndex = nDefIndex;
				break;
			}

			const int& bNotCollision = !m_apPlayer.at(nIndex)->GetCollision();
			if(!bNotCollision) continue;
			PlaySoundMem(GetSoundsHandle(SOUND_SE_BLOCK_MOVE), DX_PLAYTYPE_BACK);
			break;
		}
	}
	else if(IsFixedProcess(KEY_INPUT_RIGHT) && nIndex < nMaxIndex - 1){
		while(nIndex <= nMaxIndex - 1){
			nIndex++;
			if(nIndex >= nMaxIndex){
				nIndex = nDefIndex;
				break;
			}

			const int& bNotCollision = !m_apPlayer.at(nIndex)->GetCollision();
			if(!bNotCollision) continue;
			PlaySoundMem(GetSoundsHandle(SOUND_SE_BLOCK_MOVE), DX_PLAYTYPE_BACK);
			break;
		}
	}
}

/// <summary>
/// 衝突判定を反転します。
/// </summary>
void PracticeScene::ReverseAllCollision(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr || i == m_pPlayerSet->m_nBarIndex) continue;
		m_apPlayer.at(i)->ReverseCollision();
	}
	PlaySoundMem(GetSoundsHandle(SOUND_SE_WHITE_BLOCK_COLLISION), DX_PLAYTYPE_BACK);
}

/// <summary>
/// 描画処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::DrawLoop(){
	if(m_pFlagSet->m_bFadeout) return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	int nHandle = GetGraphicsHandle(IMAGE_PRACTICE_BACKGROUND);
	DrawGraph(0, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawLoopBlocks();
	DrawLoopPlayers();

	//フェードアウト
	int nAlpha = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nFinishStageIndex, 0, 255);
	if(m_pFlagSet->m_bFadeout) nAlpha = 255;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	DrawGraph(0, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//スコア表示
	int& nScore = m_pStageSet->m_nGameScore;
	const int& nHiScore = m_pStageSet->m_nGameHiScore;
	DrawString(WindowMaxX - 80, 50, "Score", GetColor(0, 255, 255));
	DrawFormatString(WindowMaxX - 80, 80, GetColor(255, 255, 255), "%d", nScore);

	//ハイスコア表示
	DrawString(WindowMaxX - 115, 140, "HighScore", GetColor(255, 255, 0));
	DrawFormatString(WindowMaxX - 80, 170, GetColor(255, 255, 255), "%d", (nScore < nHiScore ? nHiScore : nScore));

	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)) return;
	if(!m_pFlagSet->m_bStageClear && !m_pFlagSet->m_bGameOver) return;

	//成功or失敗
	if(m_pFlagSet->m_bStageClear){
		nHandle = GetGraphicsHandle(IMAGE_PRACTICE_GAME_CLEAR);
		DrawRotaGraph(WindowCenX, WindowCenY, 0.8, 0, nHandle, TRUE);
	}
	else if(m_pFlagSet->m_bGameOver){
		nHandle = GetGraphicsHandle(IMAGE_PRACTICE_GAME_OVER);
		DrawRotaGraph(WindowCenX, WindowCenY, 0.8, 0, nHandle, TRUE);
	}
	//共通
	nHandle = GetGraphicsHandle(IMAGE_PRACTICE_GAME_RESTART);
	DrawRotaGraph(WindowCenX, WindowCenY + 50, 0.8, 0, nHandle, TRUE);

	nAlpha = (m_pFlagSet->m_bContinue ? 255 : 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
	nHandle = GetGraphicsHandle(IMAGE_PRACTICE_SELECT_YES);
	DrawRotaGraph(WindowCenX - 50, WindowCenY + 100, 0.8, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - nAlpha);
	nHandle = GetGraphicsHandle(IMAGE_PRACTICE_SELECT_NO);
	DrawRotaGraph(WindowCenX + 50, WindowCenY + 100, 0.8, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

/// <summary>
/// ブロック描画処理をします。
/// </summary>
void PracticeScene::DrawLoopBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->DrawLoop();
	}
}

/// <summary>
/// プレイヤー描画処理をします。
/// </summary>
void PracticeScene::DrawLoopPlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->DrawLoop();
	}
}

/// <summary>
/// 終了処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Finalize(){
	FinalizeBlocks();
	FinalizePlayers();
	StopSoundMem(m_nSoundHandle);
	m_pAnime->RestartAllAnimation();
}

/// <summary>
/// ブロック終了処理をします。
/// </summary>
void PracticeScene::FinalizeBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->Finalize();
		DeletePtr(m_apBlock.at(i));
	}

	m_apBlock.clear();
	m_apBlock.shrink_to_fit();//メモリ解放
}

/// <summary>
/// プレイヤー終了処理をします。
/// </summary>
void PracticeScene::FinalizePlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->Finalize();
		DeletePtr(m_apPlayer.at(i));
	}

	m_apPlayer.clear();
	m_apPlayer.shrink_to_fit();
}
