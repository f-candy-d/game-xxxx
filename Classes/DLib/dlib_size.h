#ifndef D_LIB_SIZE_H
#define D_LIB_SIZE_H

namespace DLib
{
	template <typename T> struct size;
}

template <typename T>
struct DLib::size
{
	T width;
	T height;

	size() :width(0),height(0) {}
	size(T w, T h) :width(w),height(h) {}
	T area() { return width * height; }
};

#endif
