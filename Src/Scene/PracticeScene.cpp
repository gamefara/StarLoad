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

void PracticeScene::InitializeSceneStatus(){
	m_pStageSet->m_nGameHiScore = GetGameDataScore(GetLevel());
	m_pStageSet->m_sGameHiRank = GetGameDataRank(GetLevel());
	m_nSoundHandle = m_pResource->GetSoundsHandle(ResourceSound::SOUND_BGM_STAGE);
	m_apPlayer.clear();
}

void PracticeScene::InitializeBlocks(){
	m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationDelayCount(m_pStageSet->m_nGameMaxCount, m_pStageSet->m_nStandMaxCount);
	m_apBlock.resize(m_pBlockSet->m_nBlockCount);
}

void PracticeScene::InitializePlayer(){
	const int& nWidth = m_pPlayerSet->m_nWidth;
	const int& nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_GREEN);
	const int& nMaxIndex = m_pPlayerSet->m_nBarCount;
	const int& nWhiteHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_WHITE);
	const int nDiffIndex = nMaxIndex / 2;

	m_apPlayer.resize(nMaxIndex);
	for(int i = (-1) * nDiffIndex; i < nMaxIndex - nDiffIndex; i++){
		const int& nX = WindowCenX + i * nWidth;
		const int& nY = m_pPlayerSet->m_nFieldY;
		const int& nIndex = i + nDiffIndex;
		const int& nAnimeIndex = m_pIndexSet->m_nAnimeIndex;
		m_apPlayer.at(nIndex) = new Player(nX, nY, nHandle, nWhiteHandle, nIndex, m_pAnime, nAnimeIndex);
	}

	//移動不可エリアを生成
	int anForbidIndex[2] = { GetLevel(), nMaxIndex - 1 - GetLevel() };
	for(int i = 0; i < 2; i++) m_apPlayer.at(anForbidIndex[i])->SetCollision();
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Update(){
	UpdatePlayers();
	if(!m_pAnime->IsFinishedAnimationDelayCount(m_pIndexSet->m_nAnimeIndex)) return;

	if(m_pFlagSet->m_bStageClear) UpdateStageClearProcess();
	else if(m_pFlagSet->m_bGameOver) UpdateGameOverProcess();
	else UpdateGamePlayProcess();
}

void PracticeScene::UpdateStageClearProcess(){
	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)){
		//BGMフェードアウト
		int nVolumeBGM = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeBGM());
		int nVolume = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nFinishStageIndex, nVolumeBGM, 0);
		if(CheckSoundMem(m_nSoundHandle)) ChangeVolumeSoundMem(nVolume, m_nSoundHandle);
		int nVolumeSE = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeSE());
		UpdateBlocks();
		return;
	}
	if(CheckSoundMem(m_nSoundHandle)) StopSoundMem(m_nSoundHandle);
	if(IsFixedProcess(KEY_INPUT_LEFT) || IsFixedProcess(KEY_INPUT_RIGHT)) ReverseFlag(m_pFlagSet->m_bReserve);
	if(IsProcess(KEY_INPUT_Z)){
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFadeoutMaxCount);
		m_pFlagSet->m_bStageClear = FALSE;
		m_pFlagSet->m_bFadeout = TRUE;
	}
}

void PracticeScene::UpdateGameOverProcess(){
	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)){
		//BGMフェードアウト
		int nVolumeBGM = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeBGM());
		int nVolume = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nFinishStageIndex, nVolumeBGM, 0);
		if(CheckSoundMem(m_nSoundHandle)) ChangeVolumeSoundMem(nVolume, m_nSoundHandle);
		int nVolumeSE = static_cast<int>((255.0f / 100.0f) * GetGameDataVolumeSE());

		UpdateBlocks();
		return;
	}
	else if(m_pFlagSet->m_bFadeout) UpdateFadeOutProcess();

	if(CheckSoundMem(m_nSoundHandle)) StopSoundMem(m_nSoundHandle);
	if(IsFixedProcess(KEY_INPUT_LEFT) || IsFixedProcess(KEY_INPUT_RIGHT)) ReverseFlag(m_pFlagSet->m_bContinue);
	if(IsProcess(KEY_INPUT_Z)){
		m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFadeoutMaxCount);
		m_pAnime->RestartAnimation(m_pIndexSet->m_nAnimeIndex);
		m_pFlagSet->m_bFadeout = TRUE;
	}
}

