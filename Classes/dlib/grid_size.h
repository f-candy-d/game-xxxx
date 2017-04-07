#ifndef D_LIB_GRID_SIZE_H
#define D_LIB_GRID_SIZE_H

#include "size.h"
#include <iostream>

namespace dlib
{
	struct grid_size;
	//global operators
	bool operator<(const grid_size& a, const grid_size& b);
	bool operator>(const grid_size& a, const grid_size& b);
	bool operator==(const grid_size& a, const grid_size& b);
	bool operator!=(const grid_size& a, const grid_size& b);
	bool operator<=(const grid_size& a, const grid_size& b);
	bool operator>=(const grid_size& a, const grid_size& b);
	std::ostream& operator<<(std::ostream& os, const dlib::grid_size& gd_size);
}

struct dlib::grid_size
{
	size_t width;
	size_t height;
	dlib::size<float> unit; //px

public:
	grid_size() :width(0),height(0),unit(0.0, 0.0) {}
	grid_size(dlib::size<float> u) :width(0),height(0),unit(u) {}
	grid_size(size_t w, size_t h, dlib::size<float> u) :width(w),height(h),unit(u) {}
	dlib::size<float> comv_to_px();
	void copy(const dlib::grid_size& other);
	inline size_t area() const { return width * height; }
};

#endif
