#include "TiledLayer.h"
#include "InfoClasses.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

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

void TiledLayer::onOriginChanged(Vec2 newOrigin)
{
	mOriginPool += newOrigin;

	if(mOrientation == Orientation::LANDSCAPE)
	{
		if( static_cast<int>(mOriginPool.x) < -1 * static_cast<int>(mChankWidth * mTileSize.width))
		{
			if(mCursoreOfCenterChank < mCapacity / 2)
			{
				mCursoreOfCenterChank++;
			}
			else
			{
				std::cout << "stage new chank :: direc->end" << '\n';
				size_t n = -1 * mOriginPool.x / (static_cast<int>(mChankWidth) * mTileSize.width);

				//if some new chanks was staged,allocate tile sprites
				if(this->stageNewChank(n,LoadDirection::DIRECTION_END))
				{
					size_t i = 0;
					for(auto itr = mChanks.end() - 1; i < n && itr != mChanks.begin(); ++i,--itr)
						this->allocateSpriteToChank(*itr);
				}
				else
				{
					mCursoreOfCenterChank = std::min(mCapacity - 1,mCursoreOfCenterChank + 1);
				}
			}

			mOriginPool.x = 0;
		}
		else if( static_cast<int>(mOriginPool.x) > static_cast<int>(mChankWidth * mTileSize.width))
		{
			if(mCapacity / 2 < mCursoreOfCenterChank)
			{
				mCursoreOfCenterChank--;
			}
			else
			{
				std::cout << "stage new chank :: direc->begin" << '\n';
				size_t n = mOriginPool.x / (static_cast<int>(mChankWidth) * mTileSize.width);
				//if some new chanks was staged,allocate tile sprites
				if(this->stageNewChank(n,LoadDirection::DIRECTION_BEGIN))
				{
					size_t i = 0;
					for(auto itr = mChanks.begin(); i < n && itr != mChanks.end(); ++i,++itr)
						this->allocateSpriteToChank(*itr);
				}
				else
				{
					mCursoreOfCenterChank = std::max(0,static_cast<int>(mCursoreOfCenterChank) - 1);
				}
			}

			mOriginPool.x = 0;
		}
	}
	else
	//if the orientation is LANDSCAPE...
	{

	}
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
,mCursoreOfCenterChank(0)
,mNumOfTileType(0)
,mNumOfSubChank(1)
,mIndexOfActiveSubChank(0)
,mLayerName("")
,mTerrainSrc("")
,mAtlasSrc("")
,mIsVisible(false)
,mIsEditable(false)
,mTileSize(0,0)
,mOriginPool(0,0)
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
	|| layerInfo->getTerrainSource().empty()
	|| static_cast<size_t>(atlasInfo->getNumOfTileType()) != atlasInfo->getTextureRects().size()
	|| (mapInfo->getOrientation() == Orientation::PORTRAIT && mChankWidth < mChankHeight)
	|| (mapInfo->getOrientation() == Orientation::LANDSCAPE && mChankWidth > mChankHeight))
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mIndexOfActiveSubChank = 0;
	mCursoreOfCenterChank = 0;
	mNumOfChank = mapInfo->getNumOfChank();
	mChankWidth = mapInfo->getChankWidth();
	mChankHeight = mapInfo->getChankHeight();
	mTileSize = mapInfo->getTileSize();
	mOrientation = mapInfo->getOrientation();
	mLayerName = layerInfo->getLayerName();
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileType = atlasInfo->getNumOfTileType();
	mAtlasSrc = DIR_NAME_TM2P5D + DIR_NAME_ATLAS + atlasInfo->getAtlasSource();

	//get full path for terrain file name
	mTerrainSrc = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + DIR_NAME_TERRAIN + layerInfo->getTerrainSource());

	mChanks.reserve(capacity);

	//copy texture rects
	mTextureRects.reserve(atlasInfo->getTextureRects().size());
	std::copy(atlasInfo->getTextureRects().begin(),atlasInfo->getTextureRects().end(),std::back_inserter(mTextureRects));

	// Decide the number of sub-chanks
	if(mOrientation == Orientation::LANDSCAPE)
		for(size_t i = 1;
			visibleSize.height < mChankHeight / i * mTileSize.height
			&& i <= mChankHeight;
			i *= 2)
			mNumOfSubChank = i;
	else
	//If the orientation is portrait...
		for(size_t i = 1;
			visibleSize.width < mChankWidth / i * mTileSize.width
			&& i <= mChankWidth;
			i *= 2)
			mNumOfSubChank = i;

	//batch node
	mBatchNode = SpriteBatchNode::create(mAtlasSrc);
	mBatchNode->retain();
	this->addChild(mBatchNode);

	//Stage first chanks and allocate sprites
	this->stageNewChank(capacity,LoadDirection::DIRECTION_END);
	for(auto chank : mChanks)
		this->allocateSpriteToChank(chank);

	return true;
}

