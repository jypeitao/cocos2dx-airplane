#include "WelcomeLayer.h"
#include "GameScene.h"
#include "GameOverLayer.h"

WelcomeLayer::WelcomeLayer(void) {
}

WelcomeLayer::~WelcomeLayer(void) {
}

bool WelcomeLayer::init() {
	bool bRet = false;
	do {
		CC_BREAK_IF(!Layer::init());


		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
				"ui/shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
				"ui/shoot.plist");

		Size winSize = Director::getInstance()->getWinSize();

		//����background
		Sprite* background = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"background.png"));
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);

		//����copyright
		Sprite* copyright = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"shoot_copyright.png"));
		copyright->setAnchorPoint(ccp(0.5,0));
		copyright->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(copyright);

		//����loading
		Sprite* loading = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"game_loading1.png"));
		loading->setPosition(ccp(winSize.width/2,winSize.height/2-40));
		this->addChild(loading);

		Animation* animation = Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"game_loading1.png"));
		animation->addSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"game_loading2.png"));
		animation->addSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"game_loading3.png"));
		animation->addSpriteFrame(
				SpriteFrameCache::getInstance()->spriteFrameByName(
						"game_loading4.png"));

		Animate* animate = Animate::create(animation);
		Repeat* repeat = Repeat::create(animate, 10000);
		CallFuncN* repeatdone = CallFuncN::create(this,
				callfuncN_selector(WelcomeLayer::loadingDone));
		Sequence* sequence = Sequence::create(repeat, repeatdone, NULL);
		loading->runAction(sequence);

		getHighestHistorySorce();

		this->setKeypadEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void WelcomeLayer::loadingDone(Node* pNode) {
	Scene* pScene = GameScene::create();
	TransitionMoveInB *animateScene = TransitionMoveInB::create(0.5f,
			pScene);
	Director::getInstance()->replaceScene(animateScene);
}

void WelcomeLayer::keyBackClicked() {
	Director::sharedDirector()->end();
}

bool WelcomeLayer::isHaveSaveFile() {
	if (!UserDefault::getInstance()->getBoolForKey(
			"isHaveSaveFileXml")) {
		UserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml",
				true);
		UserDefault::getInstance()->setIntegerForKey("HighestScore", 0);
		UserDefault::getInstance()->flush();
		return false;
	} else {
		return true;
	}
}

void WelcomeLayer::getHighestHistorySorce() {
	if (isHaveSaveFile()) {
		GameOverLayer::highestHistoryScore =
				UserDefault::getInstance()->getIntegerForKey(
						"HighestScore", 0);
	}
}
