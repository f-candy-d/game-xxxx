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
	DLib::size<float> vTts(64, 64); // Tile-Texture-Size
	auto vAts = vTts; // Actual-Tile-Size

	auto ret = new Pane(vAts, vTts, pWidth, pHeight);
	if(ret->initWithSize(pWidth, pHeight))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

Pane::Pane(DLib::size<float> pAts, DLib::size<float> pTts, size_t pWidth, size_t pHeight)
:mGridSize(pWidth, pHeight, pAts)
,mTileTextureSize(pTts)
,mActualTileSize(pAts)
,mSpriteScale(1.0)
{}

Pane::Pane()
:mGridSize(DLib::size<float>())
,mTileTextureSize(DLib::size<float>())
,mActualTileSize(DLib::size<float>())
,mSpriteScale(1.0)
{}

void Pane::setSpriteScale(float scale)
{
	assert(0.0 <= scale);

	mSpriteScale = scale;
	mActualTileSize = mTileTextureSize;
	mActualTileSize.scale(mSpriteScale);
	// reset the unit size of mGridSize
	mGridSize.copy(DLib::grid_size(mGridSize.width, mGridSize.height, mActualTileSize));
}

/**
 * protected
 */
bool Pane::initWithSize(size_t pWidth, size_t pHeight)
{
	if(mGridSize.unit.width < 0 || mGridSize.unit.height < 0)
		return false;

	mTiles.reserve(mGridSize.area());

	return true;
}
