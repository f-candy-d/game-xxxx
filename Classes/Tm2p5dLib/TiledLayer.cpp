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

TiledLayer* TiledLayer::create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize,float scale)
{
	auto ret = new TiledLayer();
	if(ret->initWithInfo(mapInfo,layerInfo,atlasInfo,capacity,zolder,visibleSize,scale))
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
		if( static_cast<int>(mOriginPool.x) < -1 * static_cast<int>(mPaneWidth * mAbsoluteTileSize.width))
		{
			if(mCursoreOfCenterPane < mCapacity / 2)
			{
				mCursoreOfCenterPane++;
			}
			else
			{
				std::cout << "stage new pane :: direc->end" << '\n';
				size_t n = -1 * mOriginPool.x / (static_cast<int>(mPaneWidth) * mAbsoluteTileSize.width);

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
		else if( static_cast<int>(mOriginPool.x) > static_cast<int>(mPaneWidth * mAbsoluteTileSize.width))
		{
			if(mCapacity / 2 < mCursoreOfCenterPane)
			{
				mCursoreOfCenterPane--;
			}
			else
			{
				std::cout << "stage new pane :: direc->begin" << '\n';
				size_t n = mOriginPool.x / (static_cast<int>(mPaneWidth) * mAbsoluteTileSize.width);
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

void TiledLayer::onStageNewPane(int delta)
{
	int newAnchor = mIndexOfAnchorPane + delta;
	std::cout << "onStageNewPane(" << delta << ")" << '\n';
	if(!(newAnchor < 0 || static_cast<int>(mNumOfPane) - 1 < newAnchor))
	{
		if(this->stagePane(mIndexOfAnchorPane + delta,mIndexOfAnchorPane))
		{
			for(auto itr = mPanes.begin(); itr != mPanes.end(); ++itr)
			{
				// only when the state of pane is WATING_ALLOCATION, allocate tile sprites
				if((*itr)->getState() == State::WATING_ALLOCATION)
				{
					this->allocateSpriteToPane(*itr);
					(*itr)->setState(State::STAGED);
				}
			}
		}
		mIndexOfAnchorPane = newAnchor;
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
,mIndexOfAnchorPane(0)
,mNumOfTileType(0)
,mNumOfSubPane(1)
,mNumOfSubPaneDrawn(0)
,mIndexOfAnchorSubPane(0)
,mScale(1.0)
,mLayerName("")
,mTerrainSrc("")
,mAtlasSrc("")
,mIsVisible(false)
,mIsEditable(false)
,mTileSize(0,0)
,mTileTextureSize(0,0)
,mAbsoluteTileSize(0,0)
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
	mIndexOfAnchorSubPane = 0;
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

	// Stage first panes and allocate sprites
	this->stageNewPane(capacity,LoadDirection::DIRECTION_END);
	for(auto pane : mPanes)
		this->allocateSpriteToPane(pane);

	return true;
}

bool TiledLayer::initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize,float scale)
{
	if(!Node::init())
		return false;

	// //check parameters
	if(mapInfo->getOrientation() == Orientation::NONE
	|| scale < 0
	|| atlasInfo->getAtlasSource().empty()
	|| layerInfo->getTerrainSource().empty()
	|| static_cast<size_t>(atlasInfo->getNumOfTileType()) != atlasInfo->getTextureRects().size()
	|| (mapInfo->getOrientation() == Orientation::PORTRAIT && mPaneWidth < mPaneHeight)
	|| (mapInfo->getOrientation() == Orientation::LANDSCAPE && mPaneWidth > mPaneHeight))
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mIndexOfAnchorSubPane = 0;
	mCursoreOfCenterPane = 0;
	mIndexOfAnchorPane = -1 * capacity;
	mNumOfPane = mapInfo->getNumOfPane();
	mPaneWidth = mapInfo->getPaneWidth();
	mPaneHeight = mapInfo->getPaneHeight();
	mTileTextureSize = mapInfo->getTileSize();
	mScale = scale;

	//TODO : delete these lines
	mTileSize = mapInfo->getTileSize();
	mOrientation = mapInfo->getOrientation();

	mLayerName = layerInfo->getLayerName();
	mIsVisible = layerInfo->isVisible();
	mIsEditable = layerInfo->isEditable();
	mNumOfTileType = atlasInfo->getNumOfTileType();
	mAtlasSrc = DIR_NAME_TM2P5D + DIR_NAME_ATLAS + atlasInfo->getAtlasSource();

	//the abusolute size of a tile sprite
	mAbsoluteTileSize = mTileTextureSize * scale;

	//get full path for terrain file name
	mTerrainSrc = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + DIR_NAME_TERRAIN + layerInfo->getTerrainSource());

	//allocate memory
	mPanes.reserve(capacity);

	//copy texture rects
	mTextureRects.reserve(atlasInfo->getTextureRects().size());
	std::copy(atlasInfo->getTextureRects().begin(),atlasInfo->getTextureRects().end(),std::back_inserter(mTextureRects));

	// Decide the number of sub-panes
	// NOTE : split=Split::HORIZONTAL_SPLIT, surplus=2, visibleSize=(1024,768), paneSize=(16,256), absoluteTileSize=51.2px
	// pitch::1.0 => 4096 sprites (1 sub-panes was drawn)
	// pitch::0.9 => 4096 sprites (2 sub-panes was drawn)
	// pitch::0.8 => 3027 sprites (3 sub-panes was drawn)
	// pitch::0.65 => 1536 sprites (3 sub-panes was drawn)
	// pitch::0.5 => 768 sprites (3 sub-panes was drawn)
	// pitch::0.4 => 512 sprites (4 sub-panes was drawn)
	// pitch::0.35 => 512 sprites (4 sub-panes was drawn)
	// pitch::0.25 => 384 sprites (6 sub-panes was drawn)
	// pitch::0.2 => 384 sprites (6 sub-panes was drawn)
	// pitch::0.1 => 320 sprites (10 sub-panes was drawn)
	// pitch::0.0 => 272 sprites (17 sub-panes was drawn)
	this->optimizeSplitOfPane(0.35,2,Split::HORIZONTAL_SPLIT,visibleSize);

	// if(mOrientation == Orientation::LANDSCAPE)
	// 	for(size_t i = 1;
	// 		visibleSize.height < mPaneHeight / i * mAbsoluteTileSize.height
	// 		&& i <= mPaneHeight;
	// 		i *= 2)
	// 		mNumOfSubPane = i;
	// else
	// //If the orientation is portrait...
	// 	for(size_t i = 1;
	// 		visibleSize.width < mPaneWidth / i * mAbsoluteTileSize.width
	// 		&& i <= mPaneWidth;
	// 		i *= 2)
	// 		mNumOfSubPane = i;

	//batch node
	mBatchNode = SpriteBatchNode::create(mAtlasSrc);
	mBatchNode->retain();
	this->addChild(mBatchNode);

	// //Stage first panes and allocate sprites
	// this->stageNewPane(capacity,LoadDirection::DIRECTION_END);
	this->stagePane(0,mIndexOfAnchorPane);
	mIndexOfAnchorPane = 0;
	for(auto pane : mPanes)
	{
		this->allocateSpriteToPane(pane);
		pane->setState(State::STAGED);
	}

	return true;
}

/**
 * private
 */
void TiledLayer::optimizeSplitOfPane(float pitch,int surplus,Split split,Size visibleSize)
{
	assert(0.0 <= pitch && pitch <= 1.0);

	int side_pane = (split == Split::HORIZONTAL_SPLIT) ? mPaneHeight : mPaneWidth;
	int side_screen = (split == Split::HORIZONTAL_SPLIT) ? visibleSize.height : visibleSize.width;
	float side_tile = (split == Split::HORIZONTAL_SPLIT) ? mAbsoluteTileSize.height : mAbsoluteTileSize.width;

	if(side_pane <= 0)
		return;

	// round off log2(side_pane) * pitch to the nearest integer
	int exponent = static_cast<int>(std::log2(side_pane) * pitch + 0.5);

	// side_sub_pane is 2^exponent
	int side_sub_pane = 1;
	for(int i = 0; i < exponent; ++i)
		side_sub_pane *= 2;

	mNumOfSubPane = side_pane / side_sub_pane;

	// decide the number of sub-panes that will be drawn on the screen
	float threshold = (side_screen + side_sub_pane * side_tile * surplus) / (side_sub_pane * side_tile);
	for(mNumOfSubPaneDrawn = 1; mNumOfSubPaneDrawn < threshold; ++mNumOfSubPaneDrawn);
	// mNumOfSubPaneDrawn must be equal to mNumOfSubPane or smaller than it (mNumOfSubPane >= mNumOfSubPaneDrawn)
	mNumOfSubPaneDrawn = (mNumOfSubPane < mNumOfSubPaneDrawn) ? mNumOfSubPane : mNumOfSubPaneDrawn;

	std::cout << "TiledLayer::optimizeSplitOfPane() => " << mNumOfSubPane << " >= " << mNumOfSubPaneDrawn << " >= " << threshold << '\n';
}

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

bool TiledLayer::stagePane(int newAnchor,int oldAnchor)
{
	std::cout << "stagePane(" << newAnchor << ", " << oldAnchor << ")" << '\n';
	// NOTE : The default value of mIndexOfAnchorPane must be (-1 * mCapacity).

	//staged new pane or not
	bool flag = false;

	// the difference of a index of an old anchor pane and that of new one
	int diff_anchor = newAnchor - oldAnchor;

	// use cap_int instead of mCapacity in this function
	int cap_int = static_cast<int>(mCapacity);

	// fill a vector contains Pane* with dammy Pane objects
	if(mPanes.size() == 0)
	{
		std::cout << "first time of stagePane()" << '\n';
		for(int i = 0; i < cap_int; ++i)
		mPanes.pushBack(Pane::create(mPaneWidth,mPaneHeight,i));
	}

	// check exceptions
	if(newAnchor < 0 || static_cast<int>(mNumOfPane) - 1 < newAnchor || mPanes.size() != cap_int)
		return false;

	if(std::abs(diff_anchor) >= cap_int)
	{
		std::cout << "replace all panes staged" << '\n';
		int range_begin = std::max(newAnchor - (cap_int / 2), 0);
		int range_end = std::min(newAnchor + ((cap_int - 1) / 2), static_cast<int>(mNumOfPane) - 1);
		int left_end_index = cap_int / 2 - (newAnchor - range_begin);
		int right_end_index = cap_int / 2 + (range_end - newAnchor);

		for(int i = 0, j = range_begin; i < cap_int; ++i)
		{
			auto pane = mPanes.at(i);
			if(left_end_index <= i && i <= right_end_index)
			{
				if(pane->getIsModified())
					this->saveTerrain(pane);
				pane->recycle(j);
				this->loadTerrain(pane);
				pane->setState(State::WATING_ALLOCATION);

				std::cout << "staged a pane at index " << i << "  #pane.index = " << j << '\n';
				++j;
			}
			else
			{
				std::cout << "set the state of a pane at index " << i << " as State::ZOMBIE" << '\n';
				// pane->setIsNullState(true);
				pane->setState(State::ZOMBIE);
			}
		}
		flag = true;
	}
	else if(diff_anchor > 0)
	{
		std::cout << "left-shift" << '\n';

		// int old_range_begin = std::max(oldAnchor - (cap_int / 2), 0);
		int old_range_end = std::min(oldAnchor + ((cap_int - 1) / 2), static_cast<int>(mNumOfPane) - 1);
		int right_end_index = cap_int / 2 + (old_range_end - oldAnchor);
		int right_end_pane_index = mPanes.at(right_end_index)->getIndex();

		// left-shit
		for(int i = 0; i < diff_anchor; ++i)
		{
			auto pane = mPanes.front();
			pane->retain();
			if(pane->getIsModified())
				this->saveTerrain(pane);
			mPanes.erase(0);
			mPanes.pushBack(pane);
			pane->release();
		}

		for(int i = right_end_index - diff_anchor + 1, j = right_end_pane_index + 1; i < cap_int; ++i, ++j)
		{
			auto pane = mPanes.at(i);
			if(j < static_cast<int>(mNumOfPane))
			{
				pane->recycle(j);
				this->loadTerrain(pane);
				flag = true;
				pane->setState(State::WATING_ALLOCATION);

				std::cout << "staged a pane at index " << i << "  #pane.index = " << j << '\n';
			}
			else
			{
				// pane->setIsNullState(true);
				pane->setState(State::ZOMBIE);
				std::cout << "set the state of a pane at index " << i << " as State::ZOMBIE" << '\n';
			}
		}
	}
	else if(diff_anchor < 0)
	{
		std::cout << "right_shift" << '\n';

		int old_range_begin = std::max(oldAnchor - (cap_int / 2), 0);
		int left_end_index = cap_int / 2 - (oldAnchor - old_range_begin);
		int left_end_pane_index = mPanes.at(left_end_index)->getIndex();

		// right-shift
		for(int i = 0; i < -1 * diff_anchor; ++i)
		{
			auto pane = mPanes.back();
			pane->retain();
			if(pane->getIsModified())
				this->saveTerrain(pane);
			mPanes.popBack();
			mPanes.insert(0,pane);
			pane->release();
		}

		for(int i = left_end_index - diff_anchor - 1, j = left_end_pane_index - 1; 0 <= i; --i, --j)
		{
			auto pane = mPanes.at(i);
			if(0 <= j)
			{
				pane->recycle(j);
				this->loadTerrain(pane);
				flag = true;
				pane->setState(State::WATING_ALLOCATION);

				std::cout << "staged a pane at index " << i << "  #pane.index = " << j << '\n';
			}
			else
			{
				// pane->setIsNullState(true);
				pane->setState(State::ZOMBIE);
				std::cout << "set the state of a pane at index " << i << " as State::ZOMBIE" << '\n';
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
	//pane has nothing to be drawn,do not draw sprites
	if(pane->getState() == State::ZOMBIE)
		return;

	int sub_pane_w,sub_pane_h,sub_pane_size;

	sub_pane_w = (mPaneHeight > mPaneWidth) ? mPaneWidth : mPaneWidth / mNumOfSubPane;
	sub_pane_h = (mPaneHeight > mPaneWidth) ? mPaneHeight / mNumOfSubPane : mPaneHeight;
	sub_pane_size = sub_pane_w * sub_pane_h;

	//Create new sprite object
	if(pane->getSprites().empty())
	{
		for(int i = 0; i < sub_pane_size * mNumOfSubPaneDrawn; ++i)
		{
			auto sprite = Sprite::create(mAtlasSrc);
			mBatchNode->addChild(sprite);
			sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			sprite->setContentSize(mAbsoluteTileSize);
			sprite->setScale(mScale,mScale);
			pane->addSprite(sprite);
		}
	}

	if(mOrientation == Orientation::LANDSCAPE)
	{
		//An origin point of a pane
		Vec2 origin(mPaneWidth * mAbsoluteTileSize.width * pane->getIndex(),0);
		int y = std::max(0, sub_pane_h * (mIndexOfAnchorSubPane - (mNumOfSubPaneDrawn -1) / 2));
		int c = 0;
		for(; y < sub_pane_h * mNumOfSubPaneDrawn; ++y)
		{
			for(int x = 0; x < sub_pane_w; ++x)
			{
				// (pane->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(pane->getTypeAt(x,y) < 0))
				{
					auto sprite = pane->getSprites().at(y * mPaneWidth + x);
					sprite->setTextureRect(mTextureRects[pane->getTypeAt(x,y)]);
					sprite->setPosition(x * mAbsoluteTileSize.width + origin.x, y * mAbsoluteTileSize.height + origin.y);
					c++;
				}
			}
		}
		std::cout << "allocated " << c << " sprites of the pane at index " << pane->getIndex() << '\n';
	}
	//if the orientation is PORTRAIT...
	else
	{
		//An origin point of a pane
		Vec2 origin(0, mPaneHeight * mAbsoluteTileSize.height * pane->getIndex());
		int x = std::max(0,sub_pane_w * (mIndexOfAnchorSubPane - (mNumOfSubPaneDrawn -1) / 2));
		for(int y = 0; y < sub_pane_h; ++y)
		{
			for(; x < sub_pane_w * mNumOfSubPaneDrawn; ++x)
			{
				// (pane->getTypeAt(x,y) < 0 == true means there is no tile on that point
				if(!(pane->getTypeAt(x,y) < 0))
				{
					auto sprite = pane->getSprites().at(y * mPaneWidth + x);
					sprite->setTextureRect(mTextureRects[pane->getTypeAt(x,y)]);
					sprite->setPosition(x * mAbsoluteTileSize.width + origin.x, y * mAbsoluteTileSize.height + origin.y);
				}
			}
		}
	}

}
