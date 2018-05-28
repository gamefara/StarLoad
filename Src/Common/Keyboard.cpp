#include "Keyboard.h"


Keyboard::Keyboard()
{
	//m_acKey = new char[m_nKeyType];
	//m_anPushKeyCount = new int[m_nKeyType];
	//m_anPullKeyCount = new int[m_nKeyType];
}

Keyboard::~Keyboard()
{
	//delete[] m_acKey;
	//delete[] m_anPushKeyCount;
	//delete[] m_anPullKeyCount;
}

void Keyboard::Update(){
	GetHitKeyStateAll(m_acKey);
	for(int i = 0; i < m_nKeyType; i++){
		if(m_acKey[i] == 0){
			m_anPushKeyCount[i] = 0;
			m_anPullKeyCount[i]++;
		}
		else{
			m_anPushKeyCount[i]++;
			m_anPullKeyCount[i] = 0;
		}

		//ƒJƒEƒ“ƒ^‚ðintŒ^‚Åˆ—‚Å‚«‚é”ÍˆÍ‚É‚·‚é
		if(m_anPushKeyCount[i] < 0) m_anPullKeyCount[i] = 0;
		else if(m_anPullKeyCount[i] < 0) m_anPullKeyCount[i] = 0;
		else if(m_anPushKeyCount[i] >= INT_MAX - 1) m_anPushKeyCount[i] = 1;
		else if(m_anPullKeyCount[i] >= INT_MAX - 1) m_anPullKeyCount[i] = 1;
	}
}

void Keyboard::DrawLoop(){
#ifdef _DEBUG
	DrawFormatString(100, 30, GetColor(255, 255, 255), "IsKeyPush = %d", IsKeyPush(KEY_INPUT_LEFT));
	DrawFormatString(100, 60, GetColor(255, 255, 255), "IsKeyPushHold = %d", IsKeyPushHold(KEY_INPUT_LEFT));
	DrawFormatString(100, 90, GetColor(255, 255, 255), "IsKeyPull = %d", IsKeyPull(KEY_INPUT_LEFT));
	DrawFormatString(100, 120, GetColor(255, 255, 255), "IsKeyPullHold = %d", IsKeyPullHold(KEY_INPUT_LEFT));
	DrawFormatString(100, 150, GetColor(255, 255, 255), "m_nPushKey = %d", m_anPushKeyCount[KEY_INPUT_LEFT]);
	DrawFormatString(100, 180, GetColor(255, 255, 255), "m_nPullKey = %d", m_anPullKeyCount[KEY_INPUT_LEFT]);
#endif _DEBUG
}

int Keyboard::IsKeyPush(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPush = FALSE;
	if(m_acKey[nKey] > 0 && m_anPushKeyCount[nKey] == 1) bPush = TRUE;
	return bPush;
}

int Keyboard::IsKeyPushHold(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPush = FALSE;
	if(m_acKey[nKey] > 0 && m_anPushKeyCount[nKey] >= 1) bPush = TRUE;
	return bPush;
}

int Keyboard::IsKeyPull(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPull = FALSE;
	if(m_acKey[nKey] == 0 && m_anPullKeyCount[nKey] == 1) bPull = TRUE;
	return bPull;
}

int Keyboard::IsKeyPullHold(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPull = FALSE;
	if(m_acKey[nKey] == 0 && m_anPullKeyCount[nKey] >= 1) bPull = TRUE;
	return bPull;
}

int Keyboard::IsKeyFixedPush(int nKey, int nFixedCount){
	if(nKey < 0 || nKey >= m_nKeyType || nFixedCount <= 0 || nFixedCount >= INT_MAX - 1) return FALSE;
	int bFixedPush = FALSE;
	//‰Ÿ‚µ‚½uŠÔ‚àTRUE‚É‚·‚é‚½‚ß‚ÉŠ„‚Á‚½—]‚è‚ð1‚É‚·‚é
	if(m_acKey[nKey] > 0 && m_anPushKeyCount[nKey] % nFixedCount == 1) bFixedPush = TRUE;
	return bFixedPush;
}
