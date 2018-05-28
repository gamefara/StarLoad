#include "Animation.h"

Animation::Animation()
{
	m_anAnimationMaxCount.resize(m_nMaxIndex, Invalid);
	m_anAnimationNowCount.resize(m_nMaxIndex, 0);
	m_anAnimationDelayNowCount.resize(m_nMaxIndex, 0);
	m_anAnimationDelayMaxCount.resize(m_nMaxIndex, 0);
	m_anAnimationSpeed.resize(m_nMaxIndex, 1);
	m_abAnimation.resize(m_nMaxIndex, Invalid);
	m_anIndexList.reserve(m_nMaxIndex);
}

Animation::~Animation()
{
	m_anAnimationMaxCount.clear();
	m_anAnimationNowCount.clear();
	m_anAnimationDelayNowCount.clear();
	m_anAnimationDelayMaxCount.clear();
	m_anAnimationSpeed.clear();
	m_abAnimation.clear();
	m_anIndexList.clear();

	m_anAnimationMaxCount.shrink_to_fit();
	m_anAnimationNowCount.shrink_to_fit();
	m_anAnimationDelayNowCount.shrink_to_fit();
	m_anAnimationDelayMaxCount.shrink_to_fit();
	m_anAnimationSpeed.shrink_to_fit();
	m_abAnimation.shrink_to_fit();
	m_anIndexList.shrink_to_fit();
}

/// <summary>
/// 毎フレーム描画以外の処理を行います。
/// </summary>
void Animation::Update(){
	int nIndexSize = (int)m_anIndexList.size();
	if(nIndexSize <= 0) return;

	std::vector<int> anAnimeDeleteList;
	anAnimeDeleteList.reserve(nIndexSize);
	UpdateCount(nIndexSize, anAnimeDeleteList);

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

	anAnimeDeleteList.clear();
	anAnimeDeleteList.shrink_to_fit();
}

void Animation::UpdateCount(const int& nIndexSize, std::vector<int>& anAnimeDeleteList){
	for(int i = 0; i < nIndexSize; i++){
		int nIndex = m_anIndexList.at(i);
		if(m_anAnimationMaxCount.at(nIndex) == Invalid || m_abAnimation.at(nIndex) == FALSE) continue;
		if(m_anAnimationNowCount.at(nIndex) >= m_anAnimationMaxCount.at(nIndex)){
			m_anAnimationNowCount.at(nIndex) = Invalid;
			m_anAnimationMaxCount.at(nIndex) = Invalid;
			anAnimeDeleteList.push_back(nIndex);
		}
		else{
			//遅延指定があれば時間分待機
			if(m_anAnimationDelayMaxCount.at(nIndex) > 0){
				if(m_anAnimationDelayNowCount.at(nIndex) >= m_anAnimationDelayMaxCount.at(nIndex)){
					m_anAnimationDelayNowCount.at(nIndex) = 0;
					m_anAnimationDelayMaxCount.at(nIndex) = 0;
				}
				else m_anAnimationDelayNowCount.at(nIndex)++;
			}
			else m_anAnimationNowCount.at(nIndex) += m_anAnimationSpeed.at(nIndex);
		}
	}
}

/// <summary>
/// 毎フレーム描画処理を行います。
/// </summary>
void Animation::DrawLoop(){
#ifdef _DEBUG
	//デバッグ用に表示している
	int a = 0;
	for(unsigned int i = 0; i < 128; i++){
		if(m_anAnimationMaxCount.at(i) != Invalid){
			DrawString(200 + 40 * i, 0, "A", GetColor(0, 255, 255));
			DrawFormatString(200 + 40 * i, 70, GetColor(0, 255, 255), "%d", m_anAnimationMaxCount.at(i));
			DrawFormatString(200 + 40 * i, 50, GetColor(0, 255, 255), "%d", m_anAnimationNowCount.at(i));
			a++;
		}
	}
	DrawFormatString(200, 100, GetColor(0, 255, 192), "%d", a);
#endif _DEBUG
}
