/*******************************************************************************
 Author						: JackyFu
 Email                   	: fusijie@vip.qq.com
 QQ  						: 754505629
 Blog						: http://blog.csdn.net/jackystudio
 All Rights Reserved

 This piece of code does not have any registered copyright and is free to be
 used as necessary. The user is free to modify as per the requirements. As a
 fellow developer, all that I expect and request for is to be given the
 credit for intially developing this reusable code by not removing my name as
 the author.
 *******************************************************************************/
#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "MutiBulletsLayer.h"
#include "ControlLayer.h"
#include "UFOLayer.h"
#include "EnemyLayer.h"

USING_NS_CC;

const int ENEMY1_SCORE = 1000;
const int ENEMY2_SCORE = 6000;
const int ENEMY3_SCORE = 30000;

const int MAX_BIGBOOM_COUNT = 100000;
const int TAG_BIGBOOM_MENUITEM = 1;
const int TAG_BIGBOOMCOUNT_LABEL = 2;

class GameLayer: public Layer
{
public:

    GameLayer(void);

    ~GameLayer(void);

    CREATE_FUNC(GameLayer);

    virtual bool init();

    void backgroundMove(float dt);

    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);


    void update(float dt);

    void menuBigBoomCallback(Object* pSender);

    void updateBigBoomItem(int bigBoomCount);

    static Level getCurLevel();

private:
    Sprite* _background1;
    Sprite* _background2;

    PlaneLayer* _planeLayer;
    BulletLayer* _bulletLayer;
    MutiBulletsLayer* _mutiBulletsLayer;
    ControlLayer* _controlLayer;
    UFOLayer* _ufoLayer;
    EnemyLayer* _enemyLayer;

    Menu* _menuBigBoom;
    LabelBMFont* _bigBoomCountItem;

    int _score;
    int _bigBoomCount;
    static Level s_level;
};
