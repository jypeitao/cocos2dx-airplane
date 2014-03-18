#include "GameOverLayer.h"
#include "GameScene.h"

int GameOverLayer::highestHistoryScore=0;

GameOverLayer::GameOverLayer(void)
{
	score=0;
	highestScore=NULL;
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore)
{
	GameOverLayer *pRet = new GameOverLayer();
	pRet->score=passScore;
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

bool GameOverLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/game_over.mp3");

		Size winSize=Director::getInstance()->getWinSize();


	    Sprite* background=Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gameover.png"));
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);

		//����BackMenu
		Sprite* normalBackToGame=Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_finish.png"));
		Sprite* pressedBackToGame=Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_finish.png"));
		MenuItemImage* pBackItem=MenuItemImage::create();
		pBackItem->initWithNormalSprite(normalBackToGame,pressedBackToGame,NULL,this,menu_selector(GameOverLayer::menuBackCallback));
		pBackItem->setPosition(ccp(winSize.width-normalBackToGame->getContentSize().width/2-10,normalBackToGame->getContentSize().height/2+10));
		Menu *menuBack=Menu::create(pBackItem,NULL);
		menuBack->setPosition(Point::ZERO);
		this->addChild(menuBack);

		//����score
		__String* strScore=__String::createWithFormat("%d",score);
		LabelBMFont* finalScore=LabelBMFont::create(strScore->_string.c_str(),"font/font.fnt");
		finalScore->setColor(ccc3(143,146,147));
		finalScore->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(finalScore);

		DelayTime* delay=DelayTime::create(1.0f);
		ScaleTo* scalebig=ScaleTo::create(1.0f,3.0f);
		ScaleTo* scalelittle=ScaleTo::create(0.3f,2.0f);
		CallFunc* showAD=CallFunc::create(this,callfunc_selector(GameOverLayer::showAD));
		FiniteTimeAction* sequence=Sequence::create(delay,scalebig,scalelittle,showAD,NULL);
		finalScore->runAction(sequence);

		//����tip
		//LabelTTF* tip=LabelTTF::create("Congratulations! Your Score is","Monaco",20);
		//tip->setColor(ccc3(143,146,147));
		//tip->setPosition(ccp(finalScore->getPosition().x,finalScore->getPosition().y+finalScore->getContentSize().height+50));
		//this->addChild(tip);
		__String* strHighestScore=__String::createWithFormat("%d",highestHistoryScore);
		highestScore=LabelBMFont::create(strHighestScore->_string.c_str(),"font/font.fnt");
		highestScore->setColor(ccc3(143,146,147));
		highestScore->setAnchorPoint(ccp(0,0.5));
		highestScore->setPosition(ccp(140,winSize.height-30));
		this->addChild(highestScore);

		if (score>highestHistoryScore)
		{
			UserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score);
			highestHistoryScore=score;
			DelayTime* delayChange=DelayTime::create(1.3f);
			MoveBy* moveOut=MoveBy::create(0.1f,ccp(0,100));
			CallFuncN* beginChange=CallFuncN::create(this,callfuncN_selector(GameOverLayer::beginChangeHighestScore));
			MoveBy* moveIn=MoveBy::create(0.1f,ccp(0,-100));
			FiniteTimeAction* sequence=Sequence::create(delayChange,moveOut,beginChange,moveIn,NULL);
			highestScore->runAction(sequence);
		}

		this->setKeypadEnabled(true);

		bRet=true;
	} while (0);
	return bRet;
}

void GameOverLayer::menuBackCallback(Object* pSender)
{
	Scene* pScene=GameScene::create();
	TransitionSlideInL* animateScene=TransitionSlideInL::create(1.0f,pScene);
	Director::sharedDirector()->replaceScene(animateScene);
}

void GameOverLayer::keyBackClicked()
{
	Director::sharedDirector()->end();
}

void GameOverLayer::beginChangeHighestScore(Node* pNode)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/achievement.mp3");
	__String* changeScore=__String::createWithFormat("%d",score);
	highestScore->setString(changeScore->_string.c_str());
}

void GameOverLayer::showAD()
{
	
}
