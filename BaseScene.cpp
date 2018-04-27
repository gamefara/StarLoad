#include "BaseScene.h"
#include "DxLib.h"

BaseScene::BaseScene(){
	if(m_pAnime == nullptr) m_pAnime = new Animation();
}

BaseScene::BaseScene(ISceneManager* changer)
{
	m_pSceneChanger = changer;
	if(m_pAnime == nullptr) m_pAnime = new Animation();
	m_anImageHandle.resize(m_nImageHandleCount);
}

BaseScene::~BaseScene()
{
}

void BaseScene::Initialize(){
	m_pAnime->Initialize();
}

void BaseScene::Update(){
	m_pAnime->Update();
}

void BaseScene::DrawLoop(){
	m_pAnime->DrawLoop();
}

void BaseScene::Finalize(){
	m_pAnime->Finalize();
}

void BaseScene::SetPhase(Phase ePhase){
	m_pSceneChanger->SetPhase(ePhase);
}

int BaseScene::GetPhase(){
	return m_pSceneChanger->GetPhase();
}

std::string BaseScene::GetCurrentDateTime() {
	time_t tNow = time(nullptr);
	struct tm tStruct;
	localtime_s(&tStruct, &tNow);
	std::string sYear = std::to_string(tStruct.tm_year + 1900);
	std::string sMonth = std::to_string(tStruct.tm_mon + 1);
	std::string sDay = std::to_string(tStruct.tm_mday);
	std::string sHour = std::to_string(tStruct.tm_hour);
	std::string sMinute = std::to_string(tStruct.tm_min);
	std::string sSecond = std::to_string(tStruct.tm_sec);
	if(sMonth.length() == 1) sMonth = "0" + sMonth;
	if(sDay.length() == 1) sDay = "0" + sDay;
	if(sHour.length() == 1) sHour = "0" + sHour;
	if(sMinute.length() == 1) sMinute = "0" + sMinute;
	if(sSecond.length() == 1) sSecond = "0" + sSecond;

	std::string sDate = sYear + "/" + sMonth + "/" + sDay;
	std::string sTime = sHour + ":" + sMinute + ":" + sSecond;
	std::string sDateTime = sDate + " " + sTime;
	return sDateTime;
}