/**
 * private
 */
bool TiledLayer::stageNewChank(size_t num, LoadDirection direction)
{
	//if some chank is staged,this is true
	bool flag = false;

	for(size_t n = 0; n < num; ++n)
	{
		if(static_cast<size_t>(mChanks.size()) < mCapacity)
		{
			//Create new chnak object
			mChanks.pushBack(Chank::create(mChankWidth,mChankHeight,mChanks.size()));
			this->loadTerrain(mChanks.back());
		}
		else
		{
			if(direction == LoadDirection::DIRECTION_BEGIN
				&& 0 < mChanks.front()->getIndex())
			{
				auto chank = mChanks.back();
				chank->retain();
				if(chank->getIsModified())
					this->saveTerrain(chank);
				chank->recycle(mChanks.front()->getIndex() - 1);
				mChanks.popBack();
				mChanks.insert(0,chank);
				this->loadTerrain(chank);
				chank->release();

				flag = true;
			}
			else if(direction == LoadDirection::DIRECTION_END
				&& mChanks.back()->getIndex() < static_cast<int>(mNumOfChank - 1))
			{
				auto chank = mChanks.front();
				chank->retain();
				if(chank->getIsModified())
					this->saveTerrain(chank);
				chank->recycle(mChanks.back()->getIndex() + 1);
				mChanks.erase(0);
				mChanks.pushBack(chank);
				this->loadTerrain(chank);
				chank->release();

				flag = true;
			}
		}
	}

	return flag;
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
	int sub_chank_w,sub_chank_h,sub_chank_size;

	sub_chank_w = (mChankHeight > mChankWidth) ? mChankWidth : mChankWidth / mNumOfSubChank;
	sub_chank_h = (mChankHeight > mChankWidth) ? mChankHeight / mNumOfSubChank : mChankHeight;
	sub_chank_size = sub_chank_w * sub_chank_h;

	//Create new sprite object
	if(chank->getSprites().empty())
	{
		for(int i = 0; i < sub_chank_size * NUM_OF_DRAWN_SUB_CHANK; ++i)
		{
			auto sprite = Sprite::create(mAtlasSrc);
			mBatchNode->addChild(sprite);
			sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			chank->addSprite(sprite);
		}
	}

	if(mOrientation == Orientation::LANDSCAPE)
	{
		//An origin point of a chank
		Vec2 origin(mChankWidth * mTileSize.width * chank->getIndex(),0);
		int y = std::max(0, sub_chank_h * (mIndexOfActiveSubChank - (NUM_OF_DRAWN_SUB_CHANK -1) / 2));
		int c = 0;
		for(; y < sub_chank_h * NUM_OF_DRAWN_SUB_CHANK; ++y)
		{
			for(int x = 0; x < sub_chank_w; ++x)
			{
				// (chank->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(chank->getTypeAt(x,y) < 0))
				{
					auto sprite = chank->getSprites().at(y * mChankWidth + x);
					sprite->setTextureRect(mTextureRects[chank->getTypeAt(x,y)]);
					sprite->setPosition(x * mTileSize.width + origin.x, y * mTileSize.height + origin.y);
					c++;
				}
			}
		}
		std::cout << "allocated " << c << " sprites" << '\n';
	}
	//if the orientation is PORTRAIT...
	else
	{
		//An origin point of a chank
		Vec2 origin(0, mChankHeight * mTileSize.height * chank->getIndex());
		int x = std::max(0,sub_chank_w * (mIndexOfActiveSubChank - (NUM_OF_DRAWN_SUB_CHANK -1) / 2));
		for(int y = 0; y < sub_chank_h; ++y)
		{
			for(; x < sub_chank_w * NUM_OF_DRAWN_SUB_CHANK; ++x)
			{
				// (chank->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(chank->getTypeAt(x,y) < 0))
				{
					auto sprite = chank->getSprites().at(y * mChankWidth + x);
					sprite->setTextureRect(mTextureRects[chank->getTypeAt(x,y)]);
					sprite->setPosition(x * mTileSize.width + origin.x, y * mTileSize.height + origin.y);
				}
			}
		}
	}

}
