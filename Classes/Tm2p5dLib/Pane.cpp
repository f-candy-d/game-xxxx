#include "Pane.h"
#include <iostream>

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * struct SubPane
 */
Pane::SubPane::SubPane(int idx, size_t capa)
{
	assert(0 <= idx);

	this->hasSprites = false;
	this->index = idx;
	this->size = capa;

	if(0 < this->size)
	{
		this->sprites.reserve(this->size);
		this->tiles.reserve(this->size);

		//fill by -1
		this->tiles = std::move(std::vector<int>(this->size,-1));
	}
}

Pane::SubPane::SubPane(const Pane::SubPane& other)
{
	this->hasSprites = other.hasSprites;
	this->index = other.index;
	this->size = other.size;

	if(other.size)
	{
		this->sprites.reserve(other.size);
		this->tiles.reserve(other.size);
		this->tiles = other.tiles;

		if(other.hasSprites)
		{
			//copy elements
			this->sprites = other.sprites;
		}
	}
}

Pane::SubPane& Pane::SubPane::operator=(const Pane::SubPane& other) &
{
	this->hasSprites = other.hasSprites;
	this->index = other.index;
	this->size = other.size;

	if(other.size)
	{
		this->sprites.reserve(other.size);
		this->tiles.reserve(other.size);
		this->tiles = other.tiles;

		if(other.hasSprites)
		{
			//copy elements
			this->sprites = other.sprites;
		}
	}

	return (*this);
}

size_t Pane::SubPane::getSize()
{
	return this->size;
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

void Pane::insertType(int type,int x,int y)
{
	assert(0 <= x && 0 <= y);
	// mTiles[y * mWidth + x] = type;
	this->insertType(type,y * mWidth + x);
}

void Pane::insertType(int type,int index)
{
	assert(0 <= index);

	mSubPanes[index / mSubPaneSize].tiles[index % mSubPaneSize] = type;
}

int Pane::getTypeAt(int x,int y)
{
	assert(0 <= x && 0 <= y);
	// return mTiles[y * mWidth + x];
	return this->getTypeAt(y * mWidth + x);
}

int Pane::getTypeAt(int index)
{
	assert(0 <= index);
	// return mTiles[y * mWidth + x];
	return mSubPanes[index / mSubPaneSize].tiles[index % mSubPaneSize];
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
,mSubWidth(0)
,mSubHeight(0)
,mSubPaneSize(0)
,mIsModified(false)
,mState(State::ZOMBIE)
,mTiles(nullptr)
{}

Pane::~Pane()
{
	if(mTiles)
		delete [] mTiles;
}

bool Pane::initWithSize(size_t width,size_t height,int index,int numSubPane,Split split)
{
	assert(0 <= index);
	assert(0 < numSubPane);

	mIndex = index;
	mWidth = width;
	mHeight = height;

	mTiles = new int[width * height];

	//for sub-pane
	mSubWidth = (split == Split::HORIZONTAL_SPLIT) ? mWidth : mWidth / numSubPane;
	mSubHeight = (split == Split::HORIZONTAL_SPLIT) ? mHeight / numSubPane : mWidth;
	mSubPaneSize = mSubWidth * mSubHeight;
	mSubPanes.reserve(numSubPane);
	for(int i = 0; i < numSubPane; ++i)
		mSubPanes.push_back(SubPane(i,mSubWidth * mSubHeight));

	return true;
}
