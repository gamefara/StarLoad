#pragma once
#include "../Task.h"
#include <vector>
#include <math.h>

class Animation : public Task
{
public:
	Animation();
	~Animation();

	void Update() override;
	void UpdateCount(const int& nIndexSize, std::vector<int>& anAnimeDeleteList);
	void DrawLoop() override;

	/// <summary>
	/// アニメーションを設定します。
	/// </summary>
	/// <param name="count">アニメーションする時間</param>
	/// <returns>設定した配列要素番号</returns>
	int SetAnimationCount(const int& nCount){
		if(nCount < 0) return Invalid;

		auto itrBegin = m_anAnimationMaxCount.begin();
		auto itrEnd = m_anAnimationMaxCount.end();
		auto itrFactor = std::find(itrBegin, itrEnd, Invalid);
		//要素番号を取得
		int nIndex = std::distance(itrBegin, itrFactor);

		if(nIndex == m_nMaxIndex) return Invalid;

		m_anAnimationNowCount.at(nIndex) = 0;
		m_anAnimationMaxCount.at(nIndex) = nCount;
		m_abAnimation.at(nIndex) = TRUE;
		m_anIndexList.push_back(nIndex);
		return nIndex;
	};
	/// <summary>
	/// 指定の時間分遅延後、アニメーションを設定します。
	/// </summary>
	/// <param name="nCount">アニメーションする時間</param>
	/// <param name="nDelayCount">アニメーション前の遅延する時間</param>
	/// <returns>設定した配列要素番号</returns>
	int SetAnimationDelayCount(const int& nCount, const int& nDelayCount){
		if(nDelayCount < 0 || nDelayCount >= INT_MAX) return Invalid;

		int nIndex = SetAnimationCount(nCount);
		if(nIndex == Invalid) return Invalid;

		m_anAnimationDelayNowCount.at(nIndex) = 0;
		m_anAnimationDelayMaxCount.at(nIndex) = nDelayCount;
		return nIndex;
	};
	/// <summary>
	/// アニメーションの速度決定後、アニメーションを設定します。
	/// </summary>
	/// <param name="nCount">アニメーションする時間</param>
	/// <param name="nSpeedCount">アニメーションする速度</param>
	/// <returns></returns>
	int SetAnimationSpeed(const int& nCount, const int& nSpeedCount){
		if(nSpeedCount < 0 || nSpeedCount >= INT_MAX) return Invalid;

		int nIndex = SetAnimationCount(nCount);
		if(nIndex == Invalid) return Invalid;

		m_anAnimationSpeed.at(nIndex) = nSpeedCount;
		return nIndex;
	}
	/// <summary>
	/// 指定カウンタを取得します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>カウンタ</returns>
	int GetAnimationCount(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_anAnimationNowCount.at(nIndex);
	}
	/// <summary>
	/// 指定カウンタ終了値を取得します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>カウンタ</returns>
	int GetAnimationMaxCount(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_anAnimationMaxCount.at(nIndex);
	}
	/// <summary>
	/// 指定遅延カウンタを取得します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>カウンタ</returns>
	int GetAnimationDelayCount(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationDelayMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_anAnimationDelayNowCount.at(nIndex);
	}
	/// <summary>
	/// 指定遅延カウンタ終了値を取得します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>カウンタ</returns>
	int GetAnimationDelayMaxCount(const int& nIndex) const {
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationDelayMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_anAnimationDelayMaxCount.at(nIndex);
	}
	/// <summary>
	/// 指定カウンタの速度を取得します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>カウンタ</returns>
	int GetAnimationSpeed(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_anAnimationSpeed.at(nIndex);
	}
	/// <summary>
	/// 指定カウントに到達したかチェックします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <param name="nMatchCount">指定カウント</param>
	/// <returns>TRUE：指定カウンタになった　、FALSE：指定カウンタになっていない</returns>
	int IsMatchedAnimationCount(const int& nIndex, const int& nMatchCount) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;
		if(nMatchCount < 0 || nMatchCount > m_anAnimationMaxCount.at(nIndex)) return Invalid;

