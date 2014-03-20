#include "WelcomeLayer.h"
#include "GameScene.h"
//#include "GameOverLayer.h"

WelcomeLayer::WelcomeLayer(void)
{
}

WelcomeLayer::~WelcomeLayer(void)
{
}

bool WelcomeLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());

        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot_background.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot.plist");

        Size winSize = Director::getInstance()->getWinSize();

        Sprite* background = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("background.png"));
        background->setPosition(Point(winSize.width / 2, winSize.height / 2));
        this->addChild(background);

        Sprite* copyright = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("shoot_copyright.png"));
        copyright->setAnchorPoint(Point(0.5, 0));
        copyright->setPosition(Point(winSize.width / 2, winSize.height / 2));
        this->addChild(copyright);

        Sprite* peter_logo = Sprite::create("ui/peter_logo.png");
        peter_logo->setPosition(Point(winSize.width / 2, winSize.height / 2 - 40 ));
        this->addChild(peter_logo);

        Sprite* loading = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading1.png"));
        loading->setPosition(Point(winSize.width / 2, winSize.height / 2 - 80));
        this->addChild(loading);

        Animation* animation = Animation::create();
        animation->setDelayPerUnit(0.2f);
        animation->addSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading1.png"));
        animation->addSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading2.png"));
        animation->addSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading3.png"));
        animation->addSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading4.png"));

        Animate* animate = Animate::create(animation);
        Repeat* repeat = Repeat::create(animate, 5);
        CallFuncN* repeatdone = CallFuncN::create(CC_CALLBACK_1(WelcomeLayer::loadingDone,this));
        Sequence* sequence = Sequence::create(repeat, repeatdone, NULL);
        loading->runAction(sequence);

        getHighestHistorySorce();

        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = CC_CALLBACK_2(WelcomeLayer::onKeyReleased,this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

        bRet = true;
    } while (0);

    return bRet;
}

void WelcomeLayer::loadingDone(Node* pNode)
{
    Scene* pScene = GameScene::create();
    TransitionMoveInB *animateScene = TransitionMoveInB::create(0.5f, pScene);
    Director::getInstance()->replaceScene(animateScene);
}

void WelcomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        Director::getInstance()->end();
    }
}

bool WelcomeLayer::isHaveSaveFile()
{
    if (!UserDefault::getInstance()->getBoolForKey("isHaveSaveFileXml"))
    {
        UserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml", true);
        UserDefault::getInstance()->setIntegerForKey("HighestScore", 0);
        UserDefault::getInstance()->flush();
        return false;
    }
    else
    {
        return true;
    }
}

void WelcomeLayer::getHighestHistorySorce()
{
    if (isHaveSaveFile())
    {
//        GameOverLayer::highestHistoryScore = UserDefault::getInstance()->getIntegerForKey(
//                "HighestScore", 0);
    }
}
