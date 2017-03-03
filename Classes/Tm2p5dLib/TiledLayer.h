#ifndef TILED_LAYER_H
#define TILED_LAYER_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"
#include "Chank.h"
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

	static TiledLayer* create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize);

	/**
	 * This function will be called when a visible rect changed.
	 * @method onVisibleRectChanged
	 * @param  visibleRect          [description]
	 */
	void onVisibleRectChanged(cocos2d::Rect visibleRect);

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
	bool initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize);

private:
	/**
	 * Add tile sprites to this node.
	 */
	CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode*,mBatchNode,BatchNode);

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
	int mNumOfTileType;

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

	/**
	 * Stage new chanks.Save old terrain data and load new one.
	 * @method stageNewChank
	 * @param  num           [the number of chanks that will be staged]
	 * @param  direction     [the direction of loading terrain data]
	 */
	void stageNewChank(size_t num,LoadDirection direction);

	/**
	 * Load and Save terrain data to a terrain file.
	 * @method loadTerrain
	 * @method saveTerrain
	 * @param  chank       [chank object]
	 */
	void loadTerrain(Chank* chank);
	void saveTerrain(Chank* chank);

	/**
	 * Save all terrain date of chanks that is staged.
	 * @method saveAllTerrainOfChankStaged
	 */
	void saveAllTerrainOfChankStaged();

	/**
	 * Make or move sprites for displayed sub-chanks.
	 * @method allocateSpriteToChank
	 * @param  chank                 [chank object]
	 */
	void allocateSpriteToChank(Chank* chank);
};

} /* namespace TM2P5DComponent */

#endif
