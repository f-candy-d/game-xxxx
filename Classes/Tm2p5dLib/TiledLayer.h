#ifndef TILED_LAYER_H
#define TILED_LAYER_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"
#include "Pane.h"
#include <string>
#include <vector>

/**
 * TiledLayer class is a member of the namespace TM2P5DComponent.
 */
namespace TM2P5DComponent {

//Forward declarations
class Pane;
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
	 * @param  capacity  [the number of panes that whill be staged in this class]
	 * @param  zolder    [An z-older of this layer in a tiled-layer-bundler]
	 * @return           [TiledLayer*]
	 */

	static TiledLayer* create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize);

	/**
	 * This function will be called when a visible rect changed.
	 * @method onOriginChanged
	 * @param  newOrigin          [A new origin point of a layer]
	 */
	void onOriginChanged(cocos2d::Vec2 newOrigin);

protected:
	TiledLayer();
	~TiledLayer();

	/**
	 * Initialize TiledLayer object.
	 * @param  layerInfo [Layer information]
	 * @param  atlasInfo [tile atlas information]
	 * @param  capacity  [the number of panes that whill be staged in this class]
	 * @param  zolder    [An z-older of this layer in a tiled-layer-bundler]
	 * @return           [description]
	 */
	bool initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize);

private:
	/**
	 * The number of sub-panes that will be drawn on the screen.
	 */
	static const int NUM_OF_DRAWN_SUB_CHANK = 3;

	/**
	 * Add tile sprites to this node.
	 */
	cocos2d::SpriteBatchNode* mBatchNode;

	/**
	 * Z-older of this layer.
	 */
	CC_SYNTHESIZE(int,mZolder,Zolder);

	/**
	 * The number of staged panes.
	 */
	size_t mCapacity;

	/**
	 * The number of all panes in a layer.
	 */
	size_t mNumOfPane;

	/**
	 * The size of a pane.
	 */
	size_t mPaneWidth;
	size_t mPaneHeight;

	/**
	 * An cursore that indicate a pane displayed on the center of the screen.
	 */
	size_t mCursoreOfCenterPane;

	/**
	 * The number of kinds of tile tpyes.
	 */
	int mNumOfTileType;

	/**
	 * The number of sub-panes.
	 */
	int mNumOfSubPane;

	/**
	 * An index of a current active sub-pane.
	 */
	int mIndexOfActiveSubPane;

	/**
	 * The name of this layer.
	 */
	std::string mLayerName;

	/**
	 * The full path to a terrain data file.
	 */
	std::string mTerrainSrc;

	/**
	 * The path to a tile atlas file.
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
	 * The size of a tile.
	 */
	cocos2d::Size mTileSize;

	/**
	 * Pool a delta of the origin movement of a layer.
	 */
	cocos2d::Vec2 mOriginPool;

	/**
	 * The orientation of a map.
	 */
	Orientation::Type mOrientation;

	/**
	 * Texture rects of tiles.
	 */
	std::vector<cocos2d::Rect> mTextureRects;

	/**
	 * An vector that contains staged panes.
	 */
	cocos2d::Vector<Pane*> mPanes;

	/**
	 * Stage new panes.Save old terrain data and load new one.
	 * @method stageNewPane
	 * @param  num           [the number of panes that will be staged]
	 * @param  direction     [the direction of loading terrain data]
	 * @return               [return true if new pane was staged,otherwise return false]
	 */
	bool stageNewPane(size_t num,LoadDirection direction);

	/**
	 * Load and Save terrain data to a terrain file.
	 * @method loadTerrain
	 * @method saveTerrain
	 * @param  pane       [pane object]
	 */
	void loadTerrain(Pane* pane);
	void saveTerrain(Pane* pane);

	/**
	 * Save all terrain date of panes that is staged.
	 * @method saveAllTerrainOfPaneStaged
	 */
	void saveAllTerrainOfPaneStaged();

	/**
	 * Make or move sprites for displayed sub-panes.
	 * @method allocateSpriteToPane
	 * @param  pane                 [pane object]
	 */
	void allocateSpriteToPane(Pane* pane);
};

} /* namespace TM2P5DComponent */

#endif
