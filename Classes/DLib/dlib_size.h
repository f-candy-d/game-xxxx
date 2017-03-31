#ifndef D_LIB_SIZE_H
#define D_LIB_SIZE_H

#include <iostream>

namespace DLib
{
	template <typename T> struct size;
}

template <typename T> struct DLib::size
{
	T width;
	T height;

public:
	size() :width(0),height(0) {}
	size(T w, T h) :width(w),height(h) {}
	inline T area() const { return width * height; }
	inline void scale(float scale) { width *= scale; height *= scale; }
};

// global operators
namespace DLib
{
	template <typename T>
	bool operator<(const size<T>& a, const size<T>& b)
	{
		return (a.width < b.width) && (a.height < b.height);
	}

	template <typename T>
	bool operator>(const size<T>& a, const size<T>& b)
	{
		return (b.width < a.width) && (b.height < a.height);
	}

	template <typename T>
	bool operator==(const size<T>& a, const size<T>& b)
	{
		return (a.width == b.width) && (a.height == b.height);
	}

	template <typename T>
	bool operator!=(const size<T>& a, const size<T>& b)
	{
		return !(a == b);
	}

	template <typename T>
	bool operator<=(const size<T>& a, const size<T>& b)
	{
		return (a < b) || (a == b);
	}

	template <typename T>
	bool operator>=(const size<T>& a, const size<T>& b)
	{
		return (a > b) || (a == b);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const size<T>& psize)
	{
		return (os << '(' << psize.width << " x " << psize.height << ')');
	}
}

#endif
