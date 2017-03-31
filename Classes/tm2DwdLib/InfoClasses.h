#ifndef TM2D_W_D_LIB_INFO_CLASSES_H
#define TM2D_W_D_LIB_INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "../DLib/dlib_size.h"
#include <string>
#include <unordered_map>

namespace TM2DwD
{
	namespace TM2DwDUnit
	{
		struct MapInfo;
		struct LayerInfo;
		struct BundlerInfo;
		struct AtlasInfo;
		struct TerrainInfo;
	}
}

struct TM2DwD::TM2DwDUnit::MapInfo
{
	DLib::size<size_t> size;
	std::vector<std::string> architecture;

	MapInfo()
	:size(0, 0)
	{}
};

struct TM2DwD::TM2DwDUnit::LayerInfo
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
	,is_editable(true)
	{}
};

struct TM2DwD::TM2DwDUnit::BundlerInfo
{
	std::string bundler_name;
	std::vector<std::string> architecture;

	BundlerInfo()
	:bundler_name("")
	{}
};

struct TM2DwD::TM2DwDUnit::AtlasInfo
{
	std::string atlas_name;
	std::string atlas_src_name;
	DLib::size<float> texture_size;
	int num_tile_type;
	int tile_type_no_tile;
	std::unordered_map<int, DLib::vec2<float>> texture_positions;

	AtlasInfo()
	:atlas_name("")
	,atlas_src_name("")
	,texture_size(0.0, 0.0)
	,num_tile_type(0)
	,tile_type_no_tile(-1)
	{}
};

struct TM2DwD::TM2DwDUnit::TerrainInfo
{
	std::string terrain_name;
	std::string terrain_src_name;
	std::string atlas_name;
	DLib::size<size_t> size;
	// DLib::size<size_t> pane_size;

	TerrainInfo()
	:terrain_name("")
	,terrain_src_name("")
	,atlas_name("")
	,size(0, 0)
	{}
};

#endif
