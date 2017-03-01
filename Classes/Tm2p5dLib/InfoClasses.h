#ifndef INFO_CLASSES_H
#define INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"
#include <vector>
#include <string>

/**
 * MapInfo, LayerInfo, LayerBundlerInfo, AtlasInfo class are members of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

//Forward declaration
enum class Orientation;

/**
 * Map information class
 */
class MapInfo : public cocos2d::Ref
{
public:
	/**
	 * Width and height of a chank.
	 */
	size_t mChankWidth;
	size_t mChankHeight;

	/**
	 * The number of chanks in a terrain.
	 */
	size_t mNumOfChank;

	/**
	 * The orientation of a map.
	 */
	Orientation mOrientation;

	/**
	 * The size of a texture of a tile (px).
	 */
	cocos2d::Size mTileSize;

	/**
	 * The names of TiledLayerBundler which is contained by a map.
	 */
	std::vector<std::string> mArchitecture;

	static MapInfo* create();
	static void debugLog(MapInfo* info);

	/**
	 * Getter functions
	 */
	size_t getChankWidth();
	size_t getChankHeight();
	size_t getNumOfChank();
	Orientation getOrientation();
	cocos2d::Size getTileSize();
	std::vector<std::string>& getArchitecture();

protected:
	MapInfo();
	~MapInfo();
	bool init();
};

/**
 * Tiled Layer information class
 */
class LayerInfo : public cocos2d::Ref
{
public:
	static LayerInfo* create();
	static void debugLog(LayerInfo* info);

protected:
	LayerInfo();
	~LayerInfo();
	bool init();
};

/**
 * Tiled Layer Bundler information class
 */
class LayerBundlerInfo : public cocos2d::Ref
{
public:
	static LayerBundlerInfo* create();
	static void debugLog(LayerBundlerInfo* info);

protected:
	LayerBundlerInfo();
	~LayerBundlerInfo();
	bool init();
};

/**
 * Tile Atlas information class
 */
class AtlasInfo : public cocos2d::Ref
{
public:
	static AtlasInfo* create();
	static void debugLog(AtlasInfo* info);

protected:
	AtlasInfo();
	~AtlasInfo();
	bool init();
};

} /* namespace TM2P5DComponent */

#endif
