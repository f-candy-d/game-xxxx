#include "Pane.h"

/**
 * TM2DwD::TM2DwDUnit::Pane class
 */
using namespace TM2DwD::TM2DwDUnit;

/**
 * public
 */
Pane* Pane::create(size_t pWidth, size_t pHeight)
{
	auto ret = new Pane(pWidth, pHeight, -1);
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

Pane::Pane(size_t pWidth, size_t pHeight, int pTileTypeNoTile)
:TILE_TYPE_NO_TILE(pTileTypeNoTile)
,mGridSize(pWidth, pHeight)
,mGridPoint(0, 0)
,mTiles(pWidth * pHeight, TILE_TYPE_NO_TILE)
,mIsModified(false)
{}

Pane::Pane()
:TILE_TYPE_NO_TILE(-1)
,mGridSize(0, 0)
,mGridPoint(0, 0)
,mIsModified(false)
{}

const int& Pane::getTypeAt(int x, int y)
const
{
	return this->getTypeAt(mGridSize.width * y + x);
}

const int& Pane::getTypeAt(int index)
const
{
	return mTiles[index];
}

void Pane::insertTypeAt(int x, int y, int type)
{
	this->insertTypeAt(mGridSize.width * y + x, type);
}

void Pane::insertTypeAt(int index, int type)
{
	mTiles[index] = type;
}

/**
 * protected
 */
bool Pane::init()
{
	return true;
}
