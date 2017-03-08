#ifndef INFO_CLASSES_H
#define INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"
#include <vector>
#include <string>
/**
 * MapInfo, LayerInfo, LayerBundlerInfo, AtlasInfo class are members of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

/**
 * Map information class
 */
class MapInfo : public cocos2d::Ref
{
public:
	/**
	 * Width and height of a pane.
	 */
	size_t mPaneWidth;
	size_t mPaneHeight;

	/**
	 * The number of panes in a terrain.
	 */
	size_t mNumOfPane;

	/**
	 * The orientation of a map.
	 */
	Orientation::Type mOrientation;

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
	size_t getPaneWidth();
	size_t getPaneHeight();
	size_t getNumOfPane();
	Orientation::Type getOrientation();
	cocos2d::Size getTileSize();
	const std::vector<std::string>& getArchitecture();

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
	/**
	 * The name of a layer.
	 */
	std::string mLayerName;

	/**
	 * The name of a tile atlas which will be used in this layer.
	 */
	std::string mAtlasName;

	/**
	 * The name of a file that contains data of the terrain of a layer.
	 */
	std::string mTerrainSource;

	/**
	 * Is a layer visible or not.
	 */
	bool mIsVisible;

	/**
	 * Is a layer editable or not.
	 */
	bool mIsEditable;

	static LayerInfo* create();
	static void debugLog(LayerInfo* info);

	/**
	 * Getter functions
	 */
	std::string getLayerName();
	std::string getAtlasName();
	std::string getTerrainSource();
	bool isVisible();
	bool isEditable();

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
	/**
	 * The name of a layer-bundler.
	 */
	std::string mLayerName;

	/**
	 * The names of layers which are contained by this layer-bundler.
	 */
	std::vector<std::string> mArchitecture;

	static LayerBundlerInfo* create();
	static void debugLog(LayerBundlerInfo* info);

	/**
	 * Getter functions
	 */
	std::string getLayerName();
	const std::vector<std::string>& getArchitecture();

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
	/**
	 * The name of a tile atlas.
	 */
	std::string mAtlasName;

	/**
	 * The name of a file of tile atlas.
	 */
	std::string mAtlasSource;

	/**
	 * The number of kinds of tiles.
	 */
	int mNumOfTileType;

	/**
	 * Rects of a texture of a tile on a tile atlas.
	 */
	std::vector<cocos2d::Rect> mTextureRects;

	static AtlasInfo* create();
	static void debugLog(AtlasInfo* info);

	/**
	 * Getter functions
	 */
	std::string getAtlasName();
	std::string getAtlasSource();
	int getNumOfTileType();
	const std::vector<cocos2d::Rect>& getTextureRects();

protected:
	AtlasInfo();
	~AtlasInfo();
	bool init();
};

} /* namespace TM2P5DComponent */

#endif