void PracticeScene::UpdateFadeOutProcess(){
	if(!m_pAnime->IsFinishedAnimationDelayCount(m_pIndexSet->m_nAnimeIndex)) return;

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

void PracticeScene::UpdateGamePlayProcess(){
	UpdateBlocks();
	if(m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nAnimeIndex)) m_pFlagSet->m_bStageClear = FALSE;
	if(CheckSoundMem(m_nSoundHandle) != Invalid && !CheckSoundMem(m_nSoundHandle)) PlaySoundMem(m_nSoundHandle, DX_PLAYTYPE_BACK, TRUE);

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
		nAddRate += 100;
		UpdateRank(m_pStageSet->m_sGameRank);
		UpdateRank(m_pStageSet->m_sGameHiRank);
	}
}

void PracticeScene::UpdateRank(std::string& sRank){
	if(sRank == "D") sRank = "C";
	else if(sRank == "C") sRank = "B";
	else if(sRank == "B") sRank = "A";
	else if(sRank == "A") sRank = "S";
}

void PracticeScene::UpdateBlocks(){
	DeleteBlocks();
	CreateBlocks();
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->Update();
		int nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_GREEN);
		if(m_apBlock.at(i)->GetBlockPositionY() < WindowCenY) continue;
		if(m_apBlock.at(i)->GetBlockHandle() != nHandle) continue;
		//緑→黄 or 赤
		nHandle = GetGraphicsHandle((GetRand(TRUE) == TRUE ? IMAGE_PRACTICE_STAR_YELLOW : IMAGE_PRACTICE_STAR_RED));
		m_apBlock.at(i)->SetBlockHandle(nHandle);
	}
}

void PracticeScene::CreateBlocks(){
	int bHighLevel = (GetLevel() >= Level::LEVEL_STANDARD);
	int bHalfCount = (m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex) >= m_pStageSet->m_nGameMaxCount / 2);
	const int nInterval = (bHighLevel || bHalfCount ? 30 : 60);
	if(m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex) % nInterval != 0) return;

	int nIndex = 0;
	int& nAnimeIndex = m_pIndexSet->m_nAnimeIndex;
	const int nMaxRandom = 3;
	const int nBlock = GetRand(nMaxRandom) + 1;
	std::vector<int> anX = { 0, 1, 2, 3, 4, 5, 6 };
	int bYellow = FALSE;
	while(nIndex < m_pBlockSet->m_nBlockCount){
		if(m_apBlock.at(nIndex) == nullptr) break;
		nIndex++;
	}
	for(int i = nIndex; i < nIndex + nBlock; i++){
		if(m_apBlock.at(i) != nullptr) continue;
		int nNow = 0;
		while(TRUE){
			nNow = GetRand(anX.size() - 1);
			auto itrValue = std::find(anX.begin(), anX.end(), anX.at(nNow));
			int n = std::distance(anX.begin(), itrValue);
			if(n != anX.size()) break;
		}
		int nX = m_pPlayerSet->m_nFieldMinX + static_cast<int>(m_pPlayerSet->m_nWidth * (anX.at(nNow) + 0.5f));
		anX.erase(remove(anX.begin(), anX.end(), anX.at(nNow)), anX.end());

		const int nChange = GetRand(nMaxRandom * 3);
		int nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_RED);

		//赤を多めに
		if(nChange == nMaxRandom){
			if(nBlock == 1) nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_WHITE);
			else nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_GREEN);
		}
		else if(nChange >= nMaxRandom / 3 * 2){
			if(!bYellow) nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_YELLOW);
			else nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_RED);
			bYellow = TRUE;
		}
		else if(nChange >= nMaxRandom) nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_GREEN);
		else nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_RED);

		m_apBlock.at(i) = new Block(nX, -20, GetLevel() + 3, nNow, nHandle, m_pAnime, nAnimeIndex);
	}
}

void PracticeScene::DeleteBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		if(m_apBlock.at(i)->GetBlockPositionY() < WindowMaxY + 20) continue;
		DeletePtr(m_apBlock.at(i));
	}
}

