#include "GameLayer.h"
//#include "Enemy.h"

Level GameLayer::s_level = EASY;

GameLayer::GameLayer(void) :

                _background1(NULL),
                _background2(NULL),
                _planeLayer(NULL),
                _bulletLayer(NULL),
                _mutiBulletsLayer(NULL),
                _controlLayer(NULL),
                _ufoLayer(NULL),
                _enemyLayer(NULL),
                _menuBigBoom(NULL),
                _bigBoomCountItem(NULL),
                _score(0),
                _bigBoomCount(0)

{

}

GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());

        s_level = EASY;

        if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
                    "sound/game_music.mp3", true);
        }

        _background1 = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("background.png"));
        _background1->setAnchorPoint(Point(0, 0));
        _background1->setPosition(Point(0, 0));
        this->addChild(_background1);

        _background2 = Sprite::createWithSpriteFrame(
                SpriteFrameCache::getInstance()->getSpriteFrameByName("background.png"));
        _background2->setAnchorPoint(Point(0, 0));
        _background2->setPosition(Point(0, _background1->getPositionY()+_background1->getContentSize().height -4));
        this->addChild(_background2);

        this->_planeLayer = PlaneLayer::create();
        this->addChild(_planeLayer);

        this->_bulletLayer = BulletLayer::create();
        this->addChild(_bulletLayer);
        this->_bulletLayer->StartShoot();

        this->_mutiBulletsLayer = MutiBulletsLayer::create();
        this->addChild(_mutiBulletsLayer);
//
        this->_enemyLayer = EnemyLayer::create();
        this->addChild(_enemyLayer);

        this->_controlLayer = ControlLayer::create();
        this->addChild(_controlLayer);
//
        this->_ufoLayer = UFOLayer::create();
        this->addChild(_ufoLayer);

        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);

        touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);

        getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

        this->scheduleUpdate();

        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased,this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

        bRet = true;
    } while (0);
    return bRet;
}

void GameLayer::backgroundMove(float dt)
{
    _background1->setPositionY(_background1->getPositionY() - 2);
    _background2->setPositionY(
            _background1->getPositionY() + _background1->getContentSize().height -4);
    if (_background2->getPositionY() < 0.001)
    {
        _background1->setPositionY(0);
    }
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
    log("onTouchBegan");
//    if (!getBoundingBox().containsPoint(convertTouchToNodeSpace(touch)))
//    {
//        return false;
//    }
    return true;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
    log("onTouchMoved");
    if (this->_planeLayer->_isAlive)
    {
        Point beginPoint = touch->getLocationInView();
        beginPoint = Director::getInstance()->convertToGL(beginPoint);
        //juggle the area of drag
        Rect planeRect = this->_planeLayer->getChildByTag(AIRPLANE)->getBoundingBox();
        planeRect.origin.x -= 30;
        planeRect.origin.y -= 30;
        planeRect.size.width += 60;
        planeRect.size.height += 60;
        if (planeRect.containsPoint(this->getParent()->convertTouchToNodeSpace(touch)))
        {
            Point endPoint = touch->getPreviousLocationInView();
            endPoint = Director::getInstance()->convertToGL(endPoint);

            Point offSet = beginPoint - endPoint;
            Point toPoint = this->_planeLayer->getChildByTag(AIRPLANE)->getPosition() + offSet;
            this->_planeLayer->MoveTo(toPoint);
        }
    }
}

void GameLayer::onTouchEnded(Touch* touch, Event* event)
{

}

