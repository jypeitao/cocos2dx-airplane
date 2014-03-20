#include "PlaneLayer.h"
#include "GameOverScene.h"

PlaneLayer* PlaneLayer::s_sharedPlane=NULL;

PlaneLayer::PlaneLayer(void)
{
	_isAlive=true;
	_score=0;
}

PlaneLayer::~PlaneLayer(void)
{
}

PlaneLayer* PlaneLayer::create() 
{ 
	PlaneLayer *pRet = new PlaneLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		s_sharedPlane=pRet;
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool PlaneLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		
		Size winSize=Director::getInstance()->getWinSize();

		Sprite* plane=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
		plane->setPosition(Point(winSize.width/2,plane->getContentSize().height/2));
		this->addChild(plane,0,AIRPLANE);

		Blink *blink=Blink::create(1,3);

		Animation* animation=Animation::create();
		animation->setDelayPerUnit(0.1f);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
		Animate* animate=Animate::create(animation);
		
		plane->runAction(blink);
		plane->runAction(RepeatForever::create(animate));
		
		//Sequence* sequence=Sequence::create(blink,RepeatForever::create(animate),NULL);
		//plane->runAction(sequence);

		bRet=true;
	} while (0);
	
	return bRet;
}

void PlaneLayer::MoveTo(Point location)
{

	if(_isAlive && !Director::getInstance()->isPaused())
	{
		Point actualPoint;
		Size winSize=Director::getInstance()->getWinSize();
		Size planeSize=this->getChildByTag(AIRPLANE)->getContentSize();
		if (location.x<planeSize.width/2)
		{
			location.x=planeSize.width/2;
		}
		if (location.x>winSize.width-planeSize.width/2)
		{
			location.x=winSize.width-planeSize.width/2;
		}
		if (location.y<planeSize.height/2)
		{
			location.y=planeSize.width/2+10;
		}
		if (location.y>winSize.height-planeSize.height/2)
		{
			location.y=winSize.height-planeSize.height/2;
		}
		this->getChildByTag(AIRPLANE)->setPosition(location);
	}
}

void PlaneLayer::Blowup(int passScore)
{
	if(_isAlive)
	{
		_isAlive=false;
		_score=passScore;
		Animation* animation=Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n1.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n2.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n3.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n4.png"));

		Animate* animate=Animate::create(animation);
		CallFunc* removePlane=CallFunc::create(CC_CALLBACK_0(PlaneLayer::RemovePlane,this));
		Sequence* sequence=Sequence::create(animate,removePlane,NULL);
		this->getChildByTag(AIRPLANE)->stopAllActions();
		this->getChildByTag(AIRPLANE)->runAction(sequence);
	}
}

void PlaneLayer::RemovePlane()
{
	this->removeChildByTag(AIRPLANE,true);
	GameOverScene* pScene=GameOverScene::create(_score);
	TransitionMoveInT* animateScene=TransitionMoveInT::create(0.8f,pScene);
	Director::getInstance()->replaceScene(animateScene);
}
