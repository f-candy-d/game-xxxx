#include "InfoClasses.h"
#include <iostream>

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * MapInfo class : pulic
 */
MapInfo* MapInfo::create()
{
	auto ret = new MapInfo();
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void MapInfo::debugLog(MapInfo *info)
{
	std::cout << "\n** DEBUG LOG OF MAP-INFO **" << '\n';
	std::cout << "width of chank => " << info->mChankWidth << '\n';
	std::cout << "height of chank => " << info->mChankHeight << '\n';
	std::cout << "number of chanks => " << info->mNumOfChank << '\n';
	std::cout << "orientation => " << Orientation::toString(info->mOrientation) << '\n';

	auto size = info->mTileSize;
	std::cout << "tile size => w=" << size.width << " h=" << size.height << '\n';

	for(auto name : info->mArchitecture)
	{
		std::cout << "architecture => " << name << '\n';
	}

	std::cout << "\t** END **" << "\n\n";
}

size_t MapInfo::getChankWidth()
{
	return mChankWidth;
}

size_t MapInfo::getChankHeight()
{
	return mChankHeight;
}

size_t MapInfo::getNumOfChank()
{
	return mNumOfChank;
}

Orientation::Type MapInfo::getOrientation()
{
	return mOrientation;
}

cocos2d::Size MapInfo::getTileSize()
{
	return mTileSize;
}

const std::vector<std::string>& MapInfo::getArchitecture()
{
	return mArchitecture;
}

/**
 * MapInfo class : protected
 */
MapInfo::MapInfo()
:mChankWidth(0)
,mChankHeight(0)
,mNumOfChank(0)
,mOrientation(Orientation::NONE)
,mTileSize(Size(0,0))
{}

MapInfo::~MapInfo()
{}

bool MapInfo::init()
{
	return true;
}

/**
 * LayerInfo class : pulic
 */
LayerInfo* LayerInfo::create()
{
	auto ret = new LayerInfo();
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void LayerInfo::debugLog(LayerInfo *info)
{
	std::cout << "\n** DEBUG LOG OF LAYER-INFO **" << '\n';
	std::cout << "layer name => " << info->mLayerName << '\n';
	std::cout << "tile atlas name => " << info->mAtlasName << '\n';
	std::cout << "terrain source => " << info->mTerrainSource << '\n';
	std::cout << "is visible => " << info->mIsVisible << '\n';
	std::cout << "is editable => " << info->mIsEditable << '\n';
	std::cout << "\t** END **" << "\n\n";
}

std::string LayerInfo::getLayerName()
{
	return mLayerName;
}

std::string LayerInfo::getAtlasName()
{
	return mAtlasName;
}

std::string LayerInfo::getTerrainSource()
{
	return mTerrainSource;
}

bool LayerInfo::isVisible()
{
	return mIsVisible;
}

bool LayerInfo::isEditable()
{
	return mIsEditable;
}

/**
 * LayerInfo class : protected
 */
LayerInfo::LayerInfo()
:mLayerName("")
,mAtlasName("")
,mTerrainSource("")
,mIsVisible(false)
,mIsEditable(false)
{}

LayerInfo::~LayerInfo()
{}

bool LayerInfo::init()
{
	return true;
}


/**
 * LayerBundlerInfo class : pulic
 */
LayerBundlerInfo* LayerBundlerInfo::create()
{
	auto ret = new LayerBundlerInfo();
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void LayerBundlerInfo::debugLog(LayerBundlerInfo *info)
{
		std::cout << "\n** DEBUG LOG OF LAYER-BUNDLER-INFO **" << '\n';
		std::cout << "layer name => " << info->getLayerName() << '\n';

		for(auto name : info->mArchitecture)
		{
			std::cout << "architecture => " << name << '\n';
		}

		std::cout << "\t** END **" << "\n\n";
}

std::string LayerBundlerInfo::getLayerName()
{
	return mLayerName;
}

const std::vector<std::string>& LayerBundlerInfo::getArchitecture()
{
	return mArchitecture;
}

/**
 * LayerBundlerInfo class : protected
 */
LayerBundlerInfo::LayerBundlerInfo()
:mLayerName("")
{}

LayerBundlerInfo::~LayerBundlerInfo()
{}

bool LayerBundlerInfo::init()
{
	return true;
}


/**
 * AtlasInfo class : pulic
 */
AtlasInfo* AtlasInfo::create()
{
	auto ret = new AtlasInfo();
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void AtlasInfo::debugLog(AtlasInfo *info)
{
		std::cout << "\n** DEBUG LOG OF ATLAS-INFO **" << '\n';
		std::cout << "atlas name => " << info->getAtlasName() << '\n';
		std::cout << "atlas source => " << info->getAtlasSource() << '\n';
		std::cout << "number of tile type => " << info->getNumOfTileType() << '\n';

		for(auto rect : info->getTextureRects())
			std::cout << "texture rect => " << rect.origin.x << ", " << rect.origin.y << ", " << rect.size.width << ", " << rect.size.height << '\n';

		std::cout << "\t** END **" << "\n\n";
}

std::string AtlasInfo::getAtlasName()
{
	return mAtlasName;
}

std::string AtlasInfo::getAtlasSource()
{
	return mAtlasSource;
}

int AtlasInfo::getNumOfTileType()
{
	return mNumOfTileType;
}

const std::vector<Rect>& AtlasInfo::getTextureRects()
{
	return mTextureRects;
}

/**
 * AtlasInfo class : protected
 */
AtlasInfo::AtlasInfo()
:mAtlasName("")
,mAtlasSource("")
,mNumOfTileType(0)
{}

AtlasInfo::~AtlasInfo()
{}

bool AtlasInfo::init()
{
	return true;
}
