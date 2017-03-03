#ifndef TILED_LAYER_H
#define TILED_LAYER_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"
#include <string>
#include <vector>

/**
 * TiledLayer class is a member of the namespace TM2P5DComponent.
 */
namespace TM2P5DComponent {

//Forward declarations
class Chank;
class MapInfo;
class LayerInfo;
class LayerBundlerInfo;
class AtlasInfo;

class TiledLayer : public cocos2d::Node
{
public:
	/**
	 * Create TiledLayer object.
	 * @param  mapInfo   [Map information]
	 * @param  layerInfo [Layer information]
	 * @param  atlasInfo [tile atlas information]
	 * @param  capacity  [the number of chanks that whill be staged in this class]
	 * @param  zolder    [An z-older of this layer in a tiled-layer-bundler]
	 * @return           [TiledLayer*]
	 */

	static TiledLayer* create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder);

protected:
	TiledLayer();
	~TiledLayer();

	/**
	 * Initialize TiledLayer object.
	 * @param  layerInfo [Layer information]
	 * @param  atlasInfo [tile atlas information]
	 * @param  capacity  [the number of chanks that whill be staged in this class]
	 * @param  zolder    [An z-older of this layer in a tiled-layer-bundler]
	 * @return           [description]
	 */
	bool initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder);

private:
	/**
	 * Add tile sprites to this node.
	 */
	CC_SYNTHESIZE_RETAIN(SpriteBatchNode*,mBatchNode,BatchNode);

	/**
	 * Z-older of this layer.
	 */
	CC_SYNTHESIZE(int,mZolder,Zolder);

	/**
	 * The number of staged chanks.
	 */
	size_t mCapacity;

	/**
	 * The number of all chanks in a layer.
	 */
	size_t mNumOfChank;

	/**
	 * The size of a chank.
	 */
	size_t mChankWidth;
	size_t mChankHeight;

	/**
	 * The number of kinds of tile tpyes.
	 */
	size_t mNumOfTileTypes;

	/**
	 * The number of sub-chanks.
	 */
	int mNumOfSubChank;

	/**
	 * An index of a current active sub-chank.
	 */
	int mIndexOfActiveSubChank;

	/**
	 * The name of this layer.
	 */
	std::string mLayerName;

	/**
	 * The name of terrain data file.
	 */
	std::string mTerrainSrc;

	/**
	 * The name of a tile atlas file.
	 */
	std::string mAtlasSrc;

	/**
	 * Is a layer visible or not.
	 */
	bool mIsVisible;

	/**
	 * Is the terrain of alayer editable or not.
	 */
	bool mIsEditable;

	/**
	 * The orientation of a map.
	 */
	Orientation::Type mOrientation;

	/**
	 * Texture rects of tiles.
	 */
	std::vector<cocos2d::Rect> mTextureRects;

	/**
	 * An vector that contains staged chanks.
	 */
	cocos2d::Vector<Chank*> mChanks;
};

} /* namespace TM2P5DComponent */

#endif
