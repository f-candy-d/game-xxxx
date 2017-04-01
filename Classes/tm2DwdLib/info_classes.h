#ifndef TM2D_W_D_LIB_INFO_CLASSES_H
#define TM2D_W_D_LIB_INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "../DLib/dlib_size.h"
#include <string>
#include <unordered_map>

namespace tm2d_w_d
{
	namespace tm2d_w_d_unit
	{
		struct MapInfo;
		struct LayerInfo;
		struct BundlerInfo;
		struct AtlasInfo;
		struct TerrainInfo;
	}
}

struct tm2d_w_d::tm2d_w_d_unit::MapInfo
{
	DLib::size<size_t> size_;
	std::vector<std::string> architecture_;

	MapInfo()
	:size_(0, 0)
	{}
};

struct tm2d_w_d::tm2d_w_d_unit::LayerInfo
{
	std::string layer_name_;
	std::string atlas_name_;
	std::string terrain_name_;
	bool is_visible_;
	bool is_editable_;

	LayerInfo()
	:layer_name_(NULL)
	,atlas_name_(NULL)
	,terrain_name_(NULL)
	,is_visible_(false)
	,is_editable_(true)
	{}
};

struct tm2d_w_d::tm2d_w_d_unit::BundlerInfo
{
	std::string bundler_name_;
	std::vector<std::string> architecture_;

	BundlerInfo()
	:bundler_name_(NULL)
	{}
};

struct tm2d_w_d::tm2d_w_d_unit::AtlasInfo
{
	std::string atlas_name_;
	std::string atlas_src_name_;
	DLib::size<float> texture_size_;
	int num_tile_type_;
	int tile_type_no_tile_;
	std::unordered_map<int, DLib::vec2<float>> texture_positions_;

	AtlasInfo()
	:atlas_name_(NULL)
	,atlas_src_name_(NULL)
	,texture_size_(0.0, 0.0)
	,num_tile_type_(0)
	,tile_type_no_tile_(-1)
	{}
};

struct tm2d_w_d::tm2d_w_d_unit::TerrainInfo
{
	std::string terrain_name_;
	std::string terrain_src_name_;
	std::string atlas_name_;
	DLib::size<size_t> size_;
	// DLib::size<size_t> pane_size;

	TerrainInfo()
	:terrain_name_(NULL)
	,terrain_src_name_(NULL)
	,atlas_name_(NULL)
	,size_(0, 0)
	{}
};

#endif
