#ifndef TM2D_W_D_LIB_INFO_CLASSES_H
#define TM2D_W_D_LIB_INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "../DLib/dlib_size.h"
#include <string>
#include <unordered_map>

namespace lts_map
{
	namespace lts_map_unit
	{
		struct MapInfo;
		struct LayerInfo;
		struct BundlerInfo;
		struct AtlasInfo;
		struct TerrainInfo;
	}
}

struct lts_map::lts_map_unit::MapInfo
{
	DLib::size<size_t> size_;
	std::vector<std::string> architecture_;

	MapInfo()
	:size_(0, 0)
	{}
};

struct lts_map::lts_map_unit::LayerInfo
{
	std::string layer_name_;
	std::string atlas_name_;
	std::string terrain_name_;
	bool is_visible_;
	bool is_editable_;

	LayerInfo()
	:layer_name_("")
	,atlas_name_("")
	,terrain_name_("")
	,is_visible_(false)
	,is_editable_(false)
	{}
};

struct lts_map::lts_map_unit::BundlerInfo
{
	std::string bundler_name_;
	std::vector<std::string> architecture_;

	BundlerInfo()
	:bundler_name_("")
	{}
};

struct lts_map::lts_map_unit::AtlasInfo
{
	std::string atlas_name_;
	std::string atlas_src_name_;
	DLib::size<float> texture_size_;
	int num_tile_type_;
	int tile_type_no_tile_;
	std::unordered_map<int, DLib::vec2<float>> texture_position_map_;

	AtlasInfo()
	:atlas_name_("")
	,atlas_src_name_("")
	,texture_size_(0.0, 0.0)
	,num_tile_type_(0)
	,tile_type_no_tile_(-1)
	{}
};

struct lts_map::lts_map_unit::TerrainInfo
{
	std::string terrain_name_;
	std::string terrain_src_name_;
	std::string atlas_name_;
	DLib::size<size_t> size_;
	DLib::size<size_t> block_size_;
	std::unordered_map<std::string, DLib::vec2<int>> location_pin_map_;

	TerrainInfo()
	:terrain_name_("")
	,terrain_src_name_("")
	,atlas_name_("")
	,size_(0, 0)
	,block_size_(0, 0)
	{}
};

#endif
