#include "TM2P5DJsonParser.h"
#include "TM2P5DProperty.h"
#include <iostream>
#include <fstream>
#include <cassert>

USING_NS_CC;
using namespace TM2P5DComponent;

//The following constant values are only used in TM2P5DJsonParser class
namespace {
	//Directory names
	static const std::string DIR_NAME_TM2P5D("tm2p5d/");
	static const std::string DIR_NAME_ATLAS("atlas/");
	static const std::string DIR_NAME_TERRAIN("terrain/");

	//Identifiers that indicate what is info-object or 'include'
	static const std::string IDENTIFIER_MAP_INFO("map_info");
	static const std::string IDENTIFIER_LAYER_INFO("layer_info");
	static const std::string IDENTIFIER_LAYER_BUNDLER_INFO("layer_bundler_info");
	static const std::string IDENTIFIER_ATLAS_INFO("atlas_info");
	static const std::string IDENTIFIER_INCLUDE("include");

	//Keys
	static const std::string KEY_CHANK_WIDTH("chank_width");
	static const std::string KEY_CHANK_HEIGHT("chank_height");
	static const std::string KEY_NUM_OF_CHANK("num_of_chank");
	static const std::string KEY_ORIENTATION("orientation");
	static const std::string KEY_TILE_SIZE("tile_size");
	static const std::string KEY_ARCHITECTURE("architecture");
	static const std::string KEY_LAYER_NAME("layer_name");
	static const std::string KEY_ATLAS_NAME("atlas_name");
	static const std::string KEY_TERRAIN_SRC("terrain_src");
	static const std::string KEY_VISIBLE("visible");
	static const std::string KEY_EDITABLE("editable");
}

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
	picojson::value root = this->parseJson(path);

	if(!isError())
	{
		//parse other information files
	}
	else
	{
		this->outputLastError();
	}
}

bool TM2P5DJsonParser::isError()
{
	return !picojson::get_last_error().empty();
}

void TM2P5DJsonParser::outputLastError()
{
	if(this->isError())
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
picojson::value TM2P5DJsonParser::parseJson(std::string json)
{
	picojson::value root;
	std::ifstream fs(json.c_str(),std::ios::binary);

	//check error
	assert(fs);
	//parse .json
	fs >> root;
	fs.close();
	//check parse error
	if(isError())
		return std::move(root);

	auto itr_end = root.get<picojson::object>().end();
	for(auto itr = root.get<picojson::object>().begin(); itr != itr_end; ++itr)
	{
		if(itr->first == IDENTIFIER_MAP_INFO)
			mMapInfo = this->comvJsonToMapInfo(root);

		else if(itr->first == IDENTIFIER_LAYER_INFO)
		{
			// auto info = this->comvJsonToLayerInfo(root);
			// mLayerInfoMap.insert(info->get
		}
	}

	return std::move(root);
}

MapInfo* TM2P5DJsonParser::comvJsonToMapInfo(picojson::value& root)
{
	auto info = MapInfo::create();
	auto& elements = root.get<picojson::object>()[IDENTIFIER_MAP_INFO].get<picojson::object>();

	info->mChankWidth = static_cast<size_t>(elements[KEY_CHANK_WIDTH].get<double>());
	info->mChankHeight = static_cast<size_t>(elements[KEY_CHANK_HEIGHT].get<double>());
	info->mNumOfChank = static_cast<size_t>(elements[KEY_NUM_OF_CHANK].get<double>());
	info->mOrientation = this->comvJsonValueToOrientation(elements[KEY_ORIENTATION]);
	info->mTileSize = this->comvJsonValueToCcsize(elements[KEY_TILE_SIZE]);

	for(auto& value : elements[KEY_ARCHITECTURE].get<picojson::array>())
		info->mArchitecture.push_back(value.get<std::string>());

	MapInfo::debugLog(info);

	return info;
}

LayerInfo* TM2P5DJsonParser::comvJsonToLayerInfo(picojson::value& root)
{
	return nullptr;
}

LayerBundlerInfo* TM2P5DJsonParser::comvJsonToLayerBundlerInfo(picojson::value& root)
{
	return nullptr;
}

AtlasInfo* TM2P5DJsonParser::comvJsonToAtlasInfo(picojson::value& root)
{
	return nullptr;
}

Orientation::Enum TM2P5DJsonParser::comvJsonValueToOrientation(picojson::value& value)
{
	if(value.get<std::string>() == "portrait")
		return Orientation::Enum::PORTRAIT;
	else
		return Orientation::Enum::LANDSCAPE;
}

Size TM2P5DJsonParser::comvJsonValueToCcsize(picojson::value& value)
{
	auto& array = value.get<picojson::array>();
	auto size = Size(0,0);
	if(array.size() < 2)
		return size;

	size.width = static_cast<float>(array[0].get<double>());
	size.height = static_cast<float>(array[1].get<double>());

	return size;
}

Rect TM2P5DJsonParser::comvJsonValueToCcRect(picojson::value& value)
{
	auto& array = value.get<picojson::array>();
	auto rect = Rect(0,0,0,0);
	if(array.size() < 4)
		return rect;

	rect.origin.x = static_cast<float>(array[0].get<double>());
	rect.origin.y = static_cast<float>(array[1].get<double>());
	rect.size.width = static_cast<float>(array[2].get<double>());
	rect.size.height = static_cast<float>(array[3].get<double>());

	return rect;
}
