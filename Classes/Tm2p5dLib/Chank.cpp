#include "Chank.h"

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
Chank* Chank::create(size_t width, size_t height, int index,Size tileSize)
{
	auto ret = new Chank();
	if(ret->initWithParam(width,height,index,tileSize))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

/**
 * protected
 */
Chank::Chank()
{}

Chank::~Chank()
{}

bool Chank::initWithParam(size_t width,size_t height,int index,cocos2d::Size tileSize)
{
	return true;
}
