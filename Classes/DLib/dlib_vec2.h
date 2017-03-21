#ifndef DLIB_VEC2_H
#define DLIB_VEC2_H

#include "dlib_vec.h"

namespace DLib
{
	template <typename T> class vec2;
}

template <typename T> class DLib::vec2 : public DLib::vec<2, T>
{
public:
	vec2() :DLib::vec<2, T>() {}
	vec2(T dflt) :DLib::vec<2, T>(dflt) {}
	T x()
	const
	{ return this->mComponents[0]; }

	void x(T val)
	{ this->mComponents[0] = val; }

	T y()
	const
	{ return this->mComponents[1]; }

	void y(T val)
	{ this->mComponents[1] = val; }

};

#endif