void GameLayer::update(float dt)
{
    if (s_level == EASY && _score >= 1000000)
    {
        s_level = MIDDLE;
    }
    else if (s_level == MIDDLE && _score >= 2000000)
    {
        s_level = HARD;
    }

    __Array* bulletsToDelete = __Array::create();
    bulletsToDelete->retain();
    Object* bt, *et, *ut;

    //enemy1 & bullet CheckCollosion
    CCARRAY_FOREACH(this->_bulletLayer->_allBullet,bt)
    {
        Sprite* bullet = (Sprite*) bt;

        __Array* enemy1sToDelete = __Array::create();
        enemy1sToDelete->retain();
        //int a = this->_enemyLayer->_pAllEnemy1->capacity();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy1,et)
        {
            Enemy* enemy1 = (Enemy*) et;
            if (bullet->getBoundingBox().intersectsRect(enemy1->getBoundingBox()))
            {
                if (enemy1->getLife() == 1)
                {
                    enemy1->loseLife();
                    bulletsToDelete->addObject(bullet);
                    enemy1sToDelete->addObject(enemy1);
                    _score += ENEMY1_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }
            }
        }
        CCARRAY_FOREACH(enemy1sToDelete,et)
        {
            Enemy* enemy1 = (Enemy*) et;
            this->_enemyLayer->enemy1Blowup(enemy1);
        }
        enemy1sToDelete->release();
    }
    CCARRAY_FOREACH(bulletsToDelete,bt)
    {
        Sprite* bullet = (Sprite*) bt;
        this->_bulletLayer->RemoveBullet(bullet);
    }
    bulletsToDelete->removeAllObjects();

    //enemy2 & bullet CheckCollosion
    CCARRAY_FOREACH(this->_bulletLayer->_allBullet,bt)
    {
        Sprite* bullet = (Sprite*) bt;

        __Array* enemy2sToDelete = __Array::create();
        enemy2sToDelete->retain();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy2,et)
        {
            Enemy* enemy2 = (Enemy*) et;
            if (bullet->getBoundingBox().intersectsRect(enemy2->getBoundingBox()))
            {
                if (enemy2->getLife() > 1)
                {
                    enemy2->loseLife();
                    bulletsToDelete->addObject(bullet);
                }
                else if (enemy2->getLife() == 1)
                {
                    enemy2->loseLife();
                    bulletsToDelete->addObject(bullet);
                    enemy2sToDelete->addObject(enemy2);
                    _score += ENEMY2_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }

            }
        }
        CCARRAY_FOREACH(enemy2sToDelete,et)
        {
            Enemy* enemy2 = (Enemy*) et;
            this->_enemyLayer->enemy2Blowup(enemy2);
        }
        enemy2sToDelete->release();
    }
    CCARRAY_FOREACH(bulletsToDelete,bt)
    {
        Sprite* bullet = (Sprite*) bt;
        this->_bulletLayer->RemoveBullet(bullet);
    }
    bulletsToDelete->removeAllObjects();

    //enemy3 & bullet CheckCollosion
    CCARRAY_FOREACH(this->_bulletLayer->_allBullet,bt)
    {
        Sprite* bullet = (Sprite*) bt;

        __Array* enemy3sToDelete = __Array::create();
        enemy3sToDelete->retain();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy3,et)
        {
            Enemy* enemy3 = (Enemy*) et;
            if (bullet->getBoundingBox().intersectsRect(enemy3->getBoundingBox()))
            {
                if (enemy3->getLife() > 1)
                {
                    enemy3->loseLife();
                    bulletsToDelete->addObject(bullet);
                }
                else if (enemy3->getLife() == 1)
                {
                    enemy3->loseLife();
                    bulletsToDelete->addObject(bullet);
                    enemy3sToDelete->addObject(enemy3);
                    _score += ENEMY3_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }

            }
        }
        CCARRAY_FOREACH(enemy3sToDelete,et)
        {
            Enemy* enemy3 = (Enemy*) et;
            this->_enemyLayer->enemy3Blowup(enemy3);
        }
        enemy3sToDelete->release();
    }
    CCARRAY_FOREACH(bulletsToDelete,bt)
    {
        Sprite* bullet = (Sprite*) bt;
        this->_bulletLayer->RemoveBullet(bullet);
    }
    bulletsToDelete->removeAllObjects();
    bulletsToDelete->release();

    __Array* mutiBulletsToDelete = __Array::create();
    mutiBulletsToDelete->retain();
    Object* mbt;

    ////enemy1 & mutiBullets CheckCollosion
    CCARRAY_FOREACH(this->_mutiBulletsLayer->_allMutiBullets,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;

        __Array* enemy1sToDelete = __Array::create();
        enemy1sToDelete->retain();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy1,et)
        {
            Enemy* enemy1 = (Enemy*) et;
            if (mutiBullets->getBoundingBox().intersectsRect(enemy1->getBoundingBox()))
            {
                if (enemy1->getLife() == 1)
                {
                    enemy1->loseLife();
                    mutiBulletsToDelete->addObject(mutiBullets);
                    enemy1sToDelete->addObject(enemy1);
                    _score += ENEMY1_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }
            }
        }
        CCARRAY_FOREACH(enemy1sToDelete,et)
        {
            Enemy* enemy1 = (Enemy*) et;
            this->_enemyLayer->enemy1Blowup(enemy1);
        }
        enemy1sToDelete->release();
    }
    CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;
        this->_mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
    }
    mutiBulletsToDelete->removeAllObjects();

    //enemy2 & mutiBullets CheckCollosion
    CCARRAY_FOREACH(this->_mutiBulletsLayer->_allMutiBullets,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;

        __Array* enemy2sToDelete = __Array::create();
        enemy2sToDelete->retain();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy2,et)
        {
            Enemy* enemy2 = (Enemy*) et;
            if (mutiBullets->getBoundingBox().intersectsRect(enemy2->getBoundingBox()))
            {
                if (enemy2->getLife() > 1)
                {
                    enemy2->loseLife();
                    mutiBulletsToDelete->addObject(mutiBullets);
                }
                else if (enemy2->getLife() == 1)
                {
                    enemy2->loseLife();
                    mutiBulletsToDelete->addObject(mutiBullets);
                    enemy2sToDelete->addObject(enemy2);
                    _score += ENEMY2_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }

            }
        }
        CCARRAY_FOREACH(enemy2sToDelete,et)
        {
            Enemy* enemy2 = (Enemy*) et;
            this->_enemyLayer->enemy2Blowup(enemy2);
        }
        enemy2sToDelete->release();
    }
    CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;
        this->_mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
    }
    mutiBulletsToDelete->removeAllObjects();

    //enemy3 & mutiBullets CheckCollosion
    CCARRAY_FOREACH(this->_mutiBulletsLayer->_allMutiBullets,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;

        __Array* enemy3sToDelete = __Array::create();
        enemy3sToDelete->retain();
        CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy3,et)
        {
            Enemy* enemy3 = (Enemy*) et;
            if (mutiBullets->getBoundingBox().intersectsRect(enemy3->getBoundingBox()))
            {
                 if (enemy3->getLife() > 1)
                {
                    enemy3->loseLife();
                    mutiBulletsToDelete->addObject(mutiBullets);
                }
                else if (enemy3->getLife() == 1)
                {
                    enemy3->loseLife();
                    mutiBulletsToDelete->addObject(mutiBullets);
                    enemy3sToDelete->addObject(enemy3);
                    _score += ENEMY3_SCORE;
                    this->_controlLayer->updateScore(_score);
                }
                else
                {
                    //do nothing
                }
            }
        }
        CCARRAY_FOREACH(enemy3sToDelete,et)
        {
            Enemy* enemy3 = (Enemy*) et;
            this->_enemyLayer->enemy3Blowup(enemy3);
        }
        enemy3sToDelete->release();
    }
    CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
    {
        Sprite* mutiBullets = (Sprite*) mbt;
        this->_mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
    }
    mutiBulletsToDelete->removeAllObjects();
    mutiBulletsToDelete->release();

    Rect airplaneRect = this->_planeLayer->getChildByTag(AIRPLANE)->getBoundingBox();
    airplaneRect.origin.x += 30;
    airplaneRect.size.width -= 60;
    //enemy1 & airplane CheckCollosion
    CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy1,et)
    {
        Enemy* enemy1 = (Enemy*) et;
        if (enemy1->getLife() > 0)
        {
            if (airplaneRect.intersectsRect(enemy1->getBoundingBox()))
            {
                this->unscheduleAllSelectors();
                this->_bulletLayer->StopShoot();
                this->_mutiBulletsLayer->StopShoot();
                this->_planeLayer->Blowup(_score);
                return;
            }
        }
    }

    //enemy2 & airplane CheckCollosion
    CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy2,et)
    {
        Enemy* enemy2 = (Enemy*) et;
        if (enemy2->getLife() > 0)
        {
            if (airplaneRect.intersectsRect(enemy2->getBoundingBox()))
            {
                this->unscheduleAllSelectors();
                this->_bulletLayer->StopShoot();
                this->_mutiBulletsLayer->StopShoot();
                this->_planeLayer->Blowup(_score);
                return;
            }
        }
    }

    //enemy3 & airplane CheckCollosion
    CCARRAY_FOREACH(this->_enemyLayer->_pAllEnemy3,et)
    {
        Enemy* enemy3 = (Enemy*) et;
        if (enemy3->getLife() > 0)
        {
            if (airplaneRect.intersectsRect(enemy3->getBoundingBox()))
            {
                this->unscheduleAllSelectors();
                this->_bulletLayer->StopShoot();
                this->_mutiBulletsLayer->StopShoot();
                this->_planeLayer->Blowup(_score);
                return;
            }
        }
    }

    //mutiBullets & airplane CheckCollision
    CCARRAY_FOREACH(this->_ufoLayer->_allMutiBullets,ut)
    {
        Sprite* mutiBullets = (Sprite*) ut;
        if (this->_planeLayer->getChildByTag(AIRPLANE)->getBoundingBox().intersectsRect(
                mutiBullets->getBoundingBox()))
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    "sound/get_double_laser.mp3");
            this->_ufoLayer->RemoveMutiBullets(mutiBullets);
            this->_bulletLayer->StopShoot();
            this->_mutiBulletsLayer->StartShoot();
            this->_bulletLayer->StartShoot(6.2f);
        }
    }

    //bigBoom & airplane CheckCollision
    CCARRAY_FOREACH(this->_ufoLayer->_allBigBoom,ut)
    {
        Sprite* bigBoom = (Sprite*) ut;
        if (this->_planeLayer->getChildByTag(AIRPLANE)->getBoundingBox().intersectsRect(
                bigBoom->getBoundingBox()))
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/get_bomb.mp3");
            this->_ufoLayer->RemoveBigBoom(bigBoom);
            _bigBoomCount++;
            updateBigBoomItem(_bigBoomCount);
        }
    }
}

