#ifndef RANGE_H
#define RANGE_H

#include<cassert>

namespace DLib {
	class Range;
}

class DLib::Range
{
public:
	Range(int from = 0, int to = 0)
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

private:
	int mFrom;
	int mTo;
};

#endif
