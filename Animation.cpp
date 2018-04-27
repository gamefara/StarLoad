#include "DxLib.h"
#include "Animation.h"

Animation::Animation()
{
	m_anAnimationMaxCount.resize(m_nMaxIndex, INVALID);
	m_anAnimationNowCount.resize(m_nMaxIndex, 0);
	m_abAnimation.resize(m_nMaxIndex, INVALID);
	m_anIndexList.reserve(m_nMaxIndex);
}

Animation::~Animation()
{
	m_anAnimationMaxCount.clear();
	m_anAnimationNowCount.clear();
	m_abAnimation.clear();
	m_anIndexList.clear();
}

/// <summary>
/// 初期化処理を行います。
/// </summary>
void Animation::Initialize(){
}

/// <summary>
/// 毎フレーム描画以外の処理を行います。
/// </summary>
void Animation::Update(){
	UpdateAnimation();
}

/// <summary>
/// 毎フレーム描画処理を行います。
/// </summary>
void Animation::DrawLoop(){
	//デバッグ用に表示している
	int a = 0;
	for(unsigned int i = 0; i < 128; i++){
		if(m_anAnimationMaxCount.at(i) != INVALID){
			DrawString(200 + 40 * i, 0, "A", GetColor(0, 255, 255));
			DrawFormatString(200 + 40 * i, 70, GetColor(0, 255, 255), "%d", m_anAnimationMaxCount.at(i));
			DrawFormatString(200 + 40 * i, 50, GetColor(0, 255, 255), "%d", m_anAnimationNowCount.at(i));
			a++;
		}
	}
	DrawFormatString(200, 100, GetColor(0, 255, 192), "%d", a);
}

/// <summary>
/// 終了処理を行います。
/// </summary>
void Animation::Finalize(){
}

/// <summary>
/// アニメーションを設定します。
/// </summary>
/// <param name="count">アニメーションする時間</param>
/// <returns>設定した配列要素番号</returns>
int Animation::SetAnimationCount(int nCount){
	if(nCount < 0) return INVALID;

	auto itrBegin = m_anAnimationMaxCount.begin();
	auto itrEnd = m_anAnimationMaxCount.end();
	auto itrFactor = std::find(itrBegin, itrEnd, INVALID);
	//要素番号を取得
	int nIndex = std::distance(itrBegin, itrFactor);

	if(nIndex == m_nMaxIndex) return INVALID;

	m_anAnimationNowCount.at(nIndex) = 0;
	m_anAnimationMaxCount.at(nIndex) = nCount;
	m_abAnimation.at(nIndex) = TRUE;
	m_anIndexList.push_back(nIndex);
	return nIndex;
}

/// <summary>
/// 指定カウンタを取得します。
/// </summary>
/// <param name="nIndex">指定要素番号</param>
/// <returns>カウンタ</returns>
int Animation::GetAnimationCount(int nIndex){
	if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == INVALID) return INVALID;
	else return m_anAnimationNowCount.at(nIndex);
}

/// <summary>
/// 指定カウンタを停止します。
/// </summary>
/// <param name="nIndex">指定要素番号</param>
/// <returns></returns>
int Animation::StopAnimation(int nIndex){
	if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == INVALID) return INVALID;

	m_abAnimation.at(nIndex) = FALSE;
	return TRUE;
}

/// <summary>
/// 指定カウンタを再カウントします。
/// </summary>
/// <param name="nIndex">指定要素番号</param>
/// <returns></returns>
int Animation::RestartAnimation(int nIndex){
	if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == INVALID) return INVALID;

	m_abAnimation.at(nIndex) = TRUE;
	return TRUE;
}

/// <summary>
/// 登録済みのカウンタを全て停止します。
/// </summary>
/// <returns></returns>
int Animation::StopAllAnimation(){
	int size = m_anIndexList.size();
	for(int i = 0; i < size; i++){
		int index = m_anIndexList.at(i);
		if(m_abAnimation.at(index) != INVALID) m_abAnimation.at(index) = FALSE;
	}
	return TRUE;
}

/// <summary>
/// 登録済みのカウンタを全て再カウントします。
/// </summary>
/// <returns></returns>
int Animation::RestartAllAnimation(){
	int size = (int)m_anIndexList.size();
	for(int i = 0; i < size; i++){
		int index = m_anIndexList.at(i);
		if(m_abAnimation.at(index) != INVALID) m_abAnimation.at(index) = TRUE;
	}
	return TRUE;
}

/// <summary>
/// 毎フレームカウンタを監視し、終了値に達したカウンタを削除します。
/// </summary>
/// <returns></returns>
int Animation::UpdateAnimation(){
	int nIndexSize = (int)m_anIndexList.size();
	if(nIndexSize <= 0) return FALSE;

	std::vector<int> anAnimeDeleteList;
	anAnimeDeleteList.reserve(nIndexSize);

	for(int i = 0; i < nIndexSize; i++){
		int nIndex = m_anIndexList.at(i);

		if(m_anAnimationMaxCount.at(nIndex) == INVALID || m_abAnimation.at(nIndex) == FALSE) continue;

		if(m_anAnimationNowCount.at(nIndex) >= m_anAnimationMaxCount.at(nIndex)){
			m_anAnimationNowCount.at(nIndex) = INVALID;
			m_anAnimationMaxCount.at(nIndex) = INVALID;
			anAnimeDeleteList.push_back(nIndex);
		}
		else m_anAnimationNowCount.at(nIndex)++;
	}

	int nDeleteSize = (int)anAnimeDeleteList.size();
	//アニメーションが終了したものがあれば削除
	if(nDeleteSize > 0){
		for(int i = 0; i < nDeleteSize; i++){
			int nIndex = anAnimeDeleteList.at(i);
			auto itrFactor = std::find(m_anIndexList.begin(), m_anIndexList.end(), nIndex);
			int nFindIndex = std::distance(m_anIndexList.begin(), itrFactor);
			m_anIndexList.erase(m_anIndexList.begin() + nFindIndex);
		}
	}

	return TRUE;
}