void GameLayer::updateBigBoomItem(int bigBoomCount)
{
    Sprite* normalBomb = Sprite::createWithSpriteFrame(
            SpriteFrameCache::getInstance()->getSpriteFrameByName("bomb.png"));
    Sprite* pressedBomb = Sprite::createWithSpriteFrame(
            SpriteFrameCache::getInstance()->getSpriteFrameByName("bomb.png"));
    if (bigBoomCount < 0)
    {
        return;
    }
    else if (bigBoomCount == 0)
    {
        if (this->getChildByTag(TAG_BIGBOOM_MENUITEM))
        {
            this->removeChildByTag(TAG_BIGBOOM_MENUITEM, true);
        }
        if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
        {
            this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
        }
    }
    else if (bigBoomCount == 1)
    {

        if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
        {
            MenuItemImage* pBigBoomItem = MenuItemImage::create();
            pBigBoomItem->setNormalImage(normalBomb);
            pBigBoomItem->setSelectedImage(pressedBomb);
            pBigBoomItem->setCallback(CC_CALLBACK_1(GameLayer::menuBigBoomCallback,this));

            pBigBoomItem->setPosition(
                    Point(normalBomb->getContentSize().width / 2 + 10,
                            normalBomb->getContentSize().height / 2 + 10));
            _menuBigBoom = Menu::create(pBigBoomItem, NULL);
            _menuBigBoom->setPosition(Point::ZERO);
            this->addChild(_menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
        }
        if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
        {
            this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
        }
    }
    else
    {
        if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
        {
            MenuItemImage* pBigBoomItem = MenuItemImage::create();
            pBigBoomItem->setNormalImage(normalBomb);
            pBigBoomItem->setSelectedImage(pressedBomb);
            pBigBoomItem->setCallback(CC_CALLBACK_1(GameLayer::menuBigBoomCallback,this));
            pBigBoomItem->setPosition(
                    Point(normalBomb->getContentSize().width / 2 + 10,
                            normalBomb->getContentSize().height / 2 + 10));
            _menuBigBoom = Menu::create(pBigBoomItem, NULL);
            _menuBigBoom->setPosition(Point::ZERO);
            this->addChild(_menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
        }
        if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
        {
            this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
        }
        if (bigBoomCount >= 0 && bigBoomCount <= MAX_BIGBOOM_COUNT)
        {
            __String* strScore=__String::createWithFormat("X%d",bigBoomCount);
            _bigBoomCountItem=LabelBMFont::create(strScore->_string.c_str(),"font/font.fnt");
            _bigBoomCountItem->setColor(Color3B(143,146,147));
            _bigBoomCountItem->setAnchorPoint(Point(0,0.5));
            _bigBoomCountItem->setPosition(Point(normalBomb->getContentSize().width+15,normalBomb->getContentSize().height/2+5));
            this->addChild(_bigBoomCountItem,0,TAG_BIGBOOMCOUNT_LABEL);
        }
    }
}

void GameLayer::menuBigBoomCallback(Object* pSender)
{
    if (_bigBoomCount > 0 && !Director::getInstance()->isPaused())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/use_bomb.mp3");
        _bigBoomCount--;
        _score += this->_enemyLayer->_pAllEnemy1->count() * ENEMY1_SCORE;
        _score += this->_enemyLayer->_pAllEnemy2->count() * ENEMY2_SCORE;
        _score += this->_enemyLayer->_pAllEnemy3->count() * ENEMY3_SCORE;
        this->_enemyLayer->removeAllEnemy();
        updateBigBoomItem(_bigBoomCount);
        this->_controlLayer->updateScore(_score);
    }
}

Level GameLayer::getCurLevel()
{
    return s_level;
}
