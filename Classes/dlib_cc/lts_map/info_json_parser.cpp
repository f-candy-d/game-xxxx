#include "info_json_parser.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace cocos2d;
using namespace dlib_cc::lts_map_unit;

// constant variables
namespace
{
	//Identifiers that indicate what is info-object or 'include'
	static const std::string kIdentifierMapInfo("map_info");
	static const std::string kIdentifierLayerInfo("layer_info");
	static const std::string kIdentifierBundlerInfo("bundler_info");
	static const std::string kIdentifierAtlasInfo("atlas_info");
	static const std::string kIdentifierTerrainInfo("terrain_info");
	static const std::string kIdentifierFileIndex("file_index");

	//Keys of elements
	static const std::string kElmArchitecture("architecture");
	static const std::string kElmBundlerName("bundler_name");
	static const std::string kElmLayerName("layer_name");
	static const std::string kElmAtlasName("atlas_name");
	static const std::string kElmTerrainName("terrain_name");
	static const std::string kElmAtlasSrcName("atlas_src_name");
	static const std::string kElmTerrainSrcName("terrain_src_name");
	static const std::string kElmIsVisible("is_visible");
	static const std::string kElmIsEditable("is_editable");
	static const std::string kElmNumTileType("num_tile_type");
	static const std::string kElmTileTypeNoTile("tile_type_no_tile");
	static const std::string kElmTextureSize("texture_size");
	static const std::string kElmMapSize("map_size");
	static const std::string kElmBlockSize("block_size");
	static const std::string kElmTexturePositions("texture_positions");
	static const std::string kElmLocationPinMap("location_pin_map");
}

/**
 * public
 */
InfoJsonParser* InfoJsonParser::Create()
{
	auto ret = new InfoJsonParser();
	if(ret->Init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

bool InfoJsonParser::ParseJson(std::string json)
{
	picojson::value root;
	bool is_success = true;
	std::string path = FileUtils::getInstance()->fullPathForFilename(kMapResourceRootDirectory + json);
	std::ifstream fs(path.c_str(),std::ios::binary);

	if(!fs)
	{
		std::cout << "could't open the file => " << json << '\n';
		return false;
	}

	std::cout << "parse " << path << '\n';

	//parse .json
	fs >> root;
	fs.close();
	//check parse error
	if(!picojson::get_last_error().empty())
	{
		std::cout << "picojson : error => " << picojson::get_last_error() << " in " << path << '\n';
		//clear last error
		picojson::set_last_error("");
		// failed parsing of json
		return false;
	}

	for(auto itr = root.get<picojson::object>().begin(); itr != root.get<picojson::object>().end(); ++itr)
	{
		if(itr->first == kIdentifierMapInfo)
		{
			map_info_ = MakeMapInfo(itr->second);
		}
		else if(itr->first == kIdentifierLayerInfo)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = MakeLayerInfo(value);
				layer_info_map_[info.layer_name] = std::move(info);
			}
		}
		else if(itr->first == kIdentifierBundlerInfo)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = MakeBundlerInfo(value);
				bundler_info_map_[info.bundler_name] = std::move(info);
			}
		}
		else if(itr->first == kIdentifierAtlasInfo)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = MakeAtlasInfo(value);
				atlas_info_map_[info.atlas_name] = std::move(info);
			}
		}
		else if(itr->first == kIdentifierTerrainInfo)
		{
			for(auto& value : itr->second.get<picojson::array>())
			{
				auto info = MakeTerrainInfo(value);
				terrain_info_map_[info.terrain_name] = std::move(info);
			}
		}
		else if(itr->first == kIdentifierFileIndex)
		{
			//Parse other information files
			for(auto& value : itr->second.get<picojson::array>())
			{
				bool result = ParseJson(value.get<std::string>());
				// if there are errors in one or more files, is_success is to be false
				is_success = is_success && result;
			}
		}
	}

	return is_success;
}

/**
 * protected
 */
bool InfoJsonParser::Init()
{
	return true;
}

/**
 * private
 */
MapInfo InfoJsonParser::MakeMapInfo(picojson::value& obj)
{
	MapInfo info;
	auto& elements = obj.get<picojson::object>();

	info.map_size = MakeDLibSize<size_t>(elements[kElmMapSize]);
	for(auto& value : elements[kElmArchitecture].get<picojson::array>())
		info.architecture.push_back(value.get<std::string>());

	return std::move(info);
}

LayerInfo InfoJsonParser::MakeLayerInfo(picojson::value& obj)
{
	LayerInfo info;
	auto& elements = obj.get<picojson::object>();

	info.layer_name = elements[kElmLayerName].get<std::string>();
	info.atlas_name = elements[kElmAtlasName].get<std::string>();
	info.terrain_name = elements[kElmTerrainName].get<std::string>();
	info.is_visible = elements[kElmIsVisible].get<bool>();
	info.is_editable = elements[kElmIsEditable].get<bool>();

	return std::move(info);
}

BundlerInfo InfoJsonParser::MakeBundlerInfo(picojson::value& obj)
{
	BundlerInfo info;
	auto& elements = obj.get<picojson::object>();

	info.bundler_name = elements[kElmBundlerName].get<std::string>();
	for(auto& value : elements[kElmArchitecture].get<picojson::array>())
		info.architecture.push_back(value.get<std::string>());

	return std::move(info);
}

AtlasInfo InfoJsonParser::MakeAtlasInfo(picojson::value& obj)
{
	AtlasInfo info;
	auto& elements = obj.get<picojson::object>();

	info.atlas_name = elements[kElmAtlasName].get<std::string>();
	info.atlas_src_name = elements[kElmAtlasSrcName].get<std::string>();
	info.texture_size = MakeDLibSize<float>(elements[kElmTextureSize]);
	info.num_tile_type = static_cast<int>(elements[kElmNumTileType].get<double>());
	info.tile_type_no_tile = static_cast<int>(elements[kElmTileTypeNoTile].get<double>());
	for(auto& value : elements[kElmTexturePositions].get<picojson::array>())
	{
		info.texture_positions.push_back(MakeDLibVec2<float>(value));
	}

	return std::move(info);
}

TerrainInfo InfoJsonParser::MakeTerrainInfo(picojson::value& obj)
{
	TerrainInfo info;
	auto& elements = obj.get<picojson::object>();

	info.terrain_name = elements[kElmTerrainName].get<std::string>();
	info.terrain_src_name = elements[kElmTerrainSrcName].get<std::string>();
	info.atlas_name = elements[kElmAtlasName].get<std::string>();
	info.map_size = MakeDLibSize<size_t>(elements[kElmMapSize]);
	info.block_size = MakeDLibSize<size_t>(elements[kElmBlockSize]);
	for(auto itr = elements[kElmLocationPinMap].get<picojson::object>().begin(); itr != elements[kElmLocationPinMap].get<picojson::object>().end(); ++itr)
	{
		info.location_pin_map[itr->first] = MakeDLibVec2<int>(itr->second);
	}

	return std::move(info);
}
