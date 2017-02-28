#include "TM2P5DJsonParser.h"
#include <iostream>

USING_NS_CC;
using namespace TM2P5DComponent;

//Directory names
static const std::string DIR_NAME_TM2P5D("tm2p5d/");
static const std::string DIR_NAME_ATLAS("atlas/");
static const std::string DIR_NAME_TERRAIN("terrain/");

/**
 * public
 */
TM2P5DJsonParser* TM2P5DJsonParser::create()
{
	auto ret = new TM2P5DJsonParser();
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void TM2P5DJsonParser::parseOriginJson(std::string origin)
{
	//parse a origin file
	std::string path = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + origin);
	parseJson(path);

	if(!this->isError())
	{
		//parse other information files
	}
	else
	{
		coutLastError();
	}
}

bool TM2P5DJsonParser::isError()
{
	return !picojson::get_last_error().empty();
}

void outputLastError()
{
	if(isError())
		std::cout << "picojson : error => " << picojson::get_last_error() << '\n';
	else
		std::cout << "TM2P5DJsonParser : mes => There are no errors for now" << '\n';
}

MapInfo* TM2P5DJsonParser::getMapInfo()
{
	return mMapInfo;
}

LayerInfo* TM2P5DJsonParser::getLayerInfoByName(std::string name)
{
	return mLayerInfoMap.at(name);
}

LayerBundlerInfo* TM2P5DJsonParser::getLayerBundlerInfoByName(std::string name)
{
	return mLayerBundlerInfoMap.at(name);
}

AtlasInfo* TM2P5DJsonParser::getAtlasInfoByName(std::string name)
{
	return mAtlasInfoMap.at(name);
}

void TM2P5DJsonParser::debugLogOfMapInfo(MapInfo* info)
{
	std::cout << "\t** DEBUG LOG OF MAP-INFO **" << '\n';
	std::cout << "\t** END **" << '\n';
}

void TM2P5DJsonParser::debugLogOfLayerInfo(LayerInfo* info)
{
	std::cout << "\t** DEBUG LOG OF LAYER-INFO **" << '\n';
	std::cout << "\t** END **" << '\n';
}

void TM2P5DJsonParser::debugLogOfLayerBundlerInfo(LayerBundlerInfo* info)
{
	std::cout << "\t** DEBUG LOG OF LAYER-BUNDLER-INFO **" << '\n';
	std::cout << "\t** END **" << '\n';
}

void TM2P5DJsonParser::debugLogOfAtlasInfo(AtlasInfo* info)
{
	std::cout << "\t** DEBUG LOG OF ATLAS-INFO **" << '\n';
	std::cout << "\t** END **" << '\n';
}

/**
 * protected
 */
TM2P5DJsonParser::TM2P5DJsonParser()
:mMapInfo(nullptr)
{}

TM2P5DJsonParser::~TM2P5DJsonParser()
{

}

bool TM2P5DJsonParser::init()
{
	return true;
}

/**
 * private
 */
void parseJson(std::string json)
{

}
