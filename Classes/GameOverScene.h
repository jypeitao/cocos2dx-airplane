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
#include "GameOverLayer.h"

USING_NS_CC;

class GameOverScene :
	public Scene
{
public:

	GameOverScene(void);
	
	~GameOverScene(void);

	virtual bool init();

	static GameOverScene* create(int passScore);

public:

	GameOverLayer* _gameOverLayer;

	int _score;
};
