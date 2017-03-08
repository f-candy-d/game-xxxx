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
		if( static_cast<int>(mOriginPool.x) < -1 * static_cast<int>(mPaneWidth * mTileSize.width))
		{
			if(mCursoreOfCenterPane < mCapacity / 2)
			{
				mCursoreOfCenterPane++;
			}
			else
			{
				std::cout << "stage new pane :: direc->end" << '\n';
				size_t n = -1 * mOriginPool.x / (static_cast<int>(mPaneWidth) * mTileSize.width);

				//if some new panes was staged,allocate tile sprites
				if(this->stageNewPane(n,LoadDirection::DIRECTION_END))
				{
					size_t i = 0;
					for(auto itr = mPanes.end() - 1; i < n && itr != mPanes.begin(); ++i,--itr)
						this->allocateSpriteToPane(*itr);
				}
				else
				{
					mCursoreOfCenterPane = std::min(mCapacity - 1,mCursoreOfCenterPane + 1);
				}
			}

			mOriginPool.x = 0;
		}
		else if( static_cast<int>(mOriginPool.x) > static_cast<int>(mPaneWidth * mTileSize.width))
		{
			if(mCapacity / 2 < mCursoreOfCenterPane)
			{
				mCursoreOfCenterPane--;
			}
			else
			{
				std::cout << "stage new pane :: direc->begin" << '\n';
				size_t n = mOriginPool.x / (static_cast<int>(mPaneWidth) * mTileSize.width);
				//if some new panes was staged,allocate tile sprites
				if(this->stageNewPane(n,LoadDirection::DIRECTION_BEGIN))
				{
					size_t i = 0;
					for(auto itr = mPanes.begin(); i < n && itr != mPanes.end(); ++i,++itr)
						this->allocateSpriteToPane(*itr);
				}
				else
				{
					mCursoreOfCenterPane = std::max(0,static_cast<int>(mCursoreOfCenterPane) - 1);
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
,mNumOfPane(0)
,mPaneWidth(0)
,mPaneHeight(0)
,mCursoreOfCenterPane(0)
,mNumOfTileType(0)
,mNumOfSubPane(1)
,mIndexOfActiveSubPane(0)
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
	|| (mapInfo->getOrientation() == Orientation::PORTRAIT && mPaneWidth < mPaneHeight)
	|| (mapInfo->getOrientation() == Orientation::LANDSCAPE && mPaneWidth > mPaneHeight))
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mIndexOfActiveSubPane = 0;
	mCursoreOfCenterPane = 0;
	mNumOfPane = mapInfo->getNumOfPane();
	mPaneWidth = mapInfo->getPaneWidth();
	mPaneHeight = mapInfo->getPaneHeight();
	mTileSize = mapInfo->getTileSize();
	mOrientation = mapInfo->getOrientation();
	mLayerName = layerInfo->getLayerName();
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileType = atlasInfo->getNumOfTileType();
	mAtlasSrc = DIR_NAME_TM2P5D + DIR_NAME_ATLAS + atlasInfo->getAtlasSource();

	//get full path for terrain file name
	mTerrainSrc = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + DIR_NAME_TERRAIN + layerInfo->getTerrainSource());

	mPanes.reserve(capacity);

	//copy texture rects
	mTextureRects.reserve(atlasInfo->getTextureRects().size());
	std::copy(atlasInfo->getTextureRects().begin(),atlasInfo->getTextureRects().end(),std::back_inserter(mTextureRects));

	// Decide the number of sub-panes
	if(mOrientation == Orientation::LANDSCAPE)
		for(size_t i = 1;
			visibleSize.height < mPaneHeight / i * mTileSize.height
			&& i <= mPaneHeight;
			i *= 2)
			mNumOfSubPane = i;
	else
	//If the orientation is portrait...
		for(size_t i = 1;
			visibleSize.width < mPaneWidth / i * mTileSize.width
			&& i <= mPaneWidth;
			i *= 2)
			mNumOfSubPane = i;

	//batch node
	mBatchNode = SpriteBatchNode::create(mAtlasSrc);
	mBatchNode->retain();
	this->addChild(mBatchNode);

	//Stage first panes and allocate sprites
	this->stageNewPane(capacity,LoadDirection::DIRECTION_END);
	for(auto pane : mPanes)
		this->allocateSpriteToPane(pane);

	return true;
}

/**
 * private
 */
