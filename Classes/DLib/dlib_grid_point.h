#ifndef D_LIB_GRID_POINT_H
#define D_LIB_GRID_POINT_H

#include "dlib_size.h"

namespace DLib
{
	class grid_point;
}

class DLib::grid_point
{
public:
	enum class RepresentativePoint
	{
		LEFT_BOTTOM,
		RIGHT_BOTTOM,
		LEFT_TOP,
		RIGHT_TOP,
		CENTER,
	};

	grid_point(DLib::size<float> unit)
	:mX(0),mY(0),mRepPointScaleX(0.0),mRepPointScaleY(0.0),mUnit(unit) {}
	grid_point(int p, int q, DLib::size<float> unit)
	:mX(p),mY(q),mRepPointScaleX(0.0),mRepPointScaleY(0.0),mUnit(unit) {}
	void set_representative_point(RepresentativePoint type);
	void set_representative_point(float repScaX, float repScaY);
	int x();
	int y();
	DLib::size<float> unit();

private:
	int mX;
	int mY;
	float mRepPointScaleX;
	float mRepPointScaleY;
	DLib::size<float> mUnit;
};

#endif
