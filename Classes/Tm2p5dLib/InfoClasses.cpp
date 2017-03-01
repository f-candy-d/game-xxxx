#include "InfoClasses.h"
#include "TM2P5DProperty.h"
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
	std::cout << "** DEBUG LOG OF MAP-INFO **" << '\n';
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

/**
 * MapInfo class : protected
 */
MapInfo::MapInfo()
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
	std::cout << "** DEBUG LOG OF LAYER-INFO **" << '\n';
	std::cout << "layer name => " << info->mLayerName << '\n';
	std::cout << "tile atlas name => " << info->mAtlasName << '\n';
	std::cout << "terrain source => " << info->mTerrainSource << '\n';
	std::cout << "is visible => " << info->mIsVisible << '\n';
	std::cout << "is editable => " << info->mIsEditable << '\n';
	std::cout << "\t** END **" << "\n\n";
}

/**
 * LayerInfo class : protected
 */
LayerInfo::LayerInfo()
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
	// 	std::cout << "\t** DEBUG LOG OF LAYER-BUNDLER-INFO **" << '\n';
	// 	std::cout << "\t** END **" << '\n';
}

/**
 * LayerBundlerInfo class : protected
 */
LayerBundlerInfo::LayerBundlerInfo()
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
	// 	std::cout << "\t** DEBUG LOG OF ATLAS-INFO **" << '\n';
	// 	std::cout << "\t** END **" << '\n';
}

/**
 * AtlasInfo class : protected
 */
AtlasInfo::AtlasInfo()
{}

AtlasInfo::~AtlasInfo()
{}

bool AtlasInfo::init()
{
	return true;
}