void PracticeScene::UpdatePlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->Update();

		if(m_apPlayer.at(i)->GetCollision()) continue;
		m_apPlayer.at(i)->SetFlash(m_pPlayerSet->m_nBarIndex);
	}
	UpdateBarMovePlayers();
	UpdateCollisionPlayers();
}

void PracticeScene::UpdateCollisionPlayers(){
	const int& nIndex = m_pPlayerSet->m_nBarIndex;
	if(nIndex == Invalid) return;

	const int& nPlayerX = m_apPlayer.at(nIndex)->GetPlayerPositionX();
	const int& nPlayerY = m_apPlayer.at(nIndex)->GetPlayerPositionY();
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		//ブロックカラーによって処理変更
		if(m_apBlock.at(i) == nullptr || m_apBlock.at(i)->GetCollision()) continue;
		const int& nBlockX = m_apBlock.at(i)->GetBlockPositionX();
		const int& nBlockY = m_apBlock.at(i)->GetBlockPositionY();
		if(nBlockY < nPlayerY) continue;
		m_apBlock.at(i)->SetCollision();

		const int& nBlockHandle = m_apBlock.at(i)->GetBlockHandle();
		if(nBlockHandle == GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_YELLOW)){
			//黄色
			if(nPlayerX == nBlockX) continue;
			const int& nBlockIndex = m_apBlock.at(i)->GetBlockIndex();
			m_apPlayer.at(nBlockIndex)->SetCollision();
		}
		else if(nBlockHandle == GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_STAR_RED)){
			//赤
			if(nPlayerX != nBlockX) continue;
			m_apPlayer.at(nIndex)->SetCollision();
			UpdateCollisionMovePlayers(nIndex);
		}
		//白
		else ReverseAllCollision();
		//緑はない
	}
}

void PracticeScene::UpdateCollisionMovePlayers(const int& nIndex){
	//衝突後場所変更(左側チェック→移動先無ければ右側チェック)
	for(int i = nIndex - 1; i >= 0; i--){
		if(m_apPlayer.at(i) == nullptr || m_apPlayer.at(i)->GetCollision()) continue;
		m_pPlayerSet->m_nBarIndex = i;
		return;
	}
	for(int i = nIndex + 1; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr || m_apPlayer.at(i)->GetCollision()) continue;
		m_pPlayerSet->m_nBarIndex = i;
		return;
	}
	m_pPlayerSet->m_nBarIndex = Invalid;
}

void PracticeScene::UpdateBarMovePlayers(){
	int& nIndex = m_pPlayerSet->m_nBarIndex;
	const int& nMaxIndex = m_pPlayerSet->m_nBarCount;
	int nDefIndex = nIndex;
	if(nIndex == Invalid){
		if(m_pFlagSet->m_bGameOver) return;
		m_pFlagSet->m_bGameOver = TRUE;
		m_pIndexSet->m_nFinishStageIndex = m_pAnime->SetAnimationCount(m_pStageSet->m_nFinishStageCount);
	}
	else if(IsFixedProcess(KEY_INPUT_LEFT) && nIndex > 0){
		while(nIndex >= 0){
			nIndex--;
			if(nIndex < 0){
				nIndex = nDefIndex;
				break;
			}
			int bNotCollision = !m_apPlayer.at(nIndex)->GetCollision();
			if(bNotCollision) break;
		}
	}
	else if(IsFixedProcess(KEY_INPUT_RIGHT) && nIndex < nMaxIndex - 1){
		while(nIndex <= nMaxIndex - 1){
			nIndex++;
			if(nIndex >= nMaxIndex){
				nIndex = nDefIndex;
				break;
			}
			int bNotCollision = !m_apPlayer.at(nIndex)->GetCollision();
			if(bNotCollision) break;
		}
	}
}

