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

/// <summary>
/// ‰Šú‰»ˆ—
/// </summary>
void SceneMgr::Initialize(){
	m_pScene->BaseScene::Initialize();
	m_pScene->Initialize();
}

/// <summary>
/// XVˆ—
/// </summary>
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

/// <summary>
/// •`‰æˆ—
/// </summary>
void SceneMgr::DrawLoop(){
	m_pScene->BaseScene::DrawLoop();
	m_pScene->DrawLoop();
}

/// <summary>
/// I—¹ˆ—
/// </summary>
void SceneMgr::Finalize(){
	m_pScene->BaseScene::Finalize();
	m_pScene->Finalize();
}
