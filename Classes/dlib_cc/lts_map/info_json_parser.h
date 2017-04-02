#ifndef DLIB_CC_LTS_MAP_INFO_JSON_PARSER_H
#define DLIB_CC_LTS_MAP_INFO_JSON_PARSER_H

#include "../include_lib_index.h"
#include "picojson.h"
#include "info_classes.h"
#include <unordered_map>
#include <vector>

namespace lts_map
{
	namespace unit
	{
		class InfoJsonParser;
	}
}

class lts_map::unit::InfoJsonParser : public cocos2d::Ref
{
public:
	static InfoJsonParser* Create();
	bool ParseJson(std::string json);
	// accessors and mutators
	inline const MapInfo* map_info() const { return &map_info_; }
	inline const LayerInfo* layer_info(std::string& name) const { return &layer_info_map_.at(name); }
	inline const BundlerInfo* bundler_info(std::string& name) const {return &bundler_info_map_.at(name); }
	inline const AtlasInfo* atlas_info(std::string& name) const { return &atlas_info_map_.at(name); }
	inline const TerrainInfo* terrain_info(std::string& name) const { return &terrain_info_map_.at(name); }

protected:
	bool Init();

private:
	MapInfo map_info_;
	std::unordered_map<std::string, LayerInfo> layer_info_map_;
	std::unordered_map<std::string, BundlerInfo> bundler_info_map_;
	std::unordered_map<std::string, AtlasInfo> atlas_info_map_;
	std::unordered_map<std::string, TerrainInfo> terrain_info_map_;
	std::vector<std::string> error_pool_;

	MapInfo MakeMapInfo(picojson::value& obj);
	LayerInfo MakeLayerInfo(picojson::value& obj);
	BundlerInfo MakeBundlerInfo(picojson::value& obj);
	AtlasInfo MakeAtlasInfo(picojson::value& obj);
	TerrainInfo MakeTerrainInfo(picojson::value& obj);

	template <typename T>
	dlib::size<T> MakeDLibSize(picojson::value& value)
	{
		auto& array = value.get<picojson::array>();
		dlib::size<T> size;
		size.width = static_cast<T>(array[0].get<double>());
		size.height = static_cast<T>(array[1].get<double>());

		return size;
	}

	template <typename T>
	dlib::vec2<T> MakeDLibVec2(picojson::value& value)
	{
		auto& array = value.get<picojson::array>();
		dlib::vec2<T> vec;
		vec.x = static_cast<T>(array[0].get<double>());
		vec.y = static_cast<T>(array[1].get<double>());

		return vec;
	}
};

#endif
