#ifndef D_LIB_SIZE_H
#define D_LIB_SIZE_H

namespace DLib
{
	template <typename T> struct size;
	// global operators
	// template <typename T>
	// bool operator<(const size<T>& a, const size<T>& b);
	// template <typename T>
	// bool operator>(const size<T>& a, const size<T>& b);
	// template <typename T>
	// bool operator<=(const size<T>& a, const size<T>& b);
	// template <typename T>
	// bool operator>=(const size<T>& a, const size<T>& b);
	// template <typename T>
	// bool operator==(const size<T>& a, const size<T>& b);
	// template <typename T>
	// bool operator!=(const size<T>& a, const size<T>& b);
}

template <typename T> struct DLib::size
{
	T width;
	T height;

public:
	size() :width(0),height(0) {}
	size(T w, T h) :width(w),height(h) {}
	T area() { return width * height; }
};

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
	bool operator<=(const size<T>& a, const size<T>& b)
	{
		return !(a > b);
	}

	template <typename T>
	bool operator>=(const size<T>& a, const size<T>& b)
	{
		return !(a < b);
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
}

#endif
