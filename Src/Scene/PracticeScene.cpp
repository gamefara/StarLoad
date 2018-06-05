#include "PracticeScene.h"

PracticeScene::PracticeScene(ISceneManager* pChanger, IResourceManager* pResource) : BaseScene(pChanger, pResource)
{
}

PracticeScene::~PracticeScene(){
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Initialize(){
	m_pIndexSet = new StructIndex();
	m_pFlagSet = new StructFlag();
	m_pStageSet = new StructStage();
	m_pBlockSet = new StructBlock();
	m_pPlayerSet = new StructPlayer();
	InitializeSceneStatus();
	InitializeBlocks();
	InitializePlayer();
}

void PracticeScene::InitializeSceneStatus(){
	m_pStageSet->m_nLevel = GetLevel();
	m_pStageSet->m_nGameHiScore = GetGameDataScore(GetLevel());
	m_pStageSet->m_sGameHiRank = GetGameDataRank(GetLevel());
	m_nSoundHandle = m_pResource->GetSoundsHandle(ResourceSound::SOUND_BGM_STAGE);
}

void PracticeScene::InitializeBlocks(){
	InitializeBlockList();

	int nSpeed = m_pBlockSet->m_nBlockSpeed;
	int nHeight = m_pBlockSet->m_nBlockHeight;
	int nWidth = m_pBlockSet->m_nBlockWidth;
	int nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BLOCK_GREEN);
	int nAnimation = m_pIndexSet->m_nAnimeIndex;
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		int nX = m_anBlocksXList.at(i);
		int nY = (-1) * m_anBlocksYList.at(i);
		m_apBlock.at(i) = new Block(nX, nY, nSpeed, nHeight, nWidth, nHandle, m_pAnime);
	}
}

void PracticeScene::InitializeBlockList(){
	m_anBlocksXList.clear();
	m_anBlocksYList.clear();
	int nGameCount;
	int nTimes;

	m_pBlockSet->m_nBlockSpeed = m_pBlockSet->m_nBlockDefaultSpeed + GetLevel() / 2;
	CreateStageBlocks(nGameCount, nTimes);

	m_pStageSet->m_nGameMaxCount = (nGameCount + 2) * 60;
	m_pIndexSet->m_nAnimeIndex = m_pAnime->SetAnimationDelayCount(m_pStageSet->m_nGameMaxCount, m_pStageSet->m_nStandMaxCount);
	m_pBlockSet->m_nBlockCount = nGameCount * nTimes;

	m_apBlock.resize(m_pBlockSet->m_nBlockCount);
	int nSameIndex = 0;
	//X座標はブロック左端から中心までの長さ分右にずらす
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		std::vector<int> anX = { 0, 1, 2, 3, 4, 5, 6 };
		while(nSameIndex < m_pBlockSet->m_nBlockCount && m_anBlocksYList.at(i) == m_anBlocksYList.at(nSameIndex)){
			int nNow;
			while(TRUE){
				nNow = GetRand(anX.size() - 1);
				auto itrValue = std::find(anX.begin(), anX.end(), anX.at(nNow));
				int nIndex = std::distance(anX.begin(), itrValue);
				if(nIndex != anX.size()) break;
			}
			m_anBlocksXList.at(nSameIndex) = m_pStageSet->m_nFieldMinX + static_cast<int>(m_pBlockSet->m_nBlockWidth * (anX.at(nNow) + 0.5f));
			anX.erase(remove(anX.begin(), anX.end(), anX.at(nNow)), anX.end());
			nSameIndex++;
		}
		i = nSameIndex - 1;
	}
}

