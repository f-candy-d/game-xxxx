#ifndef TM2D_W_D_LIB_INFO_CLASSES_H
#define TM2D_W_D_LIB_INFO_CLASSES_H

#include "../../../cocos2d/cocos/cocos2d.h"
#include "../../dlib/size.h"
#include <string>
#include <unordered_map>

namespace lts_map
{
	namespace unit
	{
		struct MapInfo;
		struct LayerInfo;
		struct BundlerInfo;
		struct AtlasInfo;
		struct TerrainInfo;
	}
}

struct lts_map::unit::MapInfo
{
	dlib::size<size_t> size;
	std::vector<std::string> architecture;

	MapInfo()
	:size(0, 0)
	{}
};

struct lts_map::unit::LayerInfo
{
	std::string layer_name;
	std::string atlas_name;
	std::string terrain_name;
	bool is_visible;
	bool is_editable;

	LayerInfo()
	:layer_name("")
	,atlas_name("")
	,terrain_name("")
	,is_visible(false)
	,is_editable(false)
	{}
};

struct lts_map::unit::BundlerInfo
{
	std::string bundler_name;
	std::vector<std::string> architecture;

	BundlerInfo()
	:bundler_name("")
	{}
};

struct lts_map::unit::AtlasInfo
{
	std::string atlas_name;
	std::string atlas_src_name;
	dlib::size<float> texture_size;
	int num_tile_type;
	int tile_type_no_tile;
	std::unordered_map<int, dlib::vec2<float>> texture_position_map;

	AtlasInfo()
	:atlas_name("")
	,atlas_src_name("")
	,texture_size(0.0, 0.0)
	,num_tile_type(0)
	,tile_type_no_tile(-1)
	{}
};

struct lts_map::unit::TerrainInfo
{
	std::string terrain_name;
	std::string terrain_src_name;
	std::string atlas_name;
	dlib::size<size_t> size;
	dlib::size<size_t> block_size;
	std::unordered_map<std::string, dlib::vec2<int>> location_pin_map;

	TerrainInfo()
	:terrain_name("")
	,terrain_src_name("")
	,atlas_name("")
	,size(0, 0)
	,block_size(0, 0)
	{}
};

#endif
