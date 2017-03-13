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
	// NOTE : #a : add on 10/3/2017 instead of create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize)
	/**
	 * @param  scale       [the ratio of the size of a tile sprite to that of a texture of a tile]
	 */
	static TiledLayer* create(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize,float scale);

	// NOTE : #a : add on 10/3/2017 instead of onOriginChanged(Vec2 newOrigin)
	/**
	 * [onDrawingRectChanged description]
	 * @param delta [the difference between a index of the old anchor-pane and that of new one]
	 */
	void onStageNewPane(int delta);

protected:
	TiledLayer();
	~TiledLayer();

	// NOTE : #a : add on 10/3/2017 instead of initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,Size visibleSize)
	/**
	 * @param  scale       [the ratio of the size of a tile sprite to that of a texture of a tile]
	 */
	bool initWithInfo(MapInfo* mapInfo,LayerInfo* layerInfo,AtlasInfo* atlasInfo,size_t capacity,int zolder,cocos2d::Size visibleSize,float scale);

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
	 * The size of a sub-pane.
	 */
	size_t mSubPaneWidth;
	size_t mSubPaneHeight;

	// NOTE : #a : add on 11/3/2017
	int mIndexOfAnchorPane;

	/**
	 * The number of kinds of tile tpyes.
	 */
	int mNumOfTileType;

	/**
	 * The number of sub-panes.
	 */
	int mNumOfSubPane;

	/**
	 * The number of sub-panes that will be drawn on the screen.
	 */
	int mNumOfSubPaneDrawn;

	/**
	 * An index of a current active sub-pane.
	 */
	int mIndexOfAnchorSubPane;

	/**
	 * The scale of a tile sprite.
	 */
	float mScale;

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

	// NOTE : #a : add on 10/3/2017 instead of mTileSize
	/**
	 *
	 * The size of a texture of a tile. (px)
	 */
	cocos2d::Size mTileTextureSize;

	// NOTE : #a : add on 11/3/2017
	cocos2d::Size mAbsoluteTileSize;

	/**
	 * Pool a delta of the origin movement of a layer.
	 */
	cocos2d::Vec2 mOriginPool;

	/**
	 * The split of a map.
	 */
	Split mSplit;

	/**
	 * Texture rects of tiles.
	 */
	std::vector<cocos2d::Rect> mTextureRects;

	/**
	 * An vector that contains staged panes.
	 */
	cocos2d::Vector<Pane*> mPanes;

	/**
	 * @param pitch  [0 ~ 1.0]
	 */
	void optimizeSplitOfPane(float pitch,int surplus,Split split,cocos2d::Size visibleSize);

	// NOTE : #a : add on 11/3/2017 instead of stageNewPane(size_t num,LoadDirection direction)
	/**
	 * NOTE : The default value of mIndexOfAnchorPane must be (-1 * mCapacity) .
	 * @param  newAnchor [an index of a anchor pane which will be staged]
	 * @param  oldAnchor [an index of a anchor pane that is already staged]
	 */
	bool stagePane(int newAnchor,int oldAnchor);

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

	/**
	 * Draw a sub-pane in the pane.
	 * @param pane  [A parent pane of sub-panes]
	 * @param index [An index of a sub-pane that will be drawn]
	 */
	void drawSubPane(Pane* pane,size_t index);
};

} /* namespace TM2P5DComponent */

#endif
