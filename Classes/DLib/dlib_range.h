#ifndef RANGE_H
#define RANGE_H

#include<cassert>

namespace DLib {
	class range;
}

class DLib::range
{
public:
	range(int from = 0, int to = 0)
	{ assert(from <= to); mFrom = from; mTo = to; }

	void set(int from, int to)
	{ assert(from <= to); mFrom = from; mTo = to; }

	int from()
	const
	{ return mFrom; }

	void from(int from)
	{ assert(from <= mTo); mFrom = from; }

	int to()
	const
	{ return mTo; }

	void to(int to)
	{ assert(mFrom <= to); mTo = to; }

	int diff()
	const
	{ return mTo - mFrom; }

	int distance()
	const
	{ return this->diff() + 1; }

	void move(int delta)
	{ mFrom += delta; mTo += delta; }

	bool contains(int p)
	const
	{ return (mFrom <= p && p <= mTo); }

	range merge(const range& other)
	const
	{
		return range(mFrom < other.from() ? mFrom : other.from(), mTo < other.to() ? other.to() : mTo);
	}

private:
	int mFrom;
	int mTo;
};

#endif
