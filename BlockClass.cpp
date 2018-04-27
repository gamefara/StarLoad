#include "BlockClass.h"
#include "DxLib.h"

BlockClass::BlockClass(float fDrawX, float fSpeed)
{
	m_fDrawX = fDrawX;
	m_fDrawY = 0.0f;
	m_fSpeed = fSpeed;
	m_fMaxDrawY = 500.0f;
}


BlockClass::~BlockClass()
{
}

void BlockClass::Initialize(){

}

void BlockClass::Update(){

	m_fDrawY += m_fSpeed;
}

void BlockClass::DrawLoop(){

}

void BlockClass::Finalize(){

}

float BlockClass::GetBlockPositionY(){
	return m_fDrawY;
}

int BlockClass::EnableDeleteBlock(){
	int bDelete = FALSE;
	if(m_fDrawY > m_fMaxDrawY) bDelete = TRUE;
	return bDelete;
}