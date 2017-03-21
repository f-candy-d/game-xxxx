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
	static Pane* create();
	Pane();

private:
	std::vector<int> mTiles;
};

#endif
