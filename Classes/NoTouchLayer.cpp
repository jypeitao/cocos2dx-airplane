#include "NoTouchLayer.h"

bool NoTouchLayer::init(){  
	if (!Layer::init() )
	{  
		return false;  
	}  
	setTouchEnabled(true);  
	return true;  
}  

void NoTouchLayer::registerWithTouchDispatcher()
{  
//	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -127 , true);
//	Layer::registerWithTouchDispatcher();
}  

bool NoTouchLayer::ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent)
{     
	return true;  
}  

void NoTouchLayer::ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent)
{  
}  

void NoTouchLayer::ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
{  
}  