		int bMatch = FALSE;
		if(m_anAnimationNowCount.at(nIndex) == nMatchCount) bMatch = TRUE;
		return bMatch;
	}
	/// <summary>
	/// 指定カウンタがカウントし終えたかチェックします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>TRUE: カウントし終えた、 FALSE： カウントし終えていない</returns>
	int IsFinishedAnimationCount(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationNowCount.at(nIndex) == Invalid) return Invalid;

		int bFinish = FALSE;
		if(m_anAnimationNowCount.at(nIndex) == m_anAnimationMaxCount.at(nIndex)) bFinish = TRUE;
		return bFinish;
	}
	/// <summary>
	/// 指定カウントに到達したかチェックします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <param name="nMatchCount">指定カウント</param>
	/// <returns>TRUE：指定カウンタになった　、FALSE：指定カウンタになっていない</returns>
	int IsMatchedAnimationDelayCount(const int& nIndex, const int& nMatchCount) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationDelayMaxCount.at(nIndex) == Invalid) return Invalid;
		if(nMatchCount < 0 || nMatchCount > m_anAnimationDelayMaxCount.at(nIndex)) return Invalid;

		int bMatch = FALSE;
		if(m_anAnimationDelayNowCount.at(nIndex) == nMatchCount) bMatch = TRUE;
		return bMatch;
	}
	/// <summary>
	/// 指定遅延カウンタがカウントし終えたかチェックします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns>TRUE: カウントし終えた、 FALSE： カウントし終えていない</returns>
	int IsFinishedAnimationDelayCount(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationDelayNowCount.at(nIndex) == Invalid) return Invalid;

		int bFinish = FALSE;
		if(m_anAnimationDelayNowCount.at(nIndex) == m_anAnimationDelayMaxCount.at(nIndex)) bFinish = TRUE;
		return bFinish;
	}
	/// <summary>
	/// 指定カウンタを停止します。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns></returns>
	int StopAnimation(const int& nIndex){
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;

		m_abAnimation.at(nIndex) = FALSE;
		return TRUE;
	}
	/// <summary>
	/// 指定カウンタを再カウントします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns></returns>
	int RestartAnimation(const int& nIndex){
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;

		m_abAnimation.at(nIndex) = TRUE;
		return TRUE;
	}
	/// /// <summary>
	/// 登録済みのカウンタを全て停止します。
	/// </summary>
	/// <returns></returns>
	int StopAllAnimation(){
		int nSize = (int)m_anIndexList.size();
		for(int i = 0; i < nSize; i++){
			int nIndex = m_anIndexList.at(i);
			if(m_abAnimation.at(nIndex) != Invalid) m_abAnimation.at(nIndex) = FALSE;
		}
		return TRUE;
	}
	/// <summary>
	/// 登録済みのカウンタを全て再カウントします。
	/// </summary>
	/// <returns></returns>
	int RestartAllAnimation(){
		int nSize = (int)m_anIndexList.size();
		for(int i = 0; i < nSize; i++){
			int nIndex = m_anIndexList.at(i);
			if(m_abAnimation.at(nIndex) != Invalid) m_abAnimation.at(nIndex) = TRUE;
		}
		return TRUE;
	}
	/// <summary>
	/// 指定カウンタが停止しているかチェックします。
	/// </summary>
	/// <param name="nIndex">指定要素番号</param>
	/// <returns></returns>
	int IsStoppedAnimation(const int& nIndex) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex || m_anAnimationMaxCount.at(nIndex) == Invalid) return Invalid;
		return m_abAnimation.at(nIndex);
	}
	/// <summary>
	/// 登録済みのカウンタが全て停止しているかチェックします。
	/// </summary>
	/// <returns></returns>
	int IsAllStoppedAnimation() const{
		int nSize = (int)m_anIndexList.size();
		int nCount = 0;
		for(int i = 0; i < nSize; i++){
			int nIndex = m_anIndexList.at(i);
			if(m_abAnimation.at(nIndex)) break;
			nCount++;
		}
		int bAllStop = (nCount == nSize);
		return bAllStop;
	}
	/// <summary>
	/// 等速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetAnimationSmooth(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;
		else if(tStart == tEnd) return tEnd;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMax = (T)m_anAnimationMaxCount.at(nIndex);

		T tRatio = tNow / tMax;
		T tValue = tStart + (tEnd - tStart) * tRatio;
		return tValue;
	}
	/// <summary>
	/// 加速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetAnimationAccelerate(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;
		else if(tStart == tEnd) return tEnd;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMax = (T)m_anAnimationMaxCount.at(nIndex);
		T tRatio = (tNow / tMax) * (tNow / tMax);
		T tValue = tStart + (tEnd - tStart) * tRatio;
		return tValue;
	};
	/// <summary>
	/// 減速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetAnimationSlowDown(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;
		else if(tStart == tEnd) return tEnd;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMax = (T)m_anAnimationMaxCount.at(nIndex);
		T tRatio = 1 - (1 - (tNow / tMax)) * (1 - (tNow / tMax));
		T tValue = tStart + (tEnd - tStart) * tRatio;
		return tValue;
	};
	/// <summary>
	/// 等速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tCenter">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template <typename T> T GetAnimationSmoothLaps(const int& nIndex, const T& tStart, const T& tCenter, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMiddle = (T)(m_anAnimationMaxCount.at(nIndex) / (T)2);
		T tValue;
		if(tNow < tMiddle){
			if(tStart == tCenter) return tCenter;
			T tRatio = tNow / tMiddle;
			tValue = tStart + (tCenter - tStart) * tRatio;
		}
		else{
			if(tCenter == tEnd) return tEnd;
			tNow -= tMiddle;
			T tRatio = tNow / tMiddle;
			tValue = tCenter + (tEnd - tCenter) * tRatio;
		}
		return tValue;
	};
	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ加速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tCenter">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template <typename T> T GetAnimationAccelerateLaps(const int& nIndex, const T& tStart, const T& tCenter, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMiddle = (T)(m_anAnimationMaxCount.at(nIndex) / (T)2);
		T tValue;
		if(tNow < tMiddle){
			if(tStart == tCenter) return tCenter;
			T tRatio = (tNow / tMiddle) * (tNow / tMiddle);
			tValue = tStart + (tCenter - tStart) * tRatio;
		}
		else{
			if(tCenter == tEnd) return tEnd;
			tNow -= tMiddle;
			T tRatio = (tNow / tMiddle) * (tNow / tMiddle);
			tValue = tCenter + (tEnd - tCenter) * tRatio;
		}
		return tValue;
	};
	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ減速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tCenter">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template <typename T> T GetAnimationSlowDownLaps(const int& nIndex, const T& tStart, const T& tCenter, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMiddle = (T)(m_anAnimationMaxCount.at(nIndex) / (T)2);

		T tValue;
		if(tNow < tMiddle){
			if(tStart == tCenter) return tCenter;
			T tRatio = 1 - (1 - (tNow / tMiddle)) * (1 - (tNow / tMiddle));
			tValue = tStart + (tCenter - tStart) * tRatio;
		}
		else{
			if(tCenter == tEnd) return tEnd;
			tNow -= tMiddle;
			T tRatio = 1 - (1 - (tNow / tMiddle)) * (1 - (tNow / tMiddle));
			tValue = tCenter + (tEnd - tCenter) * tRatio;
		}
		return tValue;
	};
	/// <summary>
	/// 開始値～中間値まで減速、中間値→終了値まで加速の往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tCenter">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template <typename T> T GetAnimationGravityLaps(const int& nIndex, const T& tStart, const T& tCenter, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return (T)Invalid;

		T tNow = (T)m_anAnimationNowCount.at(nIndex);
		T tMiddle = (T)(m_anAnimationMaxCount.at(nIndex) / (T)2);
		if(tNow < tMiddle) return GetAnimationSlowDownLaps(nIndex, tStart, tCenter, tEnd);
		else return GetAnimationAccelerateLaps(nIndex, tStart, tCenter, tEnd);
	};

	//↓nIndexのアニメーションを介さず値を取得する用

	/// <summary>
	/// 等速変化の内分カウンタです。
	/// (アニメーションカウンタを用いずに値を出したい時用)
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetMomentSmooth(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(tNow <= 0) return tStart;
		if(tStart == tEnd || tNow >= tMax || tMax <= 0) return tEnd;

		T tRatio = (T)nNow / (T)nMax;
		T tValue = tStart + (tEnd - tStart) * tRatio;
		return tValue;
	};
	/// <summary>
	/// 加速変化の内分カウンタです。
	/// (アニメーションカウンタを用いずに値を出したい時用)
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetMomentAccelerate(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(tNow <= 0) return tStart;
		if(tStart == tEnd || tNow >= tMax || tMax <= 0) return tEnd;

		T tRatio = ((T)nNow / (T)nMax) * ((T)nNow / (T)nMax);
		T tValue = tStart + (tEnd - tStart) * tRatio;
		return tValue;
	};
	/// <summary>
	/// 減速変化の内分カウンタです。
	/// (アニメーションカウンタを用いずに値を出したい時用)
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns>現在時間に相当する値</returns>
	template <typename T> T GetMomentSlowDown(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(tNow <= 0) return tStart;
		if(tStart == tEnd || tNow >= tMax || tMax <= 0) return tEnd;

		T tRatio = 1 - (1 - ((T)nNow / (T)nMax)) * (1 - ((T)nNow / (T)nMax));
		T fValue = fStart + (fEnd - fStart) * tRatio;
		return fValue;
	};
private:
	const int m_nMaxIndex = 128;

	std::vector<int> m_anAnimationMaxCount;
	std::vector<int> m_anAnimationNowCount;
	std::vector<int> m_anAnimationDelayNowCount;
	std::vector<int> m_anAnimationDelayMaxCount;
	std::vector<int> m_anAnimationSpeed;
	std::vector<int> m_abAnimation;
	std::vector<int> m_anIndexList;
};

