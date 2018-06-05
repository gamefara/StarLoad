#include "Keyboard.h"


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
