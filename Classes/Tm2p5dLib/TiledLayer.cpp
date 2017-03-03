#include "TiledLayer.h"
#include "InfoClasses.h"
#include <iostream>
#include <fstream>

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
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileType = atlasInfo->getNumOfTileType();

	mChanks.reserve(capacity);

	//get full path for file name
	mTerrainSrc = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + DIR_NAME_TERRAIN + layerInfo->getTerrainSource());
	mAtlasSrc = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + DIR_NAME_ATLAS + atlasInfo->getAtlasSource());

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

	//Stage first chanks
	this->stageNewChank(capacity,LoadDirection::DIRECTION_END);

	return true;
}

/**
 * private
 */
void TiledLayer::stageNewChank(size_t num, LoadDirection direction)
{
	for(size_t n = 0; n < num; ++n)
	{
		if(static_cast<size_t>(mChanks.size()) < mCapacity)
		{
			//Create new chnak object
			mChanks.pushBack(Chank::create(mChankWidth,mChankHeight,mChanks.size()));
			loadTerrain(mChanks.back());
		}
		else
		{
			if(direction == LoadDirection::DIRECTION_BEGIN
				&& 0 < mChanks.front()->getIndex())
			{
				auto chank = mChanks.back();
				chank->recycle(mChanks.front()->getIndex() - 1);
				mChanks.popBack();
				mChanks.insert(0,chank);
				loadTerrain(chank);
			}
			else if(direction == LoadDirection::DIRECTION_END
				&& mChanks.back()->getIndex() < static_cast<int>(mNumOfChank - 1))
			{
				auto chank = mChanks.front();
				chank->recycle(mChanks.back()->getIndex() + 1);
				mChanks.erase(0);
				mChanks.pushBack(chank);
				loadTerrain(chank);
			}
		}
	}
}

void TiledLayer::loadTerrain(Chank *chank)
{
	std::ifstream fs(mTerrainSrc.c_str(),std::ios::binary|std::ios::in);

	if(!fs)
	{
		std::cout << "loadTerrain() :: Cannot open " << mTerrainSrc << '\n';
		return;
	}

	fs.seekg(chank->getIndex() * mChankWidth * mChankHeight * sizeof(int),std::ios::beg);
	auto tiles = chank->getTiles();
	for(size_t i = 0, size = mChankWidth * mChankHeight; i < size; ++i)
	{
		fs.read((char*)&(*tiles),sizeof(int));
		tiles++;
	}
}

void TiledLayer::saveTerrain(Chank *chank)
{
	std::ofstream fs(mTerrainSrc.c_str(),std::ios::in|std::ios::out|std::ios::binary|std::ios::app);

	if(!fs)
	{
		std::cout << "saveTerrain() :: Cannot open " << mTerrainSrc << '\n';
		return;
	}

	fs.seekp(chank->getIndex() * mChankWidth * mChankHeight * sizeof(int),std::ios::beg);
	auto tiles = chank->getTiles();
	for(size_t i = 0,size = mChankWidth * mChankHeight; i < size; ++i)
	{
		fs.write((char*)&(*tiles),sizeof(int));
		tiles++;
	}
}

void TiledLayer::saveAllTerrainOfChankStaged()
{
	for(auto itr = mChanks.begin(); itr != mChanks.end(); ++itr)
	{
		if((*itr)->getIsModified())
			this->saveTerrain(*itr);
	}
}

void TiledLayer::allocateSpriteToChank(Chank *chank)
{

}
