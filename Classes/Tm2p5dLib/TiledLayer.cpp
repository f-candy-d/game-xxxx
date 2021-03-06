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
,mSubPaneWidth(0)
,mSubPaneHeight(0)
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
,mTileTextureSize(0,0)
,mAbsoluteTileSize(0,0)
,mOriginPool(0,0)
,mSplit(Split::HORIZONTAL_SPLIT)
{}

TiledLayer::~TiledLayer()
{
	CC_SAFE_RELEASE_NULL(mBatchNode);
}

bool TiledLayer::initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize,float scale)
{
	if(!Node::init())
		return false;

	//check parameters
	if(scale < 0
	|| atlasInfo->getAtlasSource().empty()
	|| layerInfo->getTerrainSource().empty()
	|| static_cast<size_t>(atlasInfo->getNumOfTileType()) != atlasInfo->getTextureRects().size())
		return false;

	mZolder = zolder;
	mCapacity = capacity;
	mIndexOfAnchorSubPane = 0;
	mIndexOfAnchorPane = 0;
	mNumOfPane = mapInfo->getNumOfPane();
	mPaneWidth = mapInfo->getPaneWidth();
	mPaneHeight = mapInfo->getPaneHeight();
	mTileTextureSize = mapInfo->getTileSize();
	mScale = scale;

	//split of a layer
	mSplit = (mPaneHeight > mPaneWidth) ? Split::HORIZONTAL_SPLIT : Split::VERTICAL_SPLIT;

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
	this->optimizeSplitOfPane(0.35,2,mSplit,visibleSize);

	//the size of a sub-pane
	mSubPaneWidth = (mSplit == Split::HORIZONTAL_SPLIT) ? mPaneWidth : mPaneWidth / mNumOfSubPane;
	mSubPaneHeight = (mSplit == Split::HORIZONTAL_SPLIT) ? mPaneHeight / mNumOfSubPane : mPaneWidth;

	//batch node
	mBatchNode = SpriteBatchNode::create(mAtlasSrc);
	mBatchNode->retain();
	this->addChild(mBatchNode);

	// //Stage first panes and allocate sprites
	this->stagePane(0,0);
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

bool TiledLayer::stagePane(int newAnchor,int oldAnchor)
{
	std::cout << "stagePane(" << newAnchor << ", " << oldAnchor << ")" << '\n';

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
		mPanes.pushBack(Pane::create(mPaneWidth,mPaneHeight,i,mNumOfSubPane,mSplit));
	}

	// check exceptions
	if(newAnchor < 0 || static_cast<int>(mNumOfPane) - 1 < newAnchor || mPanes.size() != cap_int)
		return false;

	if(std::abs(diff_anchor) >= cap_int || diff_anchor == 0)
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
				pane->setState(State::ZOMBIE);
			}
		}
		flag = true;
	}
	else if(diff_anchor > 0)
	{
		std::cout << "left-shift" << '\n';

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
	// auto tiles = pane->getTiles();
	int type = -1;
	for(int i = 0, size = mPaneWidth * mPaneHeight; i < size; ++i)
	{
		// fs.read((char*)&(*tiles),sizeof(int));
		fs.read((char*)&type,sizeof(int));
		// tiles++;
		pane->insertTypeAt(type,i);
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
	// auto tiles = pane->getTiles();
	int type = -1;
	for(int i = 0,size = mPaneWidth * mPaneHeight; i < size; ++i)
	{
		type = pane->getTypeAt(i);
		// fs.write((char*)&(*tiles),sizeof(int));
		fs.write((char*)&type,sizeof(int));
		// tiles++;
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

	int sub_pane_width = mSubPaneWidth;
	int sub_pane_height = mSubPaneHeight;

	//Create new sprite object
	if(pane->getSprites().empty())
	{
		int sub_pane_size = sub_pane_width * sub_pane_height;
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

	if(mSplit == Split::HORIZONTAL_SPLIT)
	{
		//An origin point of a pane
		Vec2 origin(mPaneWidth * mAbsoluteTileSize.width * pane->getIndex(),0);
		int y = std::max(0, sub_pane_height * (mIndexOfAnchorSubPane - (mNumOfSubPaneDrawn -1) / 2));
		int begin = y;
		int c = 0;
		for(; y < begin + sub_pane_height * mNumOfSubPaneDrawn; ++y)
		{
			for(int x = 0; x < sub_pane_width; ++x)
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
	// if mSplit is Split::VERTICAL_SPLIT...
	else
	{
		//An origin point of a pane
		Vec2 origin(0, mPaneHeight * mAbsoluteTileSize.height * pane->getIndex());
		int x = std::max(0,sub_pane_width * (mIndexOfAnchorSubPane - (mNumOfSubPaneDrawn -1) / 2));
		int begin = x;
		for(int y = 0; y < sub_pane_height; ++y)
		{
			for(; x < begin + sub_pane_width * mNumOfSubPaneDrawn; ++x)
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

void TiledLayer::allocateSpriteToSubPane(SubPane* sub,Vec2 origin)
{
	assert(sub->ownSprites());

	if(mSplit == Split::HORIZONTAL_SPLIT)
	{
		// an origin point of a sub-pane
		origin.set(origin.x,mAbsoluteTileSize.height * mSubPaneHeight * sub->getIndex());

		int index,type;
		for(int y = 0; y < mSubPaneHeight; ++y)
		{
			for(int x = 0; x < mSubPaneWidth; ++x)
			{
				index = x + y * mSubPaneWidth;
				type = sub->getTiles()[index];
				// sub->getTiles()[index] < 0 == true means there is no tile on that point
				if(!(type < 0))
				{
					auto sprite = sub->getSprites().at(index);
					sprite->setTextureRect(mTextureRects[type]);
					sprite->setPosition(x * mAbsoluteTileSize.width + origin.x, y * mAbsoluteTileSize.height + origin.y);
				}
			}
		}
	}
}