void PracticeScene::CreateStageBlocks(int& nGameCount, int& nTimes){
	nGameCount = 128;
	nTimes = 3;
	float fBpm = 180.0f;
	int nSpeed = m_pBlockSet->m_nBlockSpeed;
	int bLevelBegginer = (GetLevel() == Level::LEVEL_BEGINNER);
	int bLevelStandard = (GetLevel() == Level::LEVEL_STANDARD);
	int bLevelHard = (GetLevel() == Level::LEVEL_HARD);
	int nCenCount = (bLevelBegginer ? 0 : nGameCount * nTimes);

	if(bLevelStandard) nTimes = 4;
	else if(bLevelHard) nTimes = 6;
	int nMaxCount = nGameCount * nTimes;

	m_anBlocksXList.resize(nMaxCount);
	m_anBlocksYList.resize(nMaxCount);
	float fBase = CreateOneBeat(fBpm);

	if(!bLevelBegginer){
		for(int i = 0; i < nCenCount; i++){
			float fNewBase = fBase * i + (i % 3 == 1 ? 10 : 0);
			m_anBlocksYList.at(i) = static_cast<int>(fNewBase * nSpeed);
		}
		fBase = (bLevelStandard ? CreateOneBeat(fBpm / 3) : CreateOneBeat(fBpm));
	}
	for(int i = nCenCount; i < nMaxCount; i++) m_anBlocksYList.at(i) = static_cast<int>(fBase) * (i - nCenCount) * nSpeed;
	std::sort(m_anBlocksYList.begin(), m_anBlocksYList.end());
}

void PracticeScene::InitializePlayer(){
	const int& nHeight = m_pPlayerSet->m_nHeight;
	const int& nWidth = m_pPlayerSet->m_nWidth;
	const int& nHandle = GetPlayerBar();
	const int& nMaxIndex = m_pPlayerSet->m_nBarCount;
	const int& nWhiteHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BLOCK_WHITE);
	const int nDiffIndex = nMaxIndex / 2;

	m_apPlayer.clear();
	m_apPlayer.resize(nMaxIndex);
	for(int i = (-1) * nDiffIndex; i < nMaxIndex - nDiffIndex; i++){
		const int nX = WindowCenX + i * nWidth;
		const int nY = WindowMaxY - 50;
		const int nIndex = i + nDiffIndex;
		m_apPlayer.at(nIndex) = new Player(nX, nY, nHeight, nWidth, nHandle, nWhiteHandle, nIndex, m_pAnime);
	}

	//移動不可エリアを生成
	int anForbidIndex[2] = { GetLevel(), nMaxIndex - 1 - GetLevel() };
	for(int i = 0; i < 2; i++) m_apPlayer.at(anForbidIndex[i])->SetCollision();
}

int PracticeScene::GetPlayerBar(){
	int nHandle;
	switch(GetLevel()){
	case Level::LEVEL_BEGINNER:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BLOCK_BLUE);
		break;
	case Level::LEVEL_STANDARD:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BLOCK_YELLOW);
		break;
	default:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BLOCK_RED);
		break;
	}
	return nHandle;
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

	float fSpeed = static_cast<float>(m_pBlockSet->m_nBlockSpeed);
	float fDistance = static_cast<float>(m_apPlayer.at(0)->GetPlayerPositionCenY());
	int nCount = static_cast<int>(fDistance / fSpeed);
	int nAnimeCount = m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex);
	if(nAnimeCount == nCount) PlaySoundMem(m_nSoundHandle, DX_PLAYTYPE_BACK, TRUE);

	int& nScore = m_pStageSet->m_nGameScore;
	int& nIndex = m_pStageSet->m_nRateIndex;
	const int& nMaxIndex = m_pStageSet->m_nRateMaxIndex;
	const int& nRate = m_pStageSet->m_anRankingRate[nIndex];
	int& nAddRate = m_pStageSet->m_nAddRate;
	nScore += nAddRate;	//スコア
	if(nIndex < nMaxIndex && nScore >= nRate){
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
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		if(m_apBlock.at(i) == nullptr) continue;
		m_apBlock.at(i)->Update();
	}
}

void PracticeScene::UpdatePlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		if(m_apPlayer.at(i) == nullptr) continue;
		m_apPlayer.at(i)->Update();

		if(m_apPlayer.at(i)->GetCollision()) continue;
		m_apPlayer.at(i)->SetFlash(m_pPlayerSet->m_nBarIndex);
		UpdateCollisionPlayers(i);
	}

	UpdateBarMovePlayers();
}

