#pragma once
#include "DxLib.h"
#include <algorithm>
#include <assert.h>

const static int Success = 0;							//成功時定数
const static int Invalid = -1;						//失敗時定数
const static int WindowMinX = 0;						//ウィンドウ左端X座標
const static int WindowMaxX = 640;					//ウィンドウ右端X座標
const static int WindowCenX = WindowMaxX / 2;	//ウィンドウ中央X座標
const static int WindowMinY = 0;						//ウィンドウ上端Y座標
const static int WindowMaxY = 480;					//ウィンドウ下端Y座標
const static int WindowCenY = WindowMaxY / 2;	//ウィンドウ中央Y座標

//基底クラス
class Task
{
public:
	virtual ~Task(){};

	//=0となっているものは継承先で実装しなければいけない
	//{}となっているものは継承先で実装しなくてもよい
	virtual void Initialize(){}
	virtual void Update() = 0;
	virtual void DrawLoop() = 0;
	virtual void Finalize(){}

	/// <summary>
	/// フラグ反転処理をします。
	/// </summary>
	/// <param name="bFlag">現在のフラグ</param>
	void ReverseFlag(int& bFlag){ bFlag = !bFlag; }
};

/// <summary>
/// ポインタをデリート後にヌルポインタにします。
/// </summary>
template <class T>
inline void DeletePtr(T*& ptr) {
	delete ptr;
	ptr = nullptr;
}