#include "Pane.h"
#include "InfoClasses.h"

/**
 * TM2DwD::TM2DwDUnit::Pane class
 */
using namespace TM2DwD::TM2DwDUnit;

/**
 * public
 */
Pane* Pane::create(DLib::size<size_t> pSize, const AtlasInfo* pAtlasInfo)
{
	auto ret = new Pane(pSize, pAtlasInfo->tile_type_no_tile);
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

Pane::Pane(DLib::size<size_t> pSize, int pTtnt)
:TILE_TYPE_NO_TILE(pTtnt)
,mGridSize(pSize.width, pSize.height)
,mGridPoint(0, 0)
,mTiles(pSize.area(), TILE_TYPE_NO_TILE)
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
