#include "BaseScene.h"

BaseScene::BaseScene(ISceneManager* pChanger, IResourceManager* pResource)
{
	m_pSceneChanger = pChanger;
	m_pResource = pResource;

	if(m_pAnime == nullptr) m_pAnime = new Animation();
	if(m_pKey == nullptr) m_pKey = new Keyboard();
}

BaseScene::~BaseScene()
{
	DeletePtr(m_pKey);
	DeletePtr(m_pAnime);
}

/// <summary>
/// 初期化処理
/// </summary>
void BaseScene::Initialize(){
	//ファイルチェックをし、なければデフォルト値を作成する
	InitializeLoadConfigStream();
	InitializeLoadResultStream();

	m_pAnime->Initialize();
	m_pKey->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void BaseScene::Update(){
	m_pAnime->Update();
	m_pKey->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void BaseScene::DrawLoop(){
	m_pAnime->DrawLoop();
	m_pKey->DrawLoop();
}

/// <summary>
/// 終了処理
/// </summary>
void BaseScene::Finalize(){
	FinalizeLoadConfigStream();
	FinalizeLoadResultStream();

	m_pAnime->Finalize();
	m_pKey->Finalize();
}

/// <summary>
/// Config.jsonファイルを読み込み、音量設定に反映します。
/// ファイルがない時や破損しているときは、新たにjsonファイルを作成し、初期値を書き込みます。
/// </summary>
void BaseScene::InitializeLoadConfigStream(){
	int nBGMVolume = 0;
	int nSEVolume = 0;
	try
	{
		m_fStream.close();
		m_fStream.clear();
		m_fStream.open(m_sConfigFileName, std::ios_base::in);
		if(m_fStream.fail()) throw std::exception();

		//データ読み込み
		std::string sJsonWord;
		std::string line;
		while(std::getline(m_fStream, line)) sJsonWord += line;

		std::string sErr;
		auto jsonConfig = json11::Json::parse(sJsonWord, sErr);
		if(!sErr.empty()) throw std::exception();
		if(!jsonConfig["BGM"].is_number()) throw std::exception();
		if(!jsonConfig["SE"].is_number()) throw std::exception();

		nBGMVolume = jsonConfig["BGM"].int_value();
		nSEVolume = jsonConfig["SE"].int_value();
		SetGameDataVolumeBGM(nBGMVolume);
		SetGameDataVolumeSE(nSEVolume);
	}
	catch(...)
	{
		//エラーの場合はJson新規作成して、設定初期データを書き込む
		m_fStream.close();
		m_fStream.clear();
		m_fStream.open(m_sConfigFileName, std::ios_base::out);
		m_fStream << JsonConfigData.dump() << std::endl;
		nBGMVolume = JsonConfigData["BGM"].int_value();
		nSEVolume = JsonConfigData["SE"].int_value();
		SetGameDataVolumeBGM(nBGMVolume);
		SetGameDataVolumeSE(nSEVolume);
	}
	m_fStream.close();
	m_pResource->SetSoundsBGMVolume(nBGMVolume);
	m_pResource->SetSoundsSEVolume(nSEVolume);
}

/// <summary>
/// Result.jsonファイルを読み込み、結果に反映します。
/// ファイルがない時や破損しているときは、新たにjsonファイルを作成し、初期値を書き込みます。
/// </summary>
void BaseScene::InitializeLoadResultStream(){
	const int nResultNum = 3;
	std::string asLevel[nResultNum] = { "Begginer", "Standard", "Hard" };
	try
	{
		m_fStream.close();
		m_fStream.clear();
		m_fStream.open(m_sResultFileName, std::ios_base::in);
		if(m_fStream.fail()) throw std::exception();
		//データ読み込み
		std::string sJsonWord;
		std::string line;
		while(std::getline(m_fStream, line)) sJsonWord += line;

		std::string sErr;
		auto jsonResult = json11::Json::parse(sJsonWord, sErr);
		if(!sErr.empty()) throw std::exception();

		for(int i = 0; i < nResultNum; i++){
			auto jsonKey = jsonResult[asLevel[i]];
			if(jsonKey.is_null()) throw std::exception();
			if(jsonKey["DateTime"].is_null() || jsonKey["Score"].is_null() || jsonKey["Rank"].is_null()) throw std::exception();

			auto sDatetime = jsonKey["DateTime"].string_value();
			if(!EnableConvertDateTime(sDatetime)) throw std::exception();
			if(!jsonKey["Score"].is_number() || jsonKey["Score"] < 0) throw std::exception();
			if(!jsonKey["Rank"].is_string()) throw std::exception();
			
			//ランク対応の文字かチェック
			std::string sRank = jsonKey["Rank"].string_value();
			std::string sSearch = "SABCD";
			if(sSearch.find(sRank) == Invalid) std::exception();
		}
		for(int i = 0; i < nResultNum; i++){
			auto jsonKey = jsonResult[asLevel[i]];
			SetGameDataDateTime(i, jsonKey["DateTime"].string_value());
			SetGameDataScore(i, jsonKey["Score"].int_value());
			SetGameDataRank(i, jsonKey["Rank"].string_value());
		}
	}
	catch(...)
	{
		m_fStream.close();
		m_fStream.clear();
		m_fStream.open(m_sResultFileName, std::ios_base::out);
		m_fStream << JsonResultData.dump() << std::endl;
		for(int i = 0; i < nResultNum; i++){
			auto jsonKey = JsonResultData[asLevel[i]];
			SetGameDataDateTime(i, jsonKey["DateTime"].string_value());
			SetGameDataScore(i, jsonKey["Score"].int_value());
			SetGameDataRank(i, jsonKey["Rank"].string_value());
		}
	}
	m_fStream.close();
}

/// <summary>
/// 指定の文字列が日付かどうかをチェックします。
/// </summary>
/// <param name="sDatetime">対象文字列</param>
/// <returns>日付形式(YYYY/mm/dd HH:ii:ss)かどうか</returns>
int BaseScene::EnableConvertDateTime(std::string sDatetime){
	time_t tNow = time(nullptr);
	tm tStruct;
	localtime_s(&tStruct, &tNow);
	int bConvert = TRUE;
	try
	{
		//YYYY/mm/dd HH:ii:ssの形式かチェック
		if(sDatetime.find("/") != 4) throw std::exception();
		if(sDatetime.find("/", 5) != 7) throw std::exception();
		if(sDatetime.find(" ") != 10) throw std::exception();
		if(sDatetime.find(":") != 13) throw std::exception();
		if(sDatetime.find(":", 14) != 16) throw std::exception();

		//構造体では年が1900年からの通算年、月は実際の月-1された値になっている
		tStruct.tm_year = std::stoi(sDatetime.substr(0, 4)) - 1900;
		tStruct.tm_mon = std::stoi(sDatetime.substr(5, 2)) - 1;
		tStruct.tm_mday = std::stoi(sDatetime.substr(8, 3));
		tStruct.tm_hour = std::stoi(sDatetime.substr(11, 2));
		tStruct.tm_min = std::stoi(sDatetime.substr(14, 2));
		tStruct.tm_sec = std::stoi(sDatetime.substr(17, 2));
		if(mktime(&tStruct) == Invalid) throw std::exception();
	}
	catch(const std::exception&){ bConvert = FALSE; }
	return bConvert;
}

/// <summary>
/// Config.jsonファイルに音量情報を書き込みます。
/// </summary>
void BaseScene::FinalizeLoadConfigStream(){
	json11::Json JsonSaveConfigData = json11::Json::object{
		{ "BGM", GetGameDataVolumeBGM() },
		{ "SE", GetGameDataVolumeSE() }
	};
	m_fStream.clear();
	m_fStream.open(m_sConfigFileName, std::ios_base::out);
	m_fStream << JsonSaveConfigData.dump() << std::endl;
	m_fStream.close();
}

/// <summary>
/// Result.jsonファイルに結果情報を書き込みます。
/// </summary>
void BaseScene::FinalizeLoadResultStream(){
	json11::Json JsonSaveResultData = json11::Json::object{
		{
			"Begginer", json11::Json::object{
				{ "DateTime", GetGameDataDateTime(static_cast<int>(LEVEL_BEGINNER)).c_str() },
				{ "Score", GetGameDataScore(static_cast<int>(LEVEL_BEGINNER)) },
				{ "Rank", GetGameDataRank(static_cast<int>(LEVEL_BEGINNER)).c_str() }
			}
		},
		{
			"Standard", json11::Json::object{
				{ "DateTime", GetGameDataDateTime(static_cast<int>(LEVEL_STANDARD)).c_str() },
				{ "Score", GetGameDataScore(static_cast<int>(LEVEL_STANDARD)) },
				{ "Rank", GetGameDataRank(static_cast<int>(LEVEL_STANDARD)).c_str() }
			}
		},
		{
			"Hard", json11::Json::object{
				{ "DateTime", GetGameDataDateTime(static_cast<int>(LEVEL_HARD)).c_str() },
				{ "Score", GetGameDataScore(static_cast<int>(LEVEL_HARD)) },
				{ "Rank", GetGameDataRank(static_cast<int>(LEVEL_HARD)).c_str() }
			}
		}
	};
	m_fStream.clear();
	m_fStream.open(m_sResultFileName, std::ios_base::out);
	m_fStream << JsonSaveResultData.dump() << std::endl;
	m_fStream.close();
}

/// <summary>
/// 現在時間の文字列を取得します。
/// </summary>
/// <returns>現在時間の文字列</returns>
std::string BaseScene::GetNowDateTime(){
	//現在時間をそのまま文字列化
	time_t tNow = time(nullptr);
	tm tStruct;
	localtime_s(&tStruct, &tNow);
	std::string sYear = std::to_string(tStruct.tm_year + 1900);
	std::string sMonth = std::to_string(tStruct.tm_mon + 1);
	std::string sDay = std::to_string(tStruct.tm_mday);
	std::string sHour = std::to_string(tStruct.tm_hour);
	std::string sMinute = std::to_string(tStruct.tm_min);
	std::string sSecond = std::to_string(tStruct.tm_sec);
	if(sMonth.size() == 1) sMonth = "0" + sMonth;
	if(sDay.size() == 1) sDay = "0" + sDay;
	if(sHour.size() == 1) sHour = "0" + sHour;
	if(sMinute.size() == 1) sMinute = "0" + sMinute;
	if(sSecond.size() == 1) sSecond = "0" + sSecond;
	std::string sDateTime = sYear + "/" + sMonth + "/" + sDay + " " + sHour + ":" + sMinute + ":" + sSecond;
	return sDateTime;
}