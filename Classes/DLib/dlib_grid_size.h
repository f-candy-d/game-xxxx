#ifndef D_LIB_GRID_SIZE_H
#define D_LIB_GRID_SIZE_H

#include "dlib_size.h"

namespace DLib
{
	class grid_size;
}

class DLib::grid_size
{
public:
	grid_size(DLib::size<float> unit) :mWidth(0),mHeight(0),mUnit(unit) {}
	grid_size(int width, int height, DLib::size<float> unit) :mWidth(width),mHeight(height),mUnit(unit) {}

	int width();
	int height();
	DLib::size<float> unit();
	DLib::size<float> comv_to_px();

private:
	int mWidth;
	int mHeight;
	const DLib::size<float> mUnit; //px
};

#endif
