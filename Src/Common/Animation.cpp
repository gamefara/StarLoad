#include "Animation.h"

Animation::Animation()
{
	m_anAnimationMaxCount.resize(m_nMaxIndex, Invalid);
	m_anAnimationNowCount.resize(m_nMaxIndex, 0);
	m_anAnimationDelayNowCount.resize(m_nMaxIndex, Invalid);
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
/// 更新処理
/// </summary>
void Animation::Update(){
	const int nIndexSize = static_cast<int>(m_anIndexList.size());
	if(nIndexSize <= 0) return;

	std::vector<int> anAnimeDeleteList;
	anAnimeDeleteList.reserve(nIndexSize);
	UpdateCount(nIndexSize, anAnimeDeleteList);

	const int nDeleteSize = static_cast<int>(anAnimeDeleteList.size());
	//アニメーションが終了したものがあれば削除
	if(nDeleteSize > 0){
		for(int i = 0; i < nDeleteSize; i++){
			const int nIndex = anAnimeDeleteList.at(i);
			auto itrFactor = std::find(m_anIndexList.begin(), m_anIndexList.end(), nIndex);
			const int nFindIndex = std::distance(m_anIndexList.begin(), itrFactor);
			m_anIndexList.erase(m_anIndexList.begin() + nFindIndex);
		}
	}

	anAnimeDeleteList.clear();
	anAnimeDeleteList.shrink_to_fit();
}

/// <summary>
/// カウンタを更新します。
/// </summary>
/// <param name="nIndexSize">カウンタの配列要素数</param>
/// <param name="anAnimeDeleteList">アニメーション終了する配列</param>
void Animation::UpdateCount(const int& nIndexSize, std::vector<int>& anAnimeDeleteList){
	for(int i = 0; i < nIndexSize; i++){
		const int nIndex = m_anIndexList.at(i);
		if(m_anAnimationMaxCount.at(nIndex) == Invalid || !m_abAnimation.at(nIndex)) continue;
		if(m_anAnimationNowCount.at(nIndex) >= m_anAnimationMaxCount.at(nIndex)){
			m_anAnimationNowCount.at(nIndex) = Invalid;
			m_anAnimationMaxCount.at(nIndex) = Invalid;
			m_anAnimationDelayNowCount.at(nIndex) = Invalid;
			m_anAnimationDelayMaxCount.at(nIndex) = Invalid;
			anAnimeDeleteList.push_back(nIndex);
		}
		else{
			//遅延指定があれば時間分待機
			const int bDelayCount = (m_anAnimationDelayNowCount.at(nIndex) < m_anAnimationDelayMaxCount.at(nIndex));
			if(bDelayCount) m_anAnimationDelayNowCount.at(nIndex)++;
			else m_anAnimationNowCount.at(nIndex) += m_anAnimationSpeed.at(nIndex);
		}
	}
}
