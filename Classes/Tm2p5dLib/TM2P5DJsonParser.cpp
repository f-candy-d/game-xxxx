#include "TM2P5DJsonParser.h"
#include "TM2P5DProperty.h"
#include <iostream>
#include <fstream>
#include <cassert>

USING_NS_CC;
using namespace TM2P5DComponent;

//The following constant values are only used in TM2P5DJsonParser class
namespace {

	//Identifiers that indicate what is info-object or 'include'
	static const std::string IDENTIFIER_MAP_INFO("map_info");
	static const std::string IDENTIFIER_LAYER_INFO("layer_info");
	static const std::string IDENTIFIER_LAYER_BUNDLER_INFO("layer_bundler_info");
	static const std::string IDENTIFIER_ATLAS_INFO("atlas_info");
	static const std::string IDENTIFIER_INCLUDE("include");

	//Keys
	static const std::string KEY_CHANK_WIDTH("pane_width");
	static const std::string KEY_CHANK_HEIGHT("pane_height");
	static const std::string KEY_NUM_OF_CHANK("num_of_pane");
	static const std::string KEY_TILE_SIZE("tile_size");
	static const std::string KEY_ARCHITECTURE("architecture");
	static const std::string KEY_LAYER_NAME("layer_name");
	static const std::string KEY_ATLAS_NAME("atlas_name");
	static const std::string KEY_TERRAIN_SRC("terrain_src");
	static const std::string KEY_VISIBLE("visible");
	static const std::string KEY_EDITABLE("editable");
	static const std::string KEY_ATLAS_SRC("atlas_src");
	static const std::string KEY_NUM_OF_TILE_TYPE("num_of_tile_type");
	static const std::string KEY_TEXTURE_RECT("texture_rect");
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

	//error check
	if(!this->isError())
	{
		//Parse other information files
		for(auto& value : root.get<picojson::object>()[IDENTIFIER_INCLUDE].get<picojson::array>())
		{
			std::string json = FileUtils::getInstance()->fullPathForFilename(DIR_NAME_TM2P5D + value.get<std::string>());
			parseJson(json);
		}
	}
}

bool TM2P5DJsonParser::isError()
{
	return mErrorPool.size();
}

void TM2P5DJsonParser::outputLastError()
{
	if(this->isError())
		std::cout << "picojson : error => " << mErrorPool.back() << '\n';
	else
		std::cout << "TM2P5DJsonParser : mess => There are no errors for now" << '\n';
}

void TM2P5DJsonParser::outputErrors()
{
	if(this->isError())
	{
		for(auto error : mErrorPool)
			std::cout << "picojson : error => " << error << '\n';
	}
	else
	{
		std::cout << "picojson : mess =>  there are no errors for now" << '\n';
	}
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
	if(!picojson::get_last_error().empty())
	{
		std::string err_mes(picojson::get_last_error() + " in " + json);
		mErrorPool.push_back(err_mes);
		//clear last error
		picojson::set_last_error("");
		return std::move(root);
	}

	std::cout << "parse " << json << '\n';

	auto itr_end = root.get<picojson::object>().end();
	for(auto itr = root.get<picojson::object>().begin(); itr != itr_end; ++itr)
	{
		if(itr->first == IDENTIFIER_MAP_INFO)
			mMapInfo = this->comvJsonToMapInfo(itr->second);

		else if(itr->first == IDENTIFIER_LAYER_INFO)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = this->comvJsonToLayerInfo(value);
				mLayerInfoMap.insert(info->getLayerName(),info);
			}
		}

		else if(itr->first == IDENTIFIER_LAYER_BUNDLER_INFO)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = this->comvJsonToLayerBundlerInfo(value);
				mLayerBundlerInfoMap.insert(info->getLayerName(),info);
			}
		}

		else if(itr->first == IDENTIFIER_ATLAS_INFO)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = this->comvJsonToAtlasInfo(value);
				mAtlasInfoMap.insert(info->getAtlasName(),info);
			}
		}
	}

	return std::move(root);
}

MapInfo* TM2P5DJsonParser::comvJsonToMapInfo(picojson::value& obj)
{
	auto info = MapInfo::create();
	auto& elements = obj.get<picojson::object>();

	info->mPaneWidth = static_cast<size_t>(elements[KEY_CHANK_WIDTH].get<double>());
	info->mPaneHeight = static_cast<size_t>(elements[KEY_CHANK_HEIGHT].get<double>());
	info->mNumOfPane = static_cast<size_t>(elements[KEY_NUM_OF_CHANK].get<double>());
	info->mTileSize = this->comvJsonValueToCcsize(elements[KEY_TILE_SIZE]);

	for(auto& value : elements[KEY_ARCHITECTURE].get<picojson::array>())
		info->mArchitecture.push_back(value.get<std::string>());

	// MapInfo::debugLog(info);

	return info;
}

LayerInfo* TM2P5DJsonParser::comvJsonToLayerInfo(picojson::value& obj)
{
	auto info = LayerInfo::create();
	auto& elements = obj.get<picojson::object>();

	info->mLayerName = elements[KEY_LAYER_NAME].get<std::string>();
	info->mAtlasName = elements[KEY_ATLAS_NAME].get<std::string>();
	info->mTerrainSource = elements[KEY_TERRAIN_SRC].get<std::string>();
	info->mIsVisible = elements[KEY_VISIBLE].get<bool>();
	info->mIsEditable = elements[KEY_EDITABLE].get<bool>();

	// LayerInfo::debugLog(info);

	return info;
}

LayerBundlerInfo* TM2P5DJsonParser::comvJsonToLayerBundlerInfo(picojson::value& obj)
{
	auto info = LayerBundlerInfo::create();
	auto& elements = obj.get<picojson::object>();

	info->mLayerName = elements[KEY_LAYER_NAME].get<std::string>();

	for(auto& value : elements[KEY_ARCHITECTURE].get<picojson::array>())
		info->mArchitecture.push_back(value.get<std::string>());

	// LayerBundlerInfo::debugLog(info);

	return info;
}

AtlasInfo* TM2P5DJsonParser::comvJsonToAtlasInfo(picojson::value& obj)
{
	auto info = AtlasInfo::create();
	auto& elements = obj.get<picojson::object>();

	info->mAtlasName = elements[KEY_ATLAS_NAME].get<std::string>();
	info->mAtlasSource = elements[KEY_ATLAS_SRC].get<std::string>();
	info->mNumOfTileType = static_cast<int>(elements[KEY_NUM_OF_TILE_TYPE].get<double>());

	for(auto& value : elements[KEY_TEXTURE_RECT].get<picojson::array>())
		info->mTextureRects.push_back(comvJsonValueToCcRect(value));

	// AtlasInfo::debugLog(info);

	return info;
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
