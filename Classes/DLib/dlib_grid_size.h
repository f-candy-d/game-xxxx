#ifndef D_LIB_GRID_SIZE_H
#define D_LIB_GRID_SIZE_H

#include "dlib_size.h"
#include <iostream>

namespace DLib
{
	struct grid_size;
	//global operators
	bool operator<(const grid_size& a, const grid_size& b);
	bool operator>(const grid_size& a, const grid_size& b);
	bool operator<=(const grid_size& a, const grid_size& b);
	bool operator>=(const grid_size& a, const grid_size& b);
	bool operator==(const grid_size& a, grid_size& b);
	bool operator!=(const grid_size& a, grid_size& b);
	std::ostream& operator<<(std::ostream& os, const DLib::grid_size& gd_size);
}

struct DLib::grid_size
{
	int width;
	int height;
	const DLib::size<float> unit; //px

public:
	grid_size(DLib::size<float> u) :width(0),height(0),unit(u) {}
	grid_size(int w, int h, DLib::size<float> u) :width(w),height(h),unit(u) {}
	DLib::size<float> comv_to_px();
};

#endif
