#include "Pane.h"

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
Pane* Pane::create(size_t width, size_t height, int index)
{
	auto ret = new Pane();
	if(ret->initWithSize(width,height,index))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Pane::insertType(int type,unsigned int x,unsigned int y)
{
	mTiles[y * mWidth + x] = type;
}

int Pane::getTypeAt(unsigned int x,unsigned int y)
{
	return mTiles[y * mWidth + x];
}

bool Pane::recycle(int index)
{
	if(index < 0)
		return false;

	mIndex = index;
	mIsModified = false;
	mState = State::ZOMBIE;

	return true;
}

void Pane::addSprite(Sprite* sprite)
{
	mSprites.pushBack(sprite);
}

/**
 * protected
 */
Pane::Pane()
:mIndex(0)
,mWidth(0)
,mHeight(0)
,mIsModified(false)
,mState(State::ZOMBIE)
,mTiles(nullptr)
{}

Pane::~Pane()
{
	if(mTiles)
		delete [] mTiles;
}

bool Pane::initWithSize(size_t width,size_t height,int index)
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
