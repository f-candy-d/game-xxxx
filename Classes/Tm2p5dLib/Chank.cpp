#include "Chank.h"

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
Chank* Chank::create(size_t width, size_t height, int index)
{
	auto ret = new Chank();
	if(ret->initWithSize(width,height,index))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Chank::insertType(int type,unsigned int x,unsigned int y)
{
	mTiles[y * mWidth + x] = type;
}

int Chank::getTypeAt(unsigned int x,unsigned int y)
{
	return mTiles[y * mWidth + x];
}

bool Chank::recycle(int index)
{
	if(index < 0)
		return false;

	mIndex = index;
	mIsModified = false;

	return true;
}

void Chank::addSprite(Sprite* sprite)
{
	mSprites.pushBack(sprite);
}

/**
 * protected
 */
Chank::Chank()
:mIndex(0)
,mWidth(0)
,mHeight(0)
,mIsModified(false)
,mTiles(nullptr)
{}

Chank::~Chank()
{
	if(mTiles)
		delete [] mTiles;
}

bool Chank::initWithSize(size_t width,size_t height,int index)
{
	//check a parameter
	if(index < 0)
		return false;

	mIndex = index;
	mWidth = width;
	mHeight = height;

	mTiles = new int[width * height];

	return true;
}
