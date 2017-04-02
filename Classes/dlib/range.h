#ifndef RANGE_H
#define RANGE_H

#include<cassert>

namespace dlib {
	class range;
}

class dlib::range
{
public:
	range(int from = 0, int to = 0)
	{ assert(from <= to); mFrom = from; mTo = to; }

	inline void set(int from, int to)
	{ assert(from <= to); mFrom = from; mTo = to; }

	inline int from()
	const
	{ return mFrom; }

	inline void from(int from)
	{ assert(from <= mTo); mFrom = from; }

	inline int to()
	const
	{ return mTo; }

	inline void to(int to)
	{ assert(mFrom <= to); mTo = to; }

	inline int diff()
	const
	{ return mTo - mFrom; }

	inline int distance()
	const
	{ return this->diff() + 1; }

	inline void move(int delta)
	{ mFrom += delta; mTo += delta; }

	inline bool contains(int p)
	const
	{ return (mFrom <= p && p <= mTo); }

	inline range merge(const range& other)
	const
	{
		return range(mFrom < other.from() ? mFrom : other.from(), mTo < other.to() ? other.to() : mTo);
	}

private:
	int mFrom;
	int mTo;
};

#endif
