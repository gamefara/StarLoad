#pragma once

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

//シーンの基底クラス
class BaseScene : public Task
{
public:

	//ハンドル格納用変数
	int* m_anImageHandle;
	int* m_anSoundHandle;

	BaseScene(ISceneManager* pChanger, IResourceManager* pResource);
	virtual ~BaseScene();

	virtual void Initialize();
	virtual void Update();
	virtual void DrawLoop();
	virtual void Finalize();

	void SetPhase(Phase ePhase)	{ m_pSceneChanger->SetPhase(ePhase); };
	int GetPhase()						{ return m_pSceneChanger->GetPhase(); };
	void SetLevel(Level eLevel)	{ m_pSceneChanger->SetLevel(eLevel); };
	int GetLevel()						{ return m_pSceneChanger->GetLevel(); };

	int GetGraphicsHandle(ResourceImage eImage)	{ return m_pResource->GetGraphicsHandle(eImage); };
	int GetSoundsHandle()								{ return m_pResource->GetSoundsHandle(); };
	int GetGraphicsLoadingCount()						{ return m_pResource->GetGraphicsLoadingCount(); };
	int GetSoundsLoadingCount()						{ return m_pResource->GetSoundsLoadingCount(); };
	int IsLoadGraphics()									{ return m_pResource->IsLoadGraphics(); };
	int IsLoadSounds()									{ return m_pResource->IsLoadSounds(); };
protected:
	ISceneManager * m_pSceneChanger;
	IResourceManager* m_pResource;
	Animation* m_pAnime;
	Keyboard* m_pKey;
	std::fstream m_fStream;

	json11::Json JsonConfigData = json11::Json::object{
		{ "BGM", 60 },
		{ "SE", 40 }
	};

	json11::Json JsonResultData = json11::Json::object{
		{
			"Begginer", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 100000 },
				{ "Rank", "D" }
			}
		},
		{
			"Standard", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 100000 },
				{ "Rank", "D" }
			}
		},
		{
			"Hard", json11::Json::object{
				{ "DateTime", "2018/05/01 00:00:00" },
				{ "Score", 100000 },
				{ "Rank", "D" }
			}
		}
	};

	const char* m_sResultFileName = "Resource/Data/Result.json";
	const char* m_sConfigFileName = "Resource/Data/Config.json";
	void InitializeLoadConfigStream();
	void InitializeLoadResultStream();
	void FinalizeLoadConfigStream();
	void FinalizeLoadResultStream();
	int EnableConvertDateTime(std::string sDatetime);
	std::string GetNowDateTime();

	void SetGameDataVolumeBGM(int nVolume){ m_nVolumeBGM = nVolume; }
	void SetGameDataVolumeSE(int nVolume)	{ m_nVolumeSE = nVolume; }
	int GetGameDataVolumeBGM()		const { return m_nVolumeBGM; }
	int GetGameDataVolumeSE()			const	{ return m_nVolumeSE; }

	void SetGameDataDateTime(int nLevel, std::string sTime){
		if(nLevel < 0 || nLevel > (int)Level::LEVEL_HARD) return;
		m_asDatetime[nLevel] = sTime;
	}
	void SetGameDataScore(int nLevel, int nScore){
		if(nLevel < 0 || nLevel >(int)Level::LEVEL_HARD) return;
		m_anScore[nLevel] = nScore;
	}
	void SetGameDataRank(int nLevel, std::string sRank){
		if(nLevel < 0 || nLevel >(int)Level::LEVEL_HARD) return;
		m_asRank[nLevel] = sRank;
	}
	std::string GetGameDataDateTime(int nLevel) const {
		if(nLevel < 0 || nLevel >(int)Level::LEVEL_HARD) return std::to_string(Invalid);
		return m_asDatetime[nLevel];
	}
	int GetGameDataScore(int nLevel) const {
		if(nLevel < 0 || nLevel >(int)Level::LEVEL_HARD) return Invalid;
		return m_anScore[nLevel];
	}
	std::string GetGameDataRank(int nLevel) const {
		if(nLevel < 0 || nLevel >(int)Level::LEVEL_HARD) return std::to_string(Invalid);
		return m_asRank[nLevel];
	}
private:
	int m_nVolumeBGM = 0;
	int m_nVolumeSE = 0;
	std::string m_asDatetime[3] = { "","","" };
	int m_anScore[3] = { 0,0,0 };
	std::string m_asRank[3] = { "","","" };
};

