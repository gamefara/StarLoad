#pragma once
#include "Task.h"

class BlockClass : public Task
{
public:
	enum BlockPosition{
		BLOCK_LEFT,
		BLOCK_MIDDLE_LEFT,
		BLOCK_MIDDLE_RIGHT,
		BLOCK_RIGHT
	};

	BlockClass(float fDrawX, float fSpeed);
	~BlockClass();

	void Initialize();
	void Update();
	void DrawLoop();
	void Finalize();

	float GetBlockPositionY();
	int EnableDeleteBlock();
private:
	static int m_nDrawGraph;
	float m_fDrawX;
	float m_fDrawY;
	float m_fSpeed;
	float m_fMaxDrawY;
};

