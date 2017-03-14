#include "Pane.h"
#include <iostream>

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * SubPane class
 */
/**
 * public
 */
SubPane* SubPane::create(int index,size_t capacity)
{
	auto ret = new SubPane();
	if(ret->init(index,capacity))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

SubPane::SubPane()
:mOwnSprites(false)
,mIndex(0)
,mSize(0)
{}

bool SubPane::ownSprites()
{
	return mOwnSprites;
}

void SubPane::ownSprites(bool has)
{
	mOwnSprites = has;
}

/**
 * private
 */
bool SubPane::init(int index,size_t capacity)
{
	if(index < 0)
		return false;

	mIndex = index;
	mSize = capacity;

	if(0 < capacity)
	{
		mSprites.reserve(capacity);
		mTiles.reserve(capacity);

		//fill by -1
		mTiles = std::move(std::vector<int>(capacity,-1));
	}

	return true;
}

/**
 * public
 */
Pane* Pane::create(size_t width, size_t height, int index, int numSubPane,Split split)
{
	auto ret = new Pane();
	if(ret->initWithSize(width,height,index,numSubPane,split))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Pane::insertTypeAt(int type,int x,int y)
{
	assert(0 <= x && 0 <= y);

	this->insertTypeAt(type,y * mWidth + x);
}

void Pane::insertTypeAt(int type,int index)
{
	assert(0 <= index);

	mSubPanes.at(index / mSubPaneSize)->getTiles()[index % mSubPaneSize] = type;
}

int Pane::getTypeAt(int x,int y)
{
	assert(0 <= x && 0 <= y);

	return this->getTypeAt(y * mWidth + x);
}

int Pane::getTypeAt(int index)
{
	assert(0 <= index);

	return mSubPanes.at(index / mSubPaneSize)->getTiles()[index % mSubPaneSize];
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

SubPane* Pane::getSubPaneAt(size_t index)
{
	return mSubPanes.at(index);
}

void Pane::passSpriteOwnership(size_t from,size_t to)
{
	assert(from < mSubPanes.size());
	assert(to < mSubPanes.size());

	auto old_owner = mSubPanes.at(from);
	auto new_owner = mSubPanes.at(to);

	assert(old_owner->ownSprites());
	assert(!new_owner->ownSprites());

	new_owner->getSprites() = std::move(old_owner->getSprites());
	new_owner->ownSprites(true);
	old_owner->ownSprites(false);
}

/**
 * protected
 */
Pane::Pane()
:mIndex(0)
,mWidth(0)
,mHeight(0)
,mSubWidth(0)
,mSubHeight(0)
,mSubPaneSize(0)
,mIsModified(false)
,mState(State::ZOMBIE)
{}

Pane::~Pane()
{
}

bool Pane::initWithSize(size_t width,size_t height,int index,int numSubPane,Split split)
{
	assert(0 <= index);
	assert(0 < numSubPane);

	mIndex = index;
	mWidth = width;
	mHeight = height;

	//for sub-pane
	mSubWidth = (split == Split::HORIZONTAL_SPLIT) ? mWidth : mWidth / numSubPane;
	mSubHeight = (split == Split::HORIZONTAL_SPLIT) ? mHeight / numSubPane : mWidth;
	mSubPaneSize = mSubWidth * mSubHeight;
	mSubPanes.reserve(numSubPane);
	for(int i = 0; i < numSubPane; ++i)
		mSubPanes.pushBack(SubPane::create(i,mSubWidth * mSubHeight));

	return true;
}
