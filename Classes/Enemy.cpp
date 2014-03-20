#include "Enemy.h"

Enemy::Enemy(void)
{
	_sprite=NULL;
	_life=0;
}

Enemy::~Enemy(void)
{
}

Enemy* Enemy::create()
{
	Enemy* pRet=new Enemy;
	pRet->autorelease();
	return pRet;
}

void Enemy::bindSprite(Sprite* sprite,int life)
{
	_sprite=sprite;
	_life=life;
	this->addChild(_sprite);
}

Sprite* Enemy::getSprite()
{
	return _sprite;
}

int Enemy::getLife()
{
	return _life;
}

void Enemy::loseLife()
{
	_life--;
}

Rect Enemy::getBoundingBox()
{
	Rect rect=_sprite->getBoundingBox();
	Point pos=this->convertToWorldSpace(rect.origin);
	Rect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);
	return enemyRect;
}
