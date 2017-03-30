#ifndef TM2D_W_D_LIB_PANE_H
#define TM2D_W_D_LIB_PANE_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "DLib/dlib_grid_size.h"
#include <vector>

namespace TM2DwD
{
	namespace TM2DwDUnit
	{
		class Pane;
	}
}

class TM2DwD::TM2DwDUnit::Pane : public cocos2d::Ref
{
public:
	static Pane* create(size_t pWidth, size_t pHeight);

	/**
	 * @param pAts : Actual-Tile-Size
	 * @param pTts : Tile-Texture-Size
	 * @param pWidth : Width of a pane
	 * @param pHeight : Height of a pane
	 */
	Pane(DLib::size<float> pAts, DLib::size<float> pTts,size_t pWidth, size_t pHeight);
	Pane();
	void setSpriteScale(float scale);

protected:
	bool initWithSize(size_t pWidth, size_t pHeight);

private:
	CC_SYNTHESIZE_READONLY(DLib::grid_size, mGridSize, GridSize);
	CC_SYNTHESIZE_READONLY(const DLib::size<float>, mTileTextureSize, TileTextureSize);
	CC_SYNTHESIZE_READONLY(DLib::size<float>, mActualTileSize, ActualTileSize);
	CC_SYNTHESIZE_READONLY(float, mSpriteScale, SpriteScale);
	CC_SYNTHESIZE_PASS_BY_REF(std::vector<int>, mTiles, Tiles);
};

#endif
