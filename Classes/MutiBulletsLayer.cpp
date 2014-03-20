#include "MutiBulletsLayer.h"
#include "PlaneLayer.h"

MutiBulletsLayer::MutiBulletsLayer(void)
{
	//mutiBulletsSpriteFrame=NULL;
	_mutiBullesBatchNode=NULL;

	_allMutiBullets=__Array::create();
	_allMutiBullets->retain();
}

MutiBulletsLayer::~MutiBulletsLayer(void)
{
	_allMutiBullets->release();
	_allMutiBullets=NULL;
}

bool MutiBulletsLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		//mutiBulletsSpriteFrame=SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet2.png");

		Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey("ui/shoot.png");
		_mutiBullesBatchNode = SpriteBatchNode::createWithTexture(texture);
		this->addChild(_mutiBullesBatchNode);

		bRet=true;
	} while (0);
	return bRet;
}

void MutiBulletsLayer::StartShoot()
{
	this->schedule(schedule_selector(MutiBulletsLayer::AddMutiBullets),0.2f,30,0.0f);
}

void MutiBulletsLayer::StopShoot()
{
	this->unschedule(schedule_selector(MutiBulletsLayer::AddMutiBullets));
}

void MutiBulletsLayer::AddMutiBullets(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bullet.mp3");

	Sprite* bulletLeft=Sprite::createWithSpriteFrameName("bullet2.png");
	Sprite* bulletRight=Sprite::createWithSpriteFrameName("bullet2.png");
	_mutiBullesBatchNode->addChild(bulletLeft);
	_mutiBullesBatchNode->addChild(bulletRight);
	this->_allMutiBullets->addObject(bulletLeft);
	this->_allMutiBullets->addObject(bulletRight);

	Point planePosition=PlaneLayer::s_sharedPlane->getChildByTag(AIRPLANE)->getPosition();
	Point bulletLeftPosition=Point(planePosition.x-33,planePosition.y+35);
	Point bulletRightPosition=Point(planePosition.x+33,planePosition.y+35);
	bulletLeft->setPosition(bulletLeftPosition);
	bulletRight->setPosition(bulletRightPosition);

	float length=Director::getInstance()->getWinSize().height+bulletLeft->getContentSize().height/2-bulletLeftPosition.y;
	float velocity=420/1;//420pixel/sec
	float realMoveDuration=length/velocity;

	FiniteTimeAction* actionLeftMove=MoveTo::create(realMoveDuration,Point(bulletLeftPosition.x,Director::getInstance()->getWinSize().height+bulletLeft->getContentSize().height/2));
	FiniteTimeAction* actionLeftDone=CallFuncN::create(CC_CALLBACK_1(MutiBulletsLayer::mutiBulletsMoveFinished,this));

	Sequence* sequenceLeft=Sequence::create(actionLeftMove,actionLeftDone,NULL);
	
	FiniteTimeAction* actionRightMove=MoveTo::create(realMoveDuration,Point(bulletRightPosition.x,Director::getInstance()->getWinSize().height+bulletRight->getContentSize().height/2));
	FiniteTimeAction* actionRightDone=CallFuncN::create(CC_CALLBACK_1(MutiBulletsLayer::mutiBulletsMoveFinished,this));

	Sequence* sequenceRight=Sequence::create(actionRightMove,actionRightDone,NULL);

	bulletLeft->runAction(sequenceLeft);
	bulletRight->runAction(sequenceRight);
}

void MutiBulletsLayer::mutiBulletsMoveFinished(Node* pSender)
{
	Sprite* mutiBullets=(Sprite*)pSender;
	_allMutiBullets->removeObject(mutiBullets);
	this->_mutiBullesBatchNode->removeChild(mutiBullets,true);
	log("MutiBUlletsCount=%ld",_allMutiBullets->count());
}

void MutiBulletsLayer::RemoveMutiBullets(Sprite* mutiBullets)
{
	if (mutiBullets!=NULL)
	{
		this->_allMutiBullets->removeObject(mutiBullets);
		this->_mutiBullesBatchNode->removeChild(mutiBullets,true);
	}
}
