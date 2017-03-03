#include "TiledLayer.h"

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
TiledLayer* TiledLayer::createcreate(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder)
{
	auto ret = new TiledLayer();
	if(ret->initWithInfo(mapInfo,layerInfo,atlasInfo,capacity,zolder))
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

bool TiledLayer::initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder)
{
	if(!Node::init())
		return false;

	//check parameters
	if(mapInfo->getOrientation() == Orientation::NONE
	|| atlasInfo->getAtlasSource().empty()
	|| atlasInfo->getNumOfTileType() != atlasInfo->getTextureRects().size())
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mNumOfChank = mapInfo->getNumOfChank();
	mChankWidth = mapInfo->getChankWidth();
	mChankHeight = mapInfo->getChankHeight();
	mOrientation = mapInfo->getOrientation();
	mLayerName = layerInfo->getLayerName();
	mTerrainSrc = layerInfo->getTerrainSource();
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileTypes = atlasInfo->getNumOfTileType();
	mAtlasSrc = atlasInfo->getAtlasSource();

	//Decide the number of sub-chanks
	Size winSize = Director::getInstance()->getWinSize();
	Size tileSize = mapInfo->getTileSize();
	if(mChankHeight > mChankWidth)


	return true;
}

/**
 * private
 */
