#ifndef DLIB_CC_LTS_MAP_INFO_CLASSES_H
#define DLIB_CC_LTS_MAP_INFO_CLASSES_H

#include "../tools/include_lib_index.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace lts_map
{
	namespace unit
	{
		struct MapInfo;
		struct LayerInfo;
		struct BundlerInfo;
		struct AtlasInfo;
		struct TerrainInfo;

		// for debuging
		void DebugLog(const MapInfo* info);
		void DebugLog(const LayerInfo* info);
		void DebugLog(const BundlerInfo* info);
		void DebugLog(const AtlasInfo* info);
		void DebugLog(const TerrainInfo* info);
	}
}

struct lts_map::unit::MapInfo
{
public:
	dlib::size<size_t> map_size;
	std::vector<std::string> architecture;

	MapInfo()
	:map_size(0, 0)
	{}

	inline bool Valied() const
	{
		return true;
	}
};

struct lts_map::unit::LayerInfo
{
public:
	std::string layer_name;
	std::string atlas_name;
	std::string terrain_name;
	bool is_visible;
	bool is_editable;
	float tile_scale;

	LayerInfo()
	:layer_name("")
	,atlas_name("")
	,terrain_name("")
	,is_visible(false)
	,is_editable(false)
	,tile_scale(1.0)
	{}

	inline bool Valied() const
	{
		return true;
	}
};

struct lts_map::unit::BundlerInfo
{
public:
	std::string bundler_name;
	std::vector<std::string> architecture;

	BundlerInfo()
	:bundler_name("")
	{}

	inline bool Valied() const
	{
		return true;
	}
};

struct lts_map::unit::AtlasInfo
{
public:
	std::string atlas_name;
	std::string atlas_src_name;
	dlib::size<float> texture_size;
	int num_tile_type;
	int tile_type_no_tile;
	std::vector<dlib::vec2<float>> texture_positions;

	AtlasInfo()
	:atlas_name("")
	,atlas_src_name("")
	,texture_size(0.0, 0.0)
	,num_tile_type(0)
	,tile_type_no_tile(-1)
	{}

	inline bool Valied() const
	{
		return true;
	}
};

struct lts_map::unit::TerrainInfo
{
public:
	std::string terrain_name;
	std::string terrain_src_name;
	std::string atlas_name;
	dlib::size<size_t> map_size;
	dlib::size<size_t> block_size;
	std::unordered_map<std::string, dlib::vec2<int>> location_pin_map;

	TerrainInfo()
	:terrain_name("")
	,terrain_src_name("")
	,atlas_name("")
	,map_size(0, 0)
	,block_size(0, 0)
	{}

	inline bool Valied() const
	{
		return true;
	}
};

inline void lts_map::unit::DebugLog(const lts_map::unit::MapInfo *info)
{
	std::cout << '\n' << "DEBUG LOG < MAP INFO >" << '\n';
	std::cout << "map size => " << info->map_size << '\n';
	for(auto name : info->architecture)
		std::cout << "architecture => " << name << '\n';
	std::cout << "END < MAP INFO >" << "\n\n";
}

inline void lts_map::unit::DebugLog(const lts_map::unit::LayerInfo *info)
{
	std::cout << '\n' << "DEBUG LOG < LAYER INFO >" << '\n';
	std::cout << "layer name => " << info->layer_name << '\n';
	std::cout << "atlas name => " << info->atlas_name << '\n';
	std::cout << "terrain name => " << info->terrain_name << '\n';
	std::cout << "is visible => " << info->is_visible << '\n';
	std::cout << "is editable => " << info->is_editable << '\n';
	std::cout << "END < LAYER INFO >" << "\n\n";
}

inline void lts_map::unit::DebugLog(const lts_map::unit::BundlerInfo *info)
{
	std::cout << '\n' << "DEBUG LOG < BUNDLER INFO >" << '\n';
	std::cout << "bundler name => " << info->bundler_name << '\n';
	for(auto name : info->architecture)
		std::cout << "architecture => " << name << '\n';
	std::cout << "END < BUNDLER INFO >" << "\n\n";
}

inline void lts_map::unit::DebugLog(const lts_map::unit::AtlasInfo *info)
{
	std::cout << '\n' << "DEBUG LOG < ATLAS INFO >" << '\n';
	std::cout << "atlas name => " << info->atlas_name << '\n';
	std::cout << "atlas src name => " << info->atlas_src_name << '\n';
	std::cout << "texture size => " << info->texture_size << '\n';
	std::cout << "num tile type => " << info->num_tile_type << '\n';
	std::cout << "tile type no tile => " << info->tile_type_no_tile << '\n';
	for(auto vec : info->texture_positions)
		std::cout << "texture positions => " << vec << '\n';
	std::cout << "END < ATLAS INFO >" << "\n\n";
}

inline void lts_map::unit::DebugLog(const lts_map::unit::TerrainInfo *info)
{
	std::cout << '\n' << "DEBUG LOG < TERRAIN INFO >" << '\n';
	std::cout << "terrain name => " << info->terrain_name << '\n';
	std::cout << "terrain src name => " << info->terrain_src_name << '\n';
	std::cout << "atlas name => " << info->atlas_name << '\n';
	std::cout << "map size => " << info->map_size << '\n';
	std::cout << "block size => " << info->block_size << '\n';
	for(auto itr = info->location_pin_map.begin(); itr != info->location_pin_map.end(); ++itr)
		std::cout << "location pin map => key::" << itr->first << " pos:: " << itr->second << '\n';
	std::cout << "END < TERRAIN INFO >" << "\n\n";
}

#endif