void PracticeScene::UpdateCollisionPlayers(const int& nIndex){
	int nCenX = m_apPlayer.at(nIndex)->GetPlayerPositionCenX();

	int nMinY = m_apPlayer.at(nIndex)->GetPlayerPositionMinY();
	int nMaxY = m_apPlayer.at(nIndex)->GetPlayerPositionMaxY();
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		int nBlockMinX = m_apBlock.at(i)->GetBlockPositionMinX();
		int nBlockMinY = m_apBlock.at(i)->GetBlockPositionMinY();
		int nBlockMaxX = m_apBlock.at(i)->GetBlockPositionMaxX();
		int nBlockMaxY = m_apBlock.at(i)->GetBlockPositionMaxY();

		if(nBlockMaxY < nMinY || nBlockMinY > nMaxY) continue;
		if(nCenX < nBlockMinX || nBlockMaxX < nCenX) continue;
		if(m_apPlayer.at(nIndex) == nullptr || !m_apPlayer.at(nIndex)->IsFlash()) continue;

		m_apPlayer.at(nIndex)->SetCollision();
		UpdateCollisionMovePlayers(nIndex);
		return;
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
	else if(IsFixedProcess(KEY_INPUT_RIGHT) && nIndex < nMaxIndex){
		while(nIndex < nMaxIndex){
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

/// <summary>
/// 毎フレーム描画処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::DrawLoop(){
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Practice");

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	int nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_BACKGROUND);
	DrawGraph(0, 0, nHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//魔法陣
	nHandle = GetDrawMagicHandle();
	double dAngle = (-1) * m_pAnime->GetAnimationCount(m_pIndexSet->m_nAnimeIndex) / 6;
	DrawRotaGraph(100, 100, 0.125, dAngle, nHandle, TRUE);

	//スコア表示
	int& nScore = m_pStageSet->m_nGameScore;
	const int& nHiScore = m_pStageSet->m_nGameHiScore;
	DrawString(WindowMaxX - 80, 50, "Score", GetColor(255, 255, 255));
	DrawFormatString(WindowMaxX - 80, 80, GetColor(255, 255, 255), "%d", nScore);

	//ハイスコア表示
	DrawString(WindowMaxX - 80, 100, "HiScore", GetColor(255, 255, 255));
	DrawFormatString(WindowMaxX - 80, 130, GetColor(255, 255, 255), "%d", (nScore < nHiScore ? nHiScore : nScore));
	DrawLoopBlocks();
	DrawLoopPlayers();

	if(!m_pAnime->IsFinishedAnimationCount(m_pIndexSet->m_nFinishStageIndex)) return;
	unsigned int anColor[] = { GetColor(128,128,128), GetColor(255,255,255) };
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
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_D);
		break;
	case 1:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_C);
		break;
	case 2:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_B);
		break;
	case 3:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_A);
		break;
	default:
		nHandle = m_pResource->GetGraphicsHandle(ResourceImage::IMAGE_PRACTICE_MAGIC_S);
		break;
	}
	return nHandle;
}

/// <summary>
/// 終了時処理
/// </summary>
/// <returns>TRUE</returns>
void PracticeScene::Finalize(){
	delete m_pIndexSet;
	delete m_pFlagSet;
	delete m_pStageSet;
	delete m_pBlockSet;
	delete m_pPlayerSet;
	FinalizeBlocks();
	StopSoundMem(m_nSoundHandle);
	m_pAnime->RestartAllAnimation();
}

void PracticeScene::FinalizeBlocks(){
	for(int i = 0; i < m_pBlockSet->m_nBlockCount; i++){
		m_apBlock.at(i)->Finalize();
		delete m_apBlock.at(i);
	}

	m_apBlock.clear();
	m_apBlock.shrink_to_fit();//メモリ解放
}

void PracticeScene::FinalizePlayers(){
	for(int i = 0; i < m_pPlayerSet->m_nBarCount; i++){
		m_apPlayer.at(i)->Finalize();
		delete m_apPlayer.at(i);
	}

	m_apPlayer.clear();
	m_apPlayer.shrink_to_fit();
}
