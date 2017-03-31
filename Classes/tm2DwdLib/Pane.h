#ifndef TM2D_W_D_LIB_PANE_H
#define TM2D_W_D_LIB_PANE_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "DLib/dlib_size.h"
#include "DLib/dlib_vec2.h"
#include <vector>

namespace TM2DwD
{
	namespace TM2DwDUnit
	{
		class Pane;

		//forward declaration
		struct AtlasInfo;
	}
}

class TM2DwD::TM2DwDUnit::Pane : public cocos2d::Ref
{
public:
	static Pane* create(DLib::size<size_t> pSize, const AtlasInfo* pAtlasInfo);
	Pane(DLib::size<size_t> pSize, int pTtnt);
	Pane();
	const int& getTypeAt(int x, int y) const;
	const int& getTypeAt(int index) const;
	void insertTypeAt(int x, int y, int type);
	void insertTypeAt(int index, int type);
	// inline methods
	inline bool isModified() { return mIsModified; }

protected:
	bool init();

private:
	const int TILE_TYPE_NO_TILE;
	CC_SYNTHESIZE_READONLY(const DLib::size<size_t>, mGridSize, GridSize);
	CC_SYNTHESIZE(DLib::vec2<unsigned int>, mGridPoint, GridPoint);
	CC_SYNTHESIZE_PASS_BY_REF(std::vector<int>, mTiles, Tiles);
	bool mIsModified; // read-only
};

#endif
