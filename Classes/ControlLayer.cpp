#include "ControlLayer.h"

ControlLayer::ControlLayer(void)
{
	_scoreItem=NULL;
	_pauseItem=NULL;
	//_noTouchLayer = NULL;
}

ControlLayer::~ControlLayer(void)
{
}

bool ControlLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		Size winSize=Director::getInstance()->getWinSize();

		Sprite* normalPause=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_pause_nor.png"));
		Sprite* pressedPause=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_pause_pressed.png"));
		_pauseItem=MenuItemImage::create();
        _pauseItem->setNormalImage(normalPause);
		_pauseItem->setSelectedImage(pressedPause);
		_pauseItem->setCallback(CC_CALLBACK_1(ControlLayer::menuPauseCallback,this));
		_pauseItem->setPosition(Point(normalPause->getContentSize().width/2+10,winSize.height-normalPause->getContentSize().height/2-10));
		Menu *menuPause=Menu::create(_pauseItem,NULL);
		menuPause->setPosition(Point::ZERO);
		this->addChild(menuPause,101);


		_scoreItem=LabelBMFont::create("0","font/font.fnt");
		_scoreItem->setColor(Color3B(143,146,147));
		_scoreItem->setAnchorPoint(Point(0,0.5));
		_scoreItem->setPosition(Point(_pauseItem->getPositionX()+normalPause->getContentSize().width/2+5,_pauseItem->getPositionY()));
		this->addChild(_scoreItem);

		bRet=true;
	} while (0);

	return bRet;
}

void ControlLayer::menuPauseCallback(Object* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	if(!Director::getInstance()->isPaused())
	{
		_pauseItem->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_resume_nor.png"));
		_pauseItem->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_resume_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		Director::getInstance()->pause();
		//_noTouchLayer=NoTouchLayer::create();
		//this->addChild(_noTouchLayer);
	}
	else
	{
		_pauseItem->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_pause_nor.png"));
		_pauseItem->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_pause_pressed.png"));
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		//this->removeChild(_noTouchLayer,true);
	}
}

void ControlLayer::updateScore(int score)
{
	if (score>=0 && score<=MAX_SCORE)
	{
		__String* strScore=__String::createWithFormat("%d",score);
		_scoreItem->setString(strScore->_string.c_str());
	}
}
