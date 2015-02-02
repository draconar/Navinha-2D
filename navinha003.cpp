// navinha002.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CApp.h"
#include <iostream>
#include "ErrorLogManager.h"
#include "ResourceManager.h"
#include "2DRenderManager.h"
#include "2DSceneManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include "constants.h"

#include "debug.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//create instance of errorLogManager

	CErrorLogManager* log = CErrorLogManager::getErrorManager();
	log->create("log.txt");

	try {
		
		c2DRenderManager* g_renderManager = c2DRenderManager::get2DRenderManager();
		c2DSceneManager* g_sceneManager = c2DSceneManager::get2DSceneManager();
		AudioManager* audioManager = AudioManager::getAudioManager();

		//screenW & screenH are CONSTS (constants.h)
		g_renderManager->init(screenW,screenH,false,"WINDOW");

			//Input manager & friends
		InputManager inputManager;
		inputManager.init();

		ResourceManager* resourceManager = ResourceManager::getResourceManager();
		resourceManager->loadFromXMLFile("ResourceTree.xml");
		resourceManager->printResources();
		resourceManager->setCurrentScope(0);

		g_sceneManager->init("SceneTree.xml");

		g_renderManager->sceneManager=g_sceneManager;

		cTestListener Tst;

		g_sceneManager->addListener(&Tst);
		g_sceneManager->addTimer(0,1000);

		//teste de audio (Fábio Picchi)
		//audioManager->init();
		//audioManager->playFromResource ((AudioManager*) resourceManager->findResourceByID(5));

		//audioManager->playFromResource ((AudioResource*) resourceManager->findResourceByID(5));
		//end teste audio

		//teste de entity
		EntityManager* entityManager = EntityManager::getEntityManager();

		entityManager->init(g_sceneManager->sprite_map);
		cEntityTestListener entTst;
		entityManager->addListener(&entTst);
		entityManager->addTimer(0,500);
		//end teste de entity

		//teste de input ligado a um entity
		EntityFactory* player = entityManager->getSingleEntity("player");
		TestInputListener tstInputListener(player);
		inputManager.addListener(&tstInputListener);

		EntityFactory* meteor = entityManager->getSingleEntity("meteor");
		MouseInputListener mouseInputListener(meteor); 
		inputManager.addListener(&mouseInputListener);


		//end teste de input ligado a um entity

		//teste de PSX
			
			PhysicsManager psx;
			testPSXlistener tpsx;
			mousePSXListener mpsx;
			psx.addListener(&tpsx);
			psx.addListener(&mpsx);
			psx.sceneManager = g_sceneManager;
			psx.entityManager = entityManager;
			psx.init(entityManager->entities_map);
			
		//end teste de PSX

		while (g_renderManager->update())
		{
		   g_sceneManager->update();
		   entityManager->update();
		   psx.update();
		   inputManager.update();
		   audioManager->update();
		}

	// Limpando tudo
	psx.clear();
	entityManager->clear();
	g_sceneManager->clear();
	resourceManager->clear();
	inputManager.clear();
    g_renderManager->clear();


	} catch (CException e){

		log->logBuffer << "*****ERROR*****\n";
		log->flush();
		log->logException(e);
		log->logBuffer << "*****$$$$$*****\n";
		log->flush();
	}

	//	CApp theApp;
	//  return theApp.OnExecute();

	log->close();
	_CrtDumpMemoryLeaks();

	
}