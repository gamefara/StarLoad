#include "Keyboard.h"

/// <summary>
/// 更新処理
/// </summary>
void Keyboard::Update(){
	GetHitKeyStateAll(m_asKey);
	for(int i = 0; i < m_nKeyType; i++){
		if(m_asKey[i] == 0){
			m_anPushKeyCount[i] = 0;
			m_anPullKeyCount[i]++;
		}
		else{
			m_anPushKeyCount[i]++;
			m_anPullKeyCount[i] = 0;
		}

		//カウンタをint型で処理できる範囲にする
		if(m_anPushKeyCount[i] < 0) m_anPullKeyCount[i] = 0;
		else if(m_anPullKeyCount[i] < 0) m_anPullKeyCount[i] = 0;
		else if(m_anPushKeyCount[i] >= INT_MAX - 1) m_anPushKeyCount[i] = 1;
		else if(m_anPullKeyCount[i] >= INT_MAX - 1) m_anPullKeyCount[i] = 1;
	}
}

/// <summary>
/// 指定キーを押したかチェックします。押した瞬間のみTRUEを返します。
/// </summary>
/// <param name="nKey">押したキーの種類</param>
/// <returns>押したかどうか</returns>
int Keyboard::IsKeyPush(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPush = FALSE;
	if(m_asKey[nKey] > 0 && m_anPushKeyCount[nKey] == 1) bPush = TRUE;
	return bPush;
}

/// <summary>
/// 指定キーを押しているかチェックします。押している間TRUEを返します。
/// </summary>
/// <param name="nKey">押しているキーの種類</param>
/// <returns>押しているかどうか</returns>
int Keyboard::IsKeyPushHold(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPush = FALSE;
	if(m_asKey[nKey] > 0 && m_anPushKeyCount[nKey] >= 1) bPush = TRUE;
	return bPush;
}

/// <summary>
/// 指定キーを離したかチェックします。離した瞬間TRUEを返します。
/// </summary>
/// <param name="nKey">離したキーの種類</param>
/// <returns>離したかどうか</returns>
int Keyboard::IsKeyPull(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPull = FALSE;
	if(m_asKey[nKey] == 0 && m_anPullKeyCount[nKey] == 1) bPull = TRUE;
	return bPull;
}

/// <summary>
/// 指定キーを話しているかチェックします。離している間TRUEを返します。
/// </summary>
/// <param name="nKey">離しているキーの種類</param>
/// <returns>離しているかどうか</returns>
int Keyboard::IsKeyPullHold(int nKey){
	if(nKey < 0 || nKey >= m_nKeyType) return FALSE;
	int bPull = FALSE;
	if(m_asKey[nKey] == 0 && m_anPullKeyCount[nKey] >= 1) bPull = TRUE;
	return bPull;
}

/// <summary>
/// 押しているキーが指定時間ごとにTRUEを返します。
/// </summary>
/// <param name="nKey">押しているキーの種類</param>
/// <param name="nFixedCount">TRUEを返す間隔(単位：フレーム)</param>
/// <returns>指定時間になったかどうか</returns>
int Keyboard::IsKeyFixedPush(int nKey, int nFixedCount){
	if(nKey < 0 || nKey >= m_nKeyType || nFixedCount <= 0 || nFixedCount >= INT_MAX - 1) return FALSE;
	int bFixedPush = FALSE;
	//押した瞬間もTRUEにするために割った余りを1にする
	if(m_asKey[nKey] > 0 && m_anPushKeyCount[nKey] % nFixedCount == 1) bFixedPush = TRUE;
	return bFixedPush;
}
