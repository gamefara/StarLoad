#pragma once
#define _USE_MATH_DEFINES	//円周率πを使用するのに必要

#include "../Task.h"
#include "../Common/IResourceManager.h"
#include "../Common/Animation.h"
#include "../Common/Keyboard.h"
#include "../Common/Json/json11.hpp"
#include "ISceneManager.h"
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

//シーンの基底クラス
class BaseScene : public Task
{
public:
	BaseScene(ISceneManager* pChanger, IResourceManager* pResource);
	virtual ~BaseScene();

	virtual void Initialize();
	virtual void Update();
	virtual void DrawLoop();
	virtual void Finalize();

	/// <summary>
	/// フェーズを設定します。
	/// </summary>
	/// <param name="ePhase">指定フェーズ</param>
	void SetPhase(Phase ePhase)	{ m_pSceneChanger->SetPhase(ePhase); }

	/// <summary>
	/// フェーズを取得します。
	/// </summary>
	/// <returns>フェーズ</returns>
	int GetPhase()						{ return m_pSceneChanger->GetPhase(); }

	/// <summary>
	/// レベルを設定します。
	/// </summary>
	/// <param name="eLevel">指定レベル</param>
	void SetLevel(Level eLevel)	{ m_pSceneChanger->SetLevel(eLevel); }

	/// <summary>
	/// レベルを取得します。
	/// </summary>
	/// <returns>レベル</returns>
	int GetLevel()						{ return m_pSceneChanger->GetLevel(); }

	/// <summary>
	/// 画像ハンドルヲ取得します。
	/// </summary>
	/// <returns>画像ハンドル</returns>
	int GetGraphicsHandle(ResourceImage eImage)	{ return m_pResource->GetGraphicsHandle(eImage); };

	/// <summary>
	/// BGM,SEハンドルを取得します。
	/// </summary>
	/// <returns>BGM,SEハンドル</returns>
	int GetSoundsHandle(ResourceSound eSound)		{ return m_pResource->GetSoundsHandle(eSound); };

	/// <summary>
	/// 画像読み込みカウンタを取得します。
	/// </summary>
	/// <returns>カウンタ</returns>
	int GetGraphicsLoadingCount()						{ return m_pResource->GetGraphicsLoadingCount(); };

	/// <summary>
	/// BGM,SE読み込みカウンタを取得します。
	/// </summary>
	/// <returns>カウンタ</returns>
	int GetSoundsLoadingCount()						{ return m_pResource->GetSoundsLoadingCount(); };

	/// <summary>
	/// カウンタに対応した画像をメモリに読み込みます。
	/// 全ての画像が読み込み完了していればFALSEを返します。
	/// </summary>
	/// <returns>画像読み込み中かどうか</returns>
	int IsLoadGraphics()									{ return m_pResource->IsLoadGraphics(); };

	/// <summary>
	/// カウンタに対応したBGM,SEをメモリに読み込みます。
	/// 全てのBGM,SEが読み込み完了していればFALSEを返します。
	/// </summary>
	/// <returns>BGM,SE読み込み中かどうか</returns>
	int IsLoadSounds()									{ return m_pResource->IsLoadSounds(); };
protected:
	ISceneManager * m_pSceneChanger;
	IResourceManager* m_pResource;
	Animation* m_pAnime;
	Keyboard* m_pKey;
	std::fstream m_fStream;

	json11::Json JsonConfigData = json11::Json::object{
		{ "BGM", 60 },
		{ "SE", 60 }
	};

	json11::Json JsonResultData = json11::Json::object{
		{
			"Begginer", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 50000 },
				{ "Rank", "D" }
			}
		},
		{
			"Standard", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 50000 },
				{ "Rank", "D" }
			}
		},
		{
			"Hard", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 50000 },
				{ "Rank", "D" }
			}
		}
	};

	const char* m_sResultFileName = "Resource/SaveData/Result.json";
	const char* m_sConfigFileName = "Resource/SaveData/Config.json";

	void InitializeLoadConfigStream();
	void InitializeLoadResultStream();
	void FinalizeLoadConfigStream();
	void FinalizeLoadResultStream();
	int EnableConvertDateTime(std::string sDatetime);
	std::string GetNowDateTime();

	/// <summary>
	/// BGMの音量を設定します。
	/// </summary>
	/// <param name="nVolume">音量％</param>
	void SetGameDataVolumeBGM(int nVolume)	{ m_nVolumeBGM = nVolume; }

	/// <summary>
	/// SEの音量を設定します。
	/// </summary>
	/// <param name="nVolume">音量％</param>
	void SetGameDataVolumeSE(int nVolume)	{ m_nVolumeSE = nVolume; }

	/// <summary>
	/// BGMの音量を取得します。
	/// </summary>
	/// <returns>音量％</returns>
	int GetGameDataVolumeBGM()	const			{ return m_nVolumeBGM; }

	/// <summary>
	/// SEの音量を取得します。
	/// </summary>
	/// <returns>音量％</returns>
	int GetGameDataVolumeSE()	const			{ return m_nVolumeSE; }

	/// <summary>
	/// 日付を設定します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <param name="sTime">日付文字列</param>
	void SetGameDataDateTime(int nLevel, std::string sTime){
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return;
		m_asDatetime[nLevel] = sTime;
	}

	/// <summary>
	/// スコアを設定します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <param name="nScore">スコア</param>
	void SetGameDataScore(int nLevel, int nScore){
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return;
		m_anScore[nLevel] = nScore;
	}

	/// <summary>
	/// ランクを設定します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <param name="sRank">ランク</param>
	void SetGameDataRank(int nLevel, std::string sRank){
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return;
		m_asRank[nLevel] = sRank;
	}

	/// <summary>
	/// 日付を取得します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <returns>日付文字列</returns>
	std::string GetGameDataDateTime(int nLevel) const {
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return "2018/05/01 00:00:00";
		return m_asDatetime[nLevel];
	}

	/// <summary>
	/// スコアを取得します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <returns>スコア</returns>
	int GetGameDataScore(int nLevel) const {
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return Invalid;
		return m_anScore[nLevel];
	}

	/// <summary>
	/// ランクを取得します。
	/// </summary>
	/// <param name="nLevel">レベル</param>
	/// <returns>ランク</returns>
	std::string GetGameDataRank(int nLevel) const {
		if(nLevel < 0 || nLevel > static_cast<int>(LEVEL_HARD)) return std::to_string(Invalid);
		return m_asRank[nLevel];
	}
private:
	int m_nVolumeBGM = 0;
	int m_nVolumeSE = 0;
	std::string m_asDatetime[3] = { "","","" };
	int m_anScore[3] = { 0,0,0 };
	std::string m_asRank[3] = { "","","" };
};

