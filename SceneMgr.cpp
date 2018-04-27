#include "DxLib.h"
#include "LoadingScene.h"
#include "MenuScene.h"
#include "PracticeScene.h"
#include "BattleScene.h"
#include "ReplayScene.h"
#include "ConfigScene.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr() : m_eNextScene(SCENE_UNKNOWN), m_eNextPhase(PHASE_TITLE)
{
	m_pScene = (BaseScene*) new MenuScene(this);
	m_pAnimeScene = m_pScene->m_pAnime;
}

void SceneMgr::Initialize(){
	m_pScene->Initialize();
	m_pAnimeScene->Initialize();
}

void SceneMgr::Update(){
	if(m_eNextScene != SCENE_UNKNOWN){
		m_pScene->Finalize();
		delete m_pScene;

		switch(m_eNextScene){
		case SCENE_LOAD:
			m_pScene = (BaseScene*) new LoadingScene(this);
			break;
		case SCENE_MENU:
			m_pScene = (BaseScene*) new MenuScene(this);
			break;
		case SCENE_PRACTICE:
			m_pScene = (BaseScene*) new PracticeScene(this);
			break;
		case SCENE_BATTLE:
			m_pScene = (BaseScene*) new BattleScene(this);
			break;
		case SCENE_REPLAY:
			m_pScene = (BaseScene*) new ReplayScene(this);
			break;
		case SCENE_CONFIG:
			m_pScene = (BaseScene*) new ConfigScene(this);
			break;
		}
		m_eNextScene = SCENE_UNKNOWN;
		m_pScene->Initialize();
		m_pAnimeScene = m_pScene->m_pAnime;
	}

	m_pScene->Update();
	m_pAnimeScene->Update();
}

void SceneMgr::DrawLoop(){
	m_pScene->DrawLoop();
	m_pAnimeScene->DrawLoop();
}

void SceneMgr::Finalize(){
	m_pScene->Finalize();
	m_pAnimeScene->Finalize();
}

void SceneMgr::ChangeScene(Scene eScene){
	//次のシーンをセットする
	m_eNextScene = eScene;
}

void SceneMgr::SetPhase(Phase ePhase){
	m_eNextPhase = ePhase;
}

int SceneMgr::GetPhase(){
	return m_eNextPhase;
}

SceneMgr::~SceneMgr()
{
	delete m_pScene;
}

