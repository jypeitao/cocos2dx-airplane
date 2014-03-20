#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	_gameOverLayer=NULL;
	_score=0;
}

GameOverScene::~GameOverScene(void)
{

}

GameOverScene* GameOverScene::create(int passScore)
{
	GameOverScene *pRet = new GameOverScene(); 
	pRet->_score=passScore;
	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

bool GameOverScene::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		_gameOverLayer=GameOverLayer::create(_score);
		this->addChild(_gameOverLayer);

		bRet=true;
	} while (0);
	return bRet;
}
