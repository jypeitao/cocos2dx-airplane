#include "GameOverLayer.h"
#include "GameScene.h"

int GameOverLayer::s_highestHistoryScore = 0;

GameOverLayer::GameOverLayer(void)
{
    _score = 0;
    _highestScore = NULL;
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore)
{
    GameOverLayer *pRet = new GameOverLayer();
    pRet->_score = passScore;
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
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());

        if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/game_over.mp3");

        Size winSize = Director::getInstance()->getWinSize();

        Sprite* background = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("gameover.png"));
        background->setPosition(Point(winSize.width / 2, winSize.height / 2));
        this->addChild(background);

        Sprite* normalBackToGame = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_finish.png"));
        Sprite* pressedBackToGame = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_finish.png"));
        MenuItemImage* pBackItem = MenuItemImage::create();
        pBackItem->setNormalImage(normalBackToGame);
        pBackItem->setSelectedImage(pressedBackToGame);
        pBackItem->setCallback(CC_CALLBACK_1(GameOverLayer::menuBackCallback,this));
//        pBackItem->initWithNormalSprite(normalBackToGame, pressedBackToGame, NULL, this,
//                menu_selector(GameOverLayer::menuBackCallback));
        pBackItem->setPosition(
                Point(winSize.width - normalBackToGame->getContentSize().width / 2 - 10,
                        normalBackToGame->getContentSize().height / 2 + 10));
        Menu *menuBack = Menu::create(pBackItem, NULL);
        menuBack->setPosition(Point::ZERO);
        this->addChild(menuBack);

        __String* strScore=__String::createWithFormat("%d",_score);
        LabelBMFont* finalScore = LabelBMFont::create(strScore->_string.c_str(), "font/font.fnt");
        finalScore->setColor(Color3B(143, 146, 147));
        finalScore->setPosition(Point(winSize.width / 2, winSize.height / 2));
        this->addChild(finalScore);

        DelayTime* delay = DelayTime::create(1.0f);
        ScaleTo* scalebig = ScaleTo::create(1.0f, 3.0f);
        ScaleTo* scalelittle = ScaleTo::create(0.3f, 2.0f);
        CallFunc* showAD = CallFunc::create(CC_CALLBACK_0(GameOverLayer::showAD,this));
        FiniteTimeAction* sequence = Sequence::create(delay, scalebig, scalelittle, showAD, NULL);
        finalScore->runAction(sequence);

        //LabelTTF* tip=LabelTTF::create("Congratulations! Your Score is","Monaco",20);
        //tip->setColor(Color3B(143,146,147));
        //tip->setPosition(Point(finalScore->getPosition().x,finalScore->getPosition().y+finalScore->getContentSize().height+50));
        //this->addChild(tip);
        __String* strHighestScore=__String::createWithFormat("%d",s_highestHistoryScore);
        _highestScore = LabelBMFont::create(strHighestScore->_string.c_str(), "font/font.fnt");
        _highestScore->setColor(Color3B(143, 146, 147));
        _highestScore->setAnchorPoint(Point(0, 0.5));
        _highestScore->setPosition(Point(140, winSize.height - 30));
        this->addChild(_highestScore);

        if (_score > s_highestHistoryScore)
        {
            UserDefault::getInstance()->setIntegerForKey("HighestScore", _score);
            s_highestHistoryScore = _score;
            DelayTime* delayChange = DelayTime::create(1.3f);
            MoveBy* moveOut = MoveBy::create(0.1f, Point(0, 100));
            CallFuncN* beginChange = CallFuncN::create(CC_CALLBACK_1(GameOverLayer::beginChangeHighestScore,this));
            MoveBy* moveIn = MoveBy::create(0.1f, Point(0, -100));
            FiniteTimeAction* sequence = Sequence::create(delayChange, moveOut, beginChange, moveIn,
                    NULL);
            _highestScore->runAction(sequence);
        }

//		this->setKeypadEnabled(true);
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = CC_CALLBACK_2(GameOverLayer::onKeyReleased,this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

        bRet = true;
    } while (0);
    return bRet;
}

void GameOverLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        Director::getInstance()->end();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_MENU)
    {
        //_label->setString("MENU clicked!");
    }

}

void GameOverLayer::menuBackCallback(Object* pSender)
{
    Scene* pScene = GameScene::create();
    TransitionSlideInL* animateScene = TransitionSlideInL::create(1.0f, pScene);
    Director::getInstance()->replaceScene(animateScene);
}

void GameOverLayer::beginChangeHighestScore(Node* pNode)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/achievement.mp3");
    __String* changeScore=__String::createWithFormat("%d",_score);
    _highestScore->setString(changeScore->_string.c_str());
}

void GameOverLayer::showAD()
{

}