bool TiledLayer::stageNewPane(size_t num, LoadDirection direction)
{
	//if some pane is staged,this is true
	bool flag = false;

	for(size_t n = 0; n < num; ++n)
	{
		if(static_cast<size_t>(mPanes.size()) < mCapacity)
		{
			//Create new chnak object
			mPanes.pushBack(Pane::create(mPaneWidth,mPaneHeight,mPanes.size()));
			this->loadTerrain(mPanes.back());
		}
		else
		{
			if(direction == LoadDirection::DIRECTION_BEGIN
				&& 0 < mPanes.front()->getIndex())
			{
				auto pane = mPanes.back();
				pane->retain();
				if(pane->getIsModified())
					this->saveTerrain(pane);
				pane->recycle(mPanes.front()->getIndex() - 1);
				mPanes.popBack();
				mPanes.insert(0,pane);
				this->loadTerrain(pane);
				pane->release();

				flag = true;
			}
			else if(direction == LoadDirection::DIRECTION_END
				&& mPanes.back()->getIndex() < static_cast<int>(mNumOfPane - 1))
			{
				auto pane = mPanes.front();
				pane->retain();
				if(pane->getIsModified())
					this->saveTerrain(pane);
				pane->recycle(mPanes.back()->getIndex() + 1);
				mPanes.erase(0);
				mPanes.pushBack(pane);
				this->loadTerrain(pane);
				pane->release();

				flag = true;
			}
		}
	}

	return flag;
}

void TiledLayer::loadTerrain(Pane *pane)
{
	std::ifstream fs(mTerrainSrc.c_str(),std::ios::binary|std::ios::in);

	if(!fs)
	{
		std::cout << "loadTerrain() :: Cannot open " << mTerrainSrc << '\n';
		return;
	}

	fs.seekg(pane->getIndex() * mPaneWidth * mPaneHeight * sizeof(int),std::ios::beg);
	auto tiles = pane->getTiles();
	for(size_t i = 0, size = mPaneWidth * mPaneHeight; i < size; ++i)
	{
		fs.read((char*)&(*tiles),sizeof(int));
		tiles++;
	}
}

void TiledLayer::saveTerrain(Pane *pane)
{
	std::ofstream fs(mTerrainSrc.c_str(),std::ios::in|std::ios::out|std::ios::binary|std::ios::app);

	if(!fs)
	{
		std::cout << "saveTerrain() :: Cannot open " << mTerrainSrc << '\n';
		return;
	}

	fs.seekp(pane->getIndex() * mPaneWidth * mPaneHeight * sizeof(int),std::ios::beg);
	auto tiles = pane->getTiles();
	for(size_t i = 0,size = mPaneWidth * mPaneHeight; i < size; ++i)
	{
		fs.write((char*)&(*tiles),sizeof(int));
		tiles++;
	}
}

void TiledLayer::saveAllTerrainOfPaneStaged()
{
	for(auto itr = mPanes.begin(); itr != mPanes.end(); ++itr)
	{
		if((*itr)->getIsModified())
			this->saveTerrain(*itr);
	}
}

void TiledLayer::allocateSpriteToPane(Pane *pane)
{
	int sub_pane_w,sub_pane_h,sub_pane_size;

	sub_pane_w = (mPaneHeight > mPaneWidth) ? mPaneWidth : mPaneWidth / mNumOfSubPane;
	sub_pane_h = (mPaneHeight > mPaneWidth) ? mPaneHeight / mNumOfSubPane : mPaneHeight;
	sub_pane_size = sub_pane_w * sub_pane_h;

	//Create new sprite object
	if(pane->getSprites().empty())
	{
		for(int i = 0; i < sub_pane_size * NUM_OF_DRAWN_SUB_CHANK; ++i)
		{
			auto sprite = Sprite::create(mAtlasSrc);
			mBatchNode->addChild(sprite);
			sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			pane->addSprite(sprite);
		}
	}

	if(mOrientation == Orientation::LANDSCAPE)
	{
		//An origin point of a pane
		Vec2 origin(mPaneWidth * mTileSize.width * pane->getIndex(),0);
		int y = std::max(0, sub_pane_h * (mIndexOfActiveSubPane - (NUM_OF_DRAWN_SUB_CHANK -1) / 2));
		int c = 0;
		for(; y < sub_pane_h * NUM_OF_DRAWN_SUB_CHANK; ++y)
		{
			for(int x = 0; x < sub_pane_w; ++x)
			{
				// (pane->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(pane->getTypeAt(x,y) < 0))
				{
					auto sprite = pane->getSprites().at(y * mPaneWidth + x);
					sprite->setTextureRect(mTextureRects[pane->getTypeAt(x,y)]);
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
		//An origin point of a pane
		Vec2 origin(0, mPaneHeight * mTileSize.height * pane->getIndex());
		int x = std::max(0,sub_pane_w * (mIndexOfActiveSubPane - (NUM_OF_DRAWN_SUB_CHANK -1) / 2));
		for(int y = 0; y < sub_pane_h; ++y)
		{
			for(; x < sub_pane_w * NUM_OF_DRAWN_SUB_CHANK; ++x)
			{
				// (pane->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(pane->getTypeAt(x,y) < 0))
				{
					auto sprite = pane->getSprites().at(y * mPaneWidth + x);
					sprite->setTextureRect(mTextureRects[pane->getTypeAt(x,y)]);
					sprite->setPosition(x * mTileSize.width + origin.x, y * mTileSize.height + origin.y);
				}
			}
		}
	}

}