/// <summary>
/// 等速変化の内分カウンタです。
/// </summary>
/// <param name="nIndex">要素番号</param>
/// <param name="fStart">nNow = 0の開始値</param>
/// <param name="fEnd">nNow = nMaxの終了値</param>
/// <returns>現在時間に相当する値</returns>
float Animation::GetAnimationSmooth(int nIndex, float fStart, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;
	else if(fStart == fEnd) return fEnd;

	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMax = (float)m_anAnimationMaxCount.at(nIndex);

	float fValue = fStart + (fEnd - fStart) * (fNow / fMax);
	return fValue;
}

/// <summary>
/// 加速変化の内分カウンタです。
/// </summary>
/// <param name="nIndex">要素番号</param>
/// <param name="fStart">nNow = 0の開始値</param>
/// <param name="fEnd">nNow = nMaxの終了値</param>
/// <returns>現在時間に相当する値</returns>
float Animation::GetAnimationAccelerate(int nIndex, float fStart, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;
	else if(fStart == fEnd) return fEnd;
	
	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMax = (float)m_anAnimationMaxCount.at(nIndex);

	float fValue = fStart + (fEnd - fStart) * std::powf(fNow / fMax, 2.0f);
	return fValue;
}

/// <summary>
/// 減速変化の内分カウンタです。
/// </summary>
/// <param name="nIndex">要素番号</param>
/// <param name="fStart">nNow = 0の開始値</param>
/// <param name="fEnd">nNow = nMaxの終了値</param>
/// <returns>現在時間に相当する値</returns>
float Animation::GetAnimationSlowDown(int nIndex, float fStart, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;
	else if(fStart == fEnd) return fEnd;
	
	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMax = (float)m_anAnimationMaxCount.at(nIndex);

	float fValue = fStart + (fEnd - fStart) * (1.0f - std::powf(1.0f - (fNow / fMax), 2.0f));
	return fValue;
}

/// <summary>
/// 等速往復変化の内分カウンタです。
/// </summary>
/// <param name="nIndex">要素番号</param>
/// <param name="fStart">nNow = 0の開始値</param>
/// <param name="fCenter">nNow = nMax / 2の中間値</param>
/// <param name="fEnd">nNow = nMaxの終了値</param>
/// <returns></returns>
float Animation::GetAnimationSmoothLaps(int nIndex, float fStart, float fCenter, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;

	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMiddle = (float)(m_anAnimationMaxCount.at(nIndex) / 2);

	float fValue;
	if(fNow < fMiddle){
		if(fStart == fCenter) return fCenter;
		fValue = fStart + (fCenter - fStart) * (fNow / fMiddle);
	}
	else{
		if(fCenter == fEnd) return fEnd;

		fNow -= fMiddle;
		fValue = fCenter + (fEnd - fCenter) * (fNow / fMiddle);
	}

	return fValue;
}


float Animation::GetAnimationAccelerateLaps(int nIndex, float fStart, float fCenter, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;

	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMiddle = (float)(m_anAnimationMaxCount.at(nIndex) / 2);

	float fValue;
	if(fNow < fMiddle){
		if(fStart == fCenter) return fCenter;
		fValue = fStart + (fCenter - fStart) * std::powf(fNow / fMiddle, 2.0f);
	}
	else{
		if(fCenter == fEnd) return fEnd;

		fNow -= fMiddle;
		fValue = fCenter + (fEnd - fCenter) * std::powf(fNow / fMiddle, 2.0f);
	}

	return fValue;
}


float Animation::GetAnimationSlowDownLaps(int nIndex, float fStart, float fCenter, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;

	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMiddle = (float)(m_anAnimationMaxCount.at(nIndex) / 2);

	float fValue;
	if(fNow < fMiddle){
		if(fStart == fCenter) return fCenter;
		fValue = fStart + (fCenter - fStart) * (1.0f - std::powf(1.0f - (fNow / fMiddle), 2.0f));
	}
	else{
		if(fCenter == fEnd) return fEnd;

		fNow -= fMiddle;
		fValue = fCenter + (fEnd - fCenter) * (1.0f - std::powf(1.0f - (fNow / fMiddle), 2.0f));
	}

	return fValue;
}

float Animation::GetAnimationGravityLaps(int nIndex, float fStart, float fCenter, float fEnd){
	if(nIndex < 0 || nIndex >= m_nMaxIndex) return INVALID;

	float fNow = (float)m_anAnimationNowCount.at(nIndex);
	float fMiddle = (float)(m_anAnimationMaxCount.at(nIndex) / 2);

	if(fNow < fMiddle) return GetAnimationSlowDownLaps(nIndex, fStart, fCenter, fEnd);
	else return GetAnimationAccelerateLaps(nIndex, fStart, fCenter, fEnd);
}