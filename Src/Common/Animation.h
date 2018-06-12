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
	void DrawLoop() override{}

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
		const int nIndex = std::distance(itrBegin, itrFactor);

		if(nIndex == m_nMaxIndex) return Invalid;

		m_anAnimationNowCount.at(nIndex) = 0;
		m_anAnimationMaxCount.at(nIndex) = nCount;
		m_abAnimation.at(nIndex) = TRUE;
		m_anIndexList.push_back(nIndex);
		return nIndex;
	}

	/// <summary>
	/// 指定の時間分遅延後、アニメーションを設定します。
	/// </summary>
	/// <param name="nCount">アニメーションする時間</param>
	/// <param name="nDelayCount">アニメーション前の遅延する時間</param>
	/// <returns>設定した配列要素番号</returns>
	int SetAnimationDelayCount(const int& nCount, const int& nDelayCount){
		if(nDelayCount < 0 || nDelayCount >= INT_MAX) return Invalid;

		const int nIndex = SetAnimationCount(nCount);
		if(nIndex == Invalid) return Invalid;

		m_anAnimationDelayNowCount.at(nIndex) = 0;
		m_anAnimationDelayMaxCount.at(nIndex) = nDelayCount;
		return nIndex;
	}

	/// <summary>
	/// アニメーションの速度決定後、アニメーションを設定します。
	/// </summary>
	/// <param name="nCount">アニメーションする時間</param>
	/// <param name="nSpeedCount">アニメーションする速度</param>
	/// <returns></returns>
	int SetAnimationSpeed(const int& nCount, const int& nSpeedCount){
		if(nSpeedCount < 0 || nSpeedCount >= INT_MAX) return Invalid;

		const int nIndex = SetAnimationCount(nCount);
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
		const int nSize = static_cast<int>(m_anIndexList.size());
		for(int i = 0; i < nSize; i++){
			const int nIndex = m_anIndexList.at(i);
			if(m_abAnimation.at(nIndex) != Invalid) m_abAnimation.at(nIndex) = FALSE;
		}
		return TRUE;
	}

	/// <summary>
	/// 登録済みのカウンタを全て再カウントします。
	/// </summary>
	/// <returns></returns>
	int RestartAllAnimation(){
		const int nSize = static_cast<int>(m_anIndexList.size());
		for(int i = 0; i < nSize; i++){
			const int nIndex = m_anIndexList.at(i);
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
		const int nSize = static_cast<int>(m_anIndexList.size());
		int nCount = 0;
		for(int i = 0; i < nSize; i++){
			const int nIndex = m_anIndexList.at(i);
			if(m_abAnimation.at(nIndex)) break;
			nCount++;
		}
		const int bAllStop = (nCount == nSize);
		return bAllStop;
	}

	/// <summary>
	/// 等速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template<typename T> T GetAnimationSmooth(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		else if(tStart == tEnd) return tEnd;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tMax = static_cast<T>(m_anAnimationMaxCount.at(nIndex));

		const float fRatio = static_cast<float>(tNow) / static_cast<float>(tMax);
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 加速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template<typename T> T GetAnimationAccelerate(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		else if(tStart == tEnd) return tEnd;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tMax = static_cast<T>(m_anAnimationMaxCount.at(nIndex));
		const float fDiv = static_cast<float>(tNow) / static_cast<float>(tMax);
		const float fRatio = fDiv * fDiv;
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 減速変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns>現在時間に相当する値</returns>
	template<typename T> T GetAnimationSlowDown(const int& nIndex, const T& tStart, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		else if(tStart == tEnd) return tEnd;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tMax = static_cast<T>(m_anAnimationMaxCount.at(nIndex));
		const float fDiv = static_cast<float>(tNow) / static_cast<float>(tMax);
		const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 等速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationSmoothLaps(const int& nIndex, const T& tStart, const T& tMiddle, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tCenter = static_cast<T>(m_anAnimationMaxCount.at(nIndex) / static_cast<T>(2));
		T tValue;
		if(tNow < tCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fRatio = static_cast<float>(tNow) / static_cast<float>(tCenter);
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			tNow -= tCenter;
			const float fRatio = static_cast<float>(tNow) / static_cast<float>(tCenter);
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ加速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationAccelerateLaps(const int& nIndex, const T& tStart, const T& tMiddle, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tCenter = static_cast<T>(m_anAnimationMaxCount.at(nIndex) / static_cast<T>(2));
		T tValue;
		if(tNow < tCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fDiv = static_cast<float>(tNow) / static_cast<float>(tCenter);
			const float fRatio = fDiv * fDiv;
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			tNow -= tCenter;
			const float fDiv = static_cast<float>(tNow) / static_cast<float>(tCenter);
			const float fRatio = fDiv * fDiv;
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ減速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationSlowDownLaps(const int& nIndex, const T& tStart, const T& tMiddle, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tCenter = static_cast<T>(m_anAnimationMaxCount.at(nIndex) / static_cast<T>(2));

		T tValue;
		if(tNow < tCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fDiv = static_cast<float>(tNow) / static_cast<float>(tCenter);
			const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			tNow -= tCenter;
			const float fDiv = static_cast<float>(tNow) / static_cast<float>(tCenter);
			const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値まで減速、中間値→終了値まで加速の往復変化の内分カウンタです。
	/// </summary>
	/// <param name="nIndex">要素番号</param>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationGravityLaps(const int& nIndex, const T& tStart, const T& tMiddle, const T& tEnd) const{
		if(nIndex < 0 || nIndex >= m_nMaxIndex) return static_cast<T>(Invalid);
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;

		T tNow = static_cast<T>(m_anAnimationNowCount.at(nIndex));
		T tCenter = m_anAnimationMaxCount.at(nIndex) / static_cast<T>(2);
		if(tNow < tCenter) return GetAnimationSlowDownLaps(nIndex, tStart, tMiddle, tEnd);
		else return GetAnimationAccelerateLaps(nIndex, tStart, tMiddle, tEnd);
	}
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
	template<typename T> T GetMomentSmooth(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(nNow <= 0) return tStart;
		if(tStart == tEnd || nNow >= nMax || nMax <= 0) return tEnd;

		const float fRatio = static_cast<float>(nNow) / static_cast<float>(nMax);
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 加速変化の内分カウンタです。
	/// (アニメーションカウンタを用いずに値を出したい時用)
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns>現在時間に相当する値</returns>
	template<typename T> T GetMomentAccelerate(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(nNow <= 0) return tStart;
		if(tStart == tEnd || nNow >= nMax || nMax <= 0) return tEnd;

		const float fDiv = static_cast<float>(nNow) / static_cast<float>(nMax);
		const float fRatio = fDiv * fDiv;
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 減速変化の内分カウンタです。
	/// (アニメーションカウンタを用いずに値を出したい時用)
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns>現在時間に相当する値</returns>
	template<typename T> T GetMomentSlowDown(const T& tStart, const T& tEnd, const int nNow, const int nMax){
		if(nNow <= 0) return tStart;
		if(tStart == tEnd || nNow >= nMax || nMax <= 0) return tEnd;

		const float fDiv = static_cast<float>(nNow) / static_cast<float>(nMax);
		const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
		T tValue = tStart + static_cast<T>((tEnd - tStart) * fRatio);
		return tValue;
	}

	/// <summary>
	/// 等速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nCenter">nNowの中間値</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns></returns>
	template<typename T> T GetMomentSmoothLaps(const T& tStart, const T& tMiddle, const T& tEnd, int nNow, const int nCenter, const int nMax) const{
		if(nNow <= 0) return tStart;
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;
		if(nNow >= nMax) return tEnd;

		T tValue;
		if(nNow < nCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fRatio = static_cast<float>(nNow) / static_cast<float>(nCenter);
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			nNow -= nCenter;
			const float fRatio = static_cast<float>(nNow) / static_cast<float>(nCenter);
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ加速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nCenter">nNowの中間値</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns></returns>
	template<typename T> T GetMomentAccelerateLaps(const T& tStart, const T& tMiddle, const T& tEnd, int nNow, const int nCenter, const int nMax) const{
		if(nNow <= 0) return tStart;
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;
		if(nNow >= nMax) return tEnd;

		T tValue;
		if(nNow < nCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fDiv = static_cast<float>(nNow) / static_cast<float>(nCenter);
			const float fRatio = fDiv * fDiv;
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			nNow -= nCenter;
			const float fDiv = static_cast<float>(nNow) / static_cast<float>(nCenter);
			const float fRatio = fDiv * fDiv;
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値、中間値→終了値までそれぞれ減速往復変化の内分カウンタです。
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nCenter">nNowの中間値</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationSlowDownLaps(const T& tStart, const T& tMiddle, const T& tEnd, int nNow, const int nCenter, const int nMax) const{
		if(nNow <= 0) return tStart;
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;
		if(nNow >= nMax) return tEnd;

		T tValue;
		if(nNow < nCenter){
			if(tStart == tMiddle) return tMiddle;
			const float fDiv = static_cast<float>(nNow) / static_cast<float>(nCenter);
			const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
			tValue = tStart + static_cast<T>((tMiddle - tStart) * fRatio);
		}
		else{
			if(tMiddle == tEnd) return tEnd;
			nNow -= nCenter;
			const float fDiv = static_cast<float>(nNow) / static_cast<float>(nCenter);
			const float fRatio = 1 - (1 - fDiv) * (1 - fDiv);
			tValue = tMiddle + static_cast<T>((tEnd - tMiddle) * fRatio);
		}
		return tValue;
	}

	/// <summary>
	/// 開始値～中間値まで減速、中間値→終了値まで加速の往復変化の内分カウンタです。
	/// </summary>
	/// <param name="tStart">nNow = 0の開始値</param>
	/// <param name="tMiddle">nNow = nMax / 2の中間値</param>
	/// <param name="tEnd">nNow = nMaxの終了値</param>
	/// <param name="nNow">現在時間</param>
	/// <param name="nCenter">nNowの中間値</param>
	/// <param name="nMax">nNowの最大値</param>
	/// <returns></returns>
	template<typename T> T GetAnimationGravityLaps(const T& tStart, const T& tMiddle, const T& tEnd, int nNow, const int nCenter, const int nMax) const{
		if(nNow <= 0) return tStart;
		if(tStart == tMiddle && tMiddle == tEnd) return tStart;
		if(nNow >= nMax) return tEnd;

		if(nNow < nCenter) return GetAnimationSlowDownLaps(tStart, tMiddle, tEnd, nNow, nCenter, nMax);
		else return GetAnimationAccelerateLaps(tStart, tMiddle, tEnd, nNow, nCenter, nMax);
	}
private:
	const int m_nMaxIndex = 128;

	std::vector<int> m_anAnimationMaxCount;
	std::vector<int> m_anAnimationNowCount;
	std::vector<int> m_anAnimationDelayNowCount;
	std::vector<int> m_anAnimationDelayMaxCount;
	std::vector<int> m_anAnimationSpeed;
	std::vector<int> m_abAnimation;
	std::vector<int> m_anIndexList;

	void UpdateCount(const int& nIndexSize, std::vector<int>& anAnimeDeleteList);
};
 