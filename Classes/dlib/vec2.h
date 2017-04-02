#ifndef DLIB_VEC2_H
#define DLIB_VEC2_H

#include <iostream>
#include <utility>

namespace dlib
{
	template <typename T> struct vec2;
}

template <typename T = float> struct dlib::vec2
{
	T x;
	T y;

public:
	vec2() :x(0),y(0) {}
	vec2(T p, T q) :x(p),y(q) {}

	inline vec2<T>& operator+()
	const
	{
		return (*this);
	}

	inline vec2<T>& operator-()
	{
		this->x *= -1;
		this->y *= -1;
		return (*this);
	}

	inline vec2<T> operator+(const vec2<T>& other)
	const
	{
		return std::move(vec2(this->x + other.x, this->y + other.y));
	}

	inline vec2<T> operator-(const vec2<T>& other)
	const
	{
		return std::move(vec2(this->x - other.x, this->y - other.y));
	}

	inline void set(T p, T q)
	{
		this->x = p;
		this->y = q;
	}

};

namespace dlib
{
	// global operators
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const vec2<T>& v2)
	{
		return (os << '(' << v2.x << " , " << v2.y << ')');
	}
}

#endif