void PracticeScene::ReverseAllCollision(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr || i == m_pPlayerSet->m_nBarIndex) continue;
		m_apPlayer.at(i)->ReverseCollision();
	}
}

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::DrawLoop(){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	int nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BACKGROUND);
	DrawGraph(0, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//魔法陣
	nHandle = GetDrawMagicHandle();
	const int nDrawMagicCount = 2;
	const int anMagicX[nDrawMagicCount] = { WindowCenX - 180, WindowCenX + 180 };
	const int anMagicY[nDrawMagicCount] = { WindowCenY, WindowCenY };
	const double dAngle = m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex) / 10;
	const double adAngle[nDrawMagicCount] = { -dAngle, dAngle };
	for(int i = 0; i < nDrawMagicCount; i++) DrawRotaGraph(anMagicX[i], anMagicY[i], 0.1, adAngle[i], nHandle, TRUE);

	//スコア表示
	int& nScore = m_pStageSet->m_nGameScore;
	const int& nHiScore = m_pStageSet->m_nGameHiScore;
	DrawString(WindowMaxX - 80, 50, "Score", GetColor(255, 255, 255));
	DrawFormatString(WindowMaxX - 80, 80, GetColor(255, 255, 255), "%d", nScore);

	//ハイスコア表示
	DrawString(WindowMaxX - 80, 140, "HiScore", GetColor(255, 255, 255));
	DrawFormatString(WindowMaxX - 80, 170, GetColor(255, 255, 255), "%d", (nScore < nHiScore ? nHiScore : nScore));
	DrawLoopBlocks();
	DrawLoopPlayers();

	//フェードアウト
	if(m_pFlagSet->m_bFadeout){
		int nAlpha = m_pAnime->GetAnimationSmooth(m_pIndexSet->m_nAnimeIndex, 0, 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, nAlpha);
		DrawGraph(0, 0, GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BACKGROUND), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else{
		if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)) return;
		unsigned int anColor[2] = { GetColor(128,128,128), GetColor(255,255,255) };
		if(m_pFlagSet->m_bStageClear){
			DrawString(WindowMaxX / 16 * 8, WindowMaxY / 16 * 8, "Clear!", GetColor(255, 128, 255));
			DrawString(WindowMaxX / 16 * 8 - 20, WindowMaxY / 16 * 9, "もう一度挑戦しますか？", GetColor(255, 128, 255));
			DrawString(WindowMaxX / 16 * 7 + 10, WindowMaxY / 16 * 10, "Yes", anColor[m_pFlagSet->m_bReserve]);
			DrawString(WindowMaxX / 16 * 9 + 10, WindowMaxY / 16 * 10, "No", anColor[!m_pFlagSet->m_bReserve]);
		}
		else if(m_pFlagSet->m_bGameOver){
			DrawString(WindowMaxX / 16 * 8, WindowMaxY / 16 * 8, "Game Over", GetColor(255, 128, 255));
			DrawString(WindowMaxX / 16 * 8 - 20, WindowMaxY / 16 * 9, "もう一度挑戦しますか? ", GetColor(255, 128, 255));
			DrawString(WindowMaxX / 16 * 7 + 10, WindowMaxY / 16 * 10, "Yes", anColor[m_pFlagSet->m_bContinue]);
			DrawString(WindowMaxX / 16 * 9 + 10, WindowMaxY / 16 * 10, "No", anColor[!m_pFlagSet->m_bContinue]);
		}
	}
}

void PracticeScene::DrawLoopBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->DrawLoop();
	}
}

void PracticeScene::DrawLoopPlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->DrawLoop();
	}
}

int PracticeScene::GetDrawMagicHandle(){
	int nHandle;
	switch(m_pStageSet->m_nRateIndex){
	case 0:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_D);
		break;
	case 1:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_C);
		break;
	case 2:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_B);
		break;
	case 3:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_A);
		break;
	default:
		nHandle = GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_S);
		break;
	}
	return nHandle;
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Finalize(){
	FinalizeBlocks();
	FinalizePlayers();
	StopSoundMem(m_nSoundHandle);
	m_pAnime->RestartAllAnimation();
}

void PracticeScene::FinalizeBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->Finalize();
		DeletePtr(m_apBlock.at(i));
	}

	m_apBlock.clear();
	m_apBlock.shrink_to_fit();//メモリ解放
}

void PracticeScene::FinalizePlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->Finalize();
		DeletePtr(m_apPlayer.at(i));
	}

	m_apPlayer.clear();
	m_apPlayer.shrink_to_fit();
}
