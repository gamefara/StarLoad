#include "SceneMgr.h"

SceneMgr::SceneMgr() : m_eNextScene(SCENE_UNKNOWN), m_ePhase(PHASE_LOAD)
{
	m_pResource = new Resource();
	m_pScene = dynamic_cast<BaseScene*>(new LoadingScene(this, this));
}

SceneMgr::~SceneMgr()
{
	DeletePtr(m_pResource);
	DeletePtr(m_pScene);
}

void SceneMgr::Initialize(){
	m_pScene->BaseScene::Initialize();
	m_pScene->Initialize();
}

void SceneMgr::Update(){
	if(m_eNextScene != SCENE_UNKNOWN){
		Finalize();
		DeletePtr(m_pScene);

		switch(m_eNextScene){
		case SCENE_LOAD:
			m_pScene = dynamic_cast<BaseScene*>(new LoadingScene(this, this));
			break;
		case SCENE_MENU:
			m_pScene = dynamic_cast<BaseScene*>(new MenuScene(this, this));
			break;
		case SCENE_PRACTICE:
			m_pScene = dynamic_cast<BaseScene*>(new PracticeScene(this, this));
			break;
		}
		m_eNextScene = SCENE_UNKNOWN;
		Initialize();
	}

	m_pScene->BaseScene::Update();
	m_pScene->Update();
}

void SceneMgr::DrawLoop(){
	m_pScene->BaseScene::DrawLoop();
	m_pScene->DrawLoop();
}

void SceneMgr::Finalize(){
	m_pScene->BaseScene::Finalize();
	m_pScene->Finalize();
}
