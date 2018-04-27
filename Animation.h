#pragma once
#include "Task.h"
#include <vector>
#include <math.h>

class Animation : public Task
{
public:
	Animation();
	~Animation();

	void Initialize();
	void Update();
	void DrawLoop();
	void Finalize();

	int SetAnimationCount(int nCount);
	int GetAnimationCount(int nIndex);
	int StopAnimation(int nIndex);
	int RestartAnimation(int nIndex);
	int StopAllAnimation();
	int RestartAllAnimation();

	float GetAnimationSmooth(int nIndex, float fStart, float fEnd);
	float GetAnimationAccelerate(int nIndex, float fStart, float fEnd);
	float GetAnimationSlowDown(int nIndex, float fStart, float fEnd);

	float GetAnimationSmoothLaps(int nIndex, float fStart, float fCenter, float fEnd);
	float GetAnimationAccelerateLaps(int nIndex, float fStart, float fCenter, float fEnd);
	float GetAnimationSlowDownLaps(int nIndex, float fStart, float fCenter, float fEnd);
	float GetAnimationGravityLaps(int nIndex, float fStart, float fCenter, float fEnd);

private:
	int m_nMaxIndex = 128;

	std::vector<int> m_anAnimationMaxCount;
	std::vector<int> m_anAnimationNowCount;
	std::vector<int> m_abAnimation;
	std::vector<int> m_anIndexList;

	int UpdateAnimation();
};

