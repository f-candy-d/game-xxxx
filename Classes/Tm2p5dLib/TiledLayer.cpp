#include "TiledLayer.h"
#include "InfoClasses.h"
#include <iostream>

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
TiledLayer* TiledLayer::create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize)
{
	auto ret = new TiledLayer();
	if(ret->initWithInfo(mapInfo,layerInfo,atlasInfo,capacity,zolder,visibleSize))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void TiledLayer::onVisibleRectChanged(Rect visibleRect)
{

}

/**
 * protected
 */
TiledLayer::TiledLayer()
:mBatchNode(nullptr)
,mZolder(0)
,mCapacity(0)
,mNumOfChank(0)
,mChankWidth(0)
,mChankHeight(0)
,mNumOfTileType(0)
,mNumOfSubChank(1)
,mIndexOfActiveSubChank(0)
,mLayerName("")
,mTerrainSrc("")
,mAtlasSrc("")
,mIsVisible(false)
,mIsEditable(false)
,mOrientation(Orientation::NONE)
{}

TiledLayer::~TiledLayer()
{
	CC_SAFE_RELEASE_NULL(mBatchNode);
}

bool TiledLayer::initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize)
{
	if(!Node::init())
		return false;

	// //check parameters
	if(mapInfo->getOrientation() == Orientation::NONE
	|| atlasInfo->getAtlasSource().empty()
	|| static_cast<size_t>(atlasInfo->getNumOfTileType()) != atlasInfo->getTextureRects().size())
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mIndexOfActiveSubChank = 0;
	mNumOfChank = mapInfo->getNumOfChank();
	mChankWidth = mapInfo->getChankWidth();
	mChankHeight = mapInfo->getChankHeight();
	mOrientation = mapInfo->getOrientation();
	mLayerName = layerInfo->getLayerName();
	mTerrainSrc = layerInfo->getTerrainSource();
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileType = atlasInfo->getNumOfTileType();
	mAtlasSrc = atlasInfo->getAtlasSource();

	mChanks.reserve(capacity);

	//copy texture rects
	mTextureRects.reserve(atlasInfo->getTextureRects().size());
	std::copy(atlasInfo->getTextureRects().begin(),atlasInfo->getTextureRects().end(),std::back_inserter(mTextureRects));


	// Decide the number of sub-chanks
	Size tileSize = mapInfo->getTileSize();
	if(mChankHeight > mChankWidth)
		for(size_t i = 1;
			visibleSize.height < mChankHeight / i * tileSize.height
			&& i <= mChankHeight;
			i *= 2)
			mNumOfSubChank = i;
	else
		for(size_t i = 1;
			visibleSize.width < mChankWidth / i * tileSize.width
			&& i <= mChankWidth;
			i *= 2)
			mNumOfSubChank = i;

	return true;
}

/**
 * private
 */
void TiledLayer::stageNewChank(int num, LoadDirection direction)
{
	
}

void TiledLayer::loadTerrain(Chank *chank)
{

}

void TiledLayer::saveTerrain(Chank *chank)
{

}

void TiledLayer::saveAllTerrainOfChankStaged()
{}

void TiledLayer::allocateSpriteToChank(Chank *chank)
{

}
