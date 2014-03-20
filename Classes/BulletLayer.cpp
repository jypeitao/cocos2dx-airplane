#include "BulletLayer.h"
#include "PlaneLayer.h"

BulletLayer::BulletLayer(void)
{
	//bulletSpriteFrame=NULL;
	_bulletBatchNode=NULL;

	_allBullet=__Array::create();
	_allBullet->retain();
}

BulletLayer::~BulletLayer(void)
{
	_allBullet->release();
	_allBullet=NULL;
}

bool BulletLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		//bulletSpriteFrame=SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet1.png");


		Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey("ui/shoot.png");
		_bulletBatchNode = SpriteBatchNode::createWithTexture(texture);
		this->addChild(_bulletBatchNode);

		bRet=true;
	} while (0);
	return bRet;
}

void BulletLayer::StartShoot(float delay)
{
	this->schedule(schedule_selector(BulletLayer::AddBullet),0.20f,kRepeatForever,delay);
}

void BulletLayer::StopShoot()
{
	this->unschedule(schedule_selector(BulletLayer::AddBullet));
}

void BulletLayer::AddBullet(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bullet.mp3");
	Sprite* bullet=Sprite::createWithSpriteFrameName("bullet1.png");
	_bulletBatchNode->addChild(bullet);
	//this->addChild(bullet);
	this->_allBullet->addObject(bullet);

	Point planePosition=PlaneLayer::s_sharedPlane->getChildByTag(AIRPLANE)->getPosition();
	Point bulletPosition=Point(planePosition.x,planePosition.y+PlaneLayer::s_sharedPlane->getChildByTag(AIRPLANE)->getContentSize().height/2);
	bullet->setPosition(bulletPosition);
    
	float length=Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2-bulletPosition.y;
	float velocity=320/1;//320pixel/sec
	float realMoveDuration=length/velocity;

	FiniteTimeAction* actionMove=MoveTo::create(realMoveDuration,Point(bulletPosition.x,Director::getInstance()->getWinSize().height+bullet->getContentSize().height/2));
	FiniteTimeAction* actionDone=CallFuncN::create(CC_CALLBACK_1(BulletLayer::bulletMoveFinished,this));

	Sequence* sequence=Sequence::create(actionMove,actionDone,NULL);
	bullet->runAction(sequence);
}

void BulletLayer::bulletMoveFinished(Node* pSender)
{
	Sprite* bullet=(Sprite*)pSender;
	this->_bulletBatchNode->removeChild(bullet,true);
	this->_allBullet->removeObject(bullet);
}

void BulletLayer::RemoveBullet(Sprite* bullet)
{
	if (bullet!=NULL)
	{
		this->_bulletBatchNode->removeChild(bullet,true);
		this->_allBullet->removeObject(bullet);
	}
}

