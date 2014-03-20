#include "UFOLayer.h"

UFOLayer::UFOLayer(void)
{
	_allMutiBullets=__Array::create();
	_allMutiBullets->retain();
	_allBigBoom=__Array::create();
	_allBigBoom->retain();
}

UFOLayer::~UFOLayer(void)
{
	_allMutiBullets->release();
	_allMutiBullets=NULL;
	_allBigBoom->release();
	_allBigBoom=NULL;
}

bool UFOLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		this->schedule(schedule_selector(UFOLayer::AddMutiBullets),20.0);
		this->schedule(schedule_selector(UFOLayer::AddBigBoom),20.0,kRepeatForever,5.0);
		bRet=true;
	} while (0);

	return bRet;
}

void UFOLayer::AddMutiBullets(float dt)
{
    log("AddMutiBullets %f",dt);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/out_porp.mp3");
	Sprite* mutiBullets=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ufo1.png"));

	Size mutiBlletsSize=mutiBullets->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=mutiBlletsSize.width/2;
	int maxX=winSize.width-mutiBlletsSize.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	mutiBullets->setPosition(Point(actualX,winSize.height+mutiBlletsSize.height/2));
	this->addChild(mutiBullets);
	this->_allMutiBullets->addObject(mutiBullets);

	MoveBy* move1 = MoveBy::create(0.5f, Point(0, -150));
	MoveBy* move2 = MoveBy::create(0.3f, Point(0, 100));
	MoveBy* move3 = MoveBy::create(1.0f, Point(0,0-winSize.height-mutiBlletsSize.height/2));
	CallFuncN* moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::mutiBulletsMoveFinished,this));


	FiniteTimeAction* sequence = Sequence::create(move1,move2,move3,moveDone,NULL);
	mutiBullets->runAction(sequence);

}

void UFOLayer::mutiBulletsMoveFinished(Node* pSender)
{

	Sprite* mutiBullets=(Sprite*)pSender;
	this->removeChild(mutiBullets,true);
	this->_allMutiBullets->removeObject(mutiBullets);
}

void UFOLayer::AddBigBoom(float dt)
{
    log("AddBigBoom %f",dt);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/out_porp.mp3");
	Sprite* bigBoom=Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ufo2.png"));

	Size bigBoomSize=bigBoom->getContentSize();
	Size winSize=Director::getInstance()->getWinSize();
	int minX=bigBoomSize.width/2;
	int maxX=winSize.width-bigBoomSize.width/2;
	int rangeX=maxX-minX;
	int actualX=(rand()%rangeX)+minX;

	bigBoom->setPosition(Point(actualX,winSize.height+bigBoomSize.height/2));
	this->addChild(bigBoom);
	this->_allBigBoom->addObject(bigBoom);

	MoveBy* move1 = MoveBy::create(0.5, Point(0, -150));
	MoveBy* move2 = MoveBy::create(0.3, Point(0, 100));
	MoveBy* move3 = MoveBy::create(1.0, Point(0,0-winSize.height-bigBoomSize.height/2));
	CallFuncN* moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::bigBoomMoveFinished,this));

	FiniteTimeAction* sequence = Sequence::create(move1,move2,move3,moveDone,NULL);
	bigBoom->runAction(sequence);
}

void UFOLayer::bigBoomMoveFinished(Node* pSender)
{
	Sprite* bigBoom=(Sprite*)pSender;
	this->removeChild(bigBoom,true);
	this->_allBigBoom->removeObject(bigBoom);
}

void UFOLayer::RemoveMutiBullets(Sprite* mutiBullets)
{
	this->removeChild(mutiBullets,true);
	this->_allMutiBullets->removeObject(mutiBullets);
}

void UFOLayer::RemoveBigBoom(Sprite* bigBoom)
{
	this->removeChild(bigBoom,true);
	this->_allBigBoom->removeObject(bigBoom);
}
