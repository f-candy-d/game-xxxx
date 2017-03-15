#ifndef LOCKER_ARRAY_H
#define LOCKER_ARRAY_H

#include "Range.h"
#include "../../cocos2d/cocos/cocos2d.h"
#include <iostream>
#include <vector>

namespace DLib
{
	template <typename T> class LockerArray;
}

template <typename T>
class DLib::LockerArray
{
	/**
	 * structure Key
	 */
 public:
	struct Key
	{
		// NOTE : ONLY LockerArray<T> class can create an object of this class
		friend LockerArray<T>;

	private:
		int id;
		// the number of lockers in used
		size_t used;
		Key(int id):id(-1),used(0) { this->id = id; }
	};

	/**
	 * LockerArray<T> class
	 */
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

public:
	// iterator
	iterator begin()
	{ return mLockers.begin(); }

	iterator end()
	{ return mLockers.end(); }

	const_iterator cbegin()
	const
	{ return mLockers.cbegin(); }

	const_iterator cend()
	const
	{ return mLockers.cend(); }

	const_iterator beginOf(Key key)
	const
	{ return mLockers.cbegin() += mRanges[key.id].from(); }

	const_iterator endOf(Key key)
	const
	{ return mLockers.cbegin() += mRanges[key.id].to() + 1; }

	LockerArray(const size_t capacity = 0)
	{
		// NOTE : T must be a child class of cocos2d::Ref class
		// static_assert(std::is_convertible<T, cocos2d::Ref*>::value, "In LockerArray<T>, T must be a pointer of a child class of cocos2d::Ref class!");

		if(0 < capacity)
			mLockers.reserve(capacity);
	}

	LockerArray(const LockerArray<T>& other)
	{
		this->copy(other);
	}

	~LockerArray()
	{}

	LockerArray<T>& operator=(const LockerArray<T>& other)
	{
		this->copy(other);
		return (*this);
	}

	size_t size()
	const
	{
		return mLockers.size();
	}

	void setDummyBaggage(T dummy)
	{
		mDummy = dummy;
	}

	T getDummyBaggage()
	const
	{
		return mDummy;
	}

	Key rentLocker(size_t size)
	{
		assert(0 < size);

		DLib::Range range(mLockers.size(), mLockers.size() + size - 1);
		mRanges.push_back(range);

		//fill lockers with dummy objects
		for(size_t i = 0; i < size; ++i)
			mLockers.push_back(mDummy);

		// the id of Key is the index of 'range' in the vector 'mRange'
		return Key(mRanges.size() - 1);
	}

	T getBaggage(Key key, size_t local_index)
	const
	{
		return mLockers[this->toIndex(key, local_index)];
	}

	T removeBaggage(Key& key, size_t local_index)
	{
		size_t index = this->toIndex(key, local_index);
		// remove the baggage from a locker and leave dummy object instead
		T old = mLockers[index];
		if(old != mDummy)
		{
			mLockers[index] = mDummy;
			--key.used;
		}

		return old;
	}

	bool leaveBaggage(Key& key, size_t local_index, T baggage)
	{
		size_t index = this->toIndex(key, local_index);
		// if only there are no baggage in the locker at the index,
		// leave the baggage in it and return true,otherwise return false
		if(mLockers[index] == mDummy && baggage != mDummy)
		{
			mLockers[index] = baggage;
			++key.used;
			return true;
		}

		return false;
	}

	T replaceBaggage(Key& key, size_t local_index, T baggage)
	{
		T old = this->removeBaggage(key, local_index);
		this->leaveBaggage(key, local_index, baggage);

		return old;
	}

	std::vector<T> getLockersOf(Key key)
	const
	{
		std::vector<T> vec;
		vec.reserve(mRanges[key.id].distance());
		std::copy(this->beginOf(key), this->endOf(key), std::back_inserter(vec));

		return std::move(vec);
	}

private:
	T mDummy;
	std::vector<T> mLockers;
	std::vector<DLib::Range> mRanges;

	void copy(const LockerArray<T>& other)
	{
		this->mLockers.reserve(other.mLockers.size());
		std::copy(other.mLockers.begin(),other.mLockers.end(),std::back_inserter(this->mLockers));

		this->mRanges.reserve(other.mRanges.size());
		std::copy(other.mRanges.begin(),other.mRanges.end(),std::back_inserter(this->mRanges));
	}

	size_t toIndex(Key key, size_t local_index)
	{
		DLib::Range range = mRanges[key.id];
		size_t index = range.from() + local_index;

		// range check
		assert(range.contains(index));
		assert(index < mLockers.size());

		return index;
	}

};

#endif
