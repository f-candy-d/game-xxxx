#ifndef DLIB_CC_LTS_MAP_CONFIG_H
#define DLIB_CC_LTS_MAP_CONFIG_H

#include <string>

namespace lts_map
{
	namespace unit
	{
		// file pathes to directories which has resources of a stl-map
		const std::string kMapResourceRootDirectory("lts_map_res/");
		const std::string kMapAtlasDirectory(kMapResourceRootDirectory + "atlas/");
		const std::string kMapTerrainDirectory(kMapResourceRootDirectory + "terrain/");

		// minimum map size
		constexpr size_t kMinMapWidth = 3;
		constexpr size_t kMinMapHeight = 3;
		constexpr size_t kMinMapSize = kMinMapWidth * kMinMapHeight;

		// for Block
		constexpr int kDefaultTileTypeNoTile = -1;
	}
}

#endif
