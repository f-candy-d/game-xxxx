#ifndef LOCKER_ARRAY_H
#define LOCKER_ARRAY_H

#include "dlib_range.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace DLib
{
	template <typename T> class locker_array;
}

template <typename T> class DLib::locker_array
{
	/**
	 * key object. use for access to the baaggage of the locker
	 */
 public:
	struct key
	{
		// NOTE : ONLY locker_array<T> class can create an object of this class
		friend locker_array<T>;

	public:
		// read-only member varialbes
		// the number of lockers you rented
		const size_t size;
		// the number of lockers in use
		const size_t in_use;
		// is the key broken or not
		const bool is_broken;
		// use this parameter to identifier a certain key
		int tag;

		key(const key& other)
		:id(other.id)
		,parent_serial(other.parent_serial)
		,size(other.size)
		,in_use(other.in_use)
		,is_broken(other.is_broken)
		,tag(other.tag)
		{}

		key& operator=(const key& other) &
		{
			const_cast<bool&>(is_broken) = other.is_broken;
			const_cast<size_t&>(size) = other.size;
			const_cast<size_t&>(in_use) = other.in_use;
			const_cast<int&>(id) = other.id;
			const_cast<int&>(parent_serial) = other.parent_serial;
			tag = other.tag;
			return (*this);
		}

		key spare() { return std::move(key(*this)); }

	private:
		key(int id, int parent_serial, size_t size)
		:id(id)
		,parent_serial(parent_serial)
		,size(size)
		,in_use(0)
		,is_broken(false)
		,tag(0)
		{}

		const int id;
		const int parent_serial;
		void increase() { const_cast<size_t&>(in_use) += 1; }
		void decrease() { const_cast<size_t&>(in_use) -= 1; }
		void destroy()
		{
			const_cast<bool&>(is_broken) = true;
			const_cast<size_t&>(size) = 0;
			const_cast<size_t&>(in_use) = 0;
		}
	};

	/**
	 * iterator class
	 */
public:
	class range_iterator : public std::iterator<std::forward_iterator_tag, T>
	{
		// NOTE : ONLY locker_array<T> class can create an object of this class
		friend locker_array<T>;

	public:
		range_iterator& operator++()
		{
			mIndex = (mRange.to() < mIndex + 1) ? mRange.to() : mIndex + 1;
		}

		range_iterator operator++(int)
		{
			auto before = *this;
			mIndex = (mRange.to() < mIndex + 1) ? mRange.to() : mIndex + 1;
			return before;
		}

		T& operator*() {
			if(mIndex <= mRange.to())
				return locker_array<T>::mLockers[mIndex];
			else
				return locker_array<T>::mDummy;
		};

		bool operator==(const range_iterator& other);
		bool operator!=(const range_iterator& other);

		range_iterator(const range_iterator& other)
		{
			this->mIndex = other.mIndex;
			this->mParent = other.mParent;
		}

	private:
		int mIndex;
		const DLib::range mRange;

		//constructors are private
		range_iterator()
		:mIndex(-1),mRange(-1,-1) {};

		range_iterator(int index, DLib::range& rng)
		{
			assert(rng.contains(index));

			mIndex = index;
			mRange = rng;
		}
	};

	/**
	 * locker_array<T> class
	 */
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

public:
	// iterator
	iterator begin()
	{
		return mLockers.begin();
	}

	iterator end()
	{
		return mLockers.end();
	}

	iterator begin_of(key k_ey)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mLockers.end();

		return mLockers.begin() += mRanges.at(k_ey.id).from();
	}

	iterator end_of(key k_ey)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mLockers.end();

		return mLockers.begin() += mRanges.at(k_ey.id).to() + 1;
	}

	const_iterator cbegin()
	const
	{
		return mLockers.cbegin();
	}

	const_iterator cend()
	const
	{
		return mLockers.cend();
	}

	const_iterator cbegin_of(key k_ey)
	const
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mLockers.cend();

		return mLockers.cbegin() += mRanges.at(k_ey.id).from();
	}

	const_iterator cend_of(key k_ey)
	const
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mLockers.cend();

		return mLockers.cbegin() += mRanges.at(k_ey.id).to() + 1;
	}

	locker_array(const size_t capacity = 0)
	:mSerial(-1)
	{
		mSerial = this->make_serial_code();

		if(0 < capacity)
			mLockers.reserve(capacity);
	}

	locker_array(const locker_array& other)
	{
		this->mLockers = other.mLockers;
		this->mRanges = other.mRanges;
		this->mDummy = other.mDummy;
		// NOTE : DO NOT copy a serial number! make new one
		this->mSerial = this->make_serial_code();
	}

	virtual ~locker_array() {}

	locker_array& operator=(const locker_array& other)
	{
		// NOTE : DO NOT copy a serial number!
		this->mLockers = other.mLockers;
		this->mRanges = other.mRanges;
		this->mDummy = other.mDummy;

		return (*this);
	}

	size_t size()
	const
	{
		return mLockers.size();
	}

	virtual void set_dummy_baggage(T dummy)
	{
		mDummy = dummy;
	}

	T get_dummy_baggage()
	const
	{
		return mDummy;
	}

	key rent_locker(size_t size)
	{
		assert(0 < size);

		// the id of key is the index of 'range' in the vector 'mRange'
		key k_ey(mRanges.size(), mSerial, size);
		DLib::range range(mLockers.size(), mLockers.size() + size - 1);

		mRanges[k_ey.id] = range;

		// fill lockers with dummy objects
		for(size_t i = 0; i < size; ++i)
			mLockers.push_back(mDummy);

		return k_ey;
	}

	virtual bool return_locker(key& k_ey)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return false;

		//remove the baggage in a range
		DLib::range rng = mRanges[k_ey.id];
		mLockers.erase(this->begin_of(k_ey), this->end_of(k_ey));

		mRanges.erase(k_ey.id);

		for(auto itr = mRanges.begin(); itr != mRanges.end(); ++itr)
			if(rng.from() < itr->second.from())
			{
				itr->second.move(-1 * k_ey.size);
			}

		// important!
		k_ey.destroy();

		return true;
	}

	T get_baggage(key k_ey, size_t local_index)
	const
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mDummy;
		else
			return mLockers[this->to_index(k_ey, local_index)];
	}

	virtual T remove_baggage(key& k_ey, size_t local_index)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mDummy;

		size_t index = this->to_index(k_ey, local_index);
		// remove the baggage from a locker and leave dummy object instead
		T old = mLockers[index];
		if(old != mDummy)
		{
			mLockers[index] = mDummy;
			k_ey.decrease();
		}

		return old;
	}

	virtual bool leave_baggage(key& k_ey, size_t local_index, T baggage)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return false;

		size_t index = this->to_index(k_ey, local_index);
		// if only there are no baggage in the locker at the index,
		// leave the baggage in it and return true,otherwise return false
		if(mLockers[index] == mDummy && baggage != mDummy)
		{
			mLockers[index] = baggage;
			k_ey.increase();

			return true;
		}

		return false;
	}

	virtual T replace_baggage(key& k_ey, size_t local_index, T baggage)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return mDummy;

		T old = this->remove_baggage(k_ey, local_index);
		this->leave_baggage(k_ey, local_index, baggage);

		return old;
	}

	bool swap(key k_ey, size_t local_index_a, size_t local_index_b)
	{
		if(k_ey.is_broken && k_ey.parent_serial == mSerial)
			return false;

		size_t index_a = this->to_index(k_ey, local_index_a);
		size_t index_b = this->to_index(k_ey, local_index_b);

		T tmp = mLockers[index_a];
		mLockers[index_a] = mLockers[index_b];
		mLockers[index_b] = tmp;

		return true;
	}

	std::vector<T> get_lockers_of(key k_ey)
	const
	{
		std::vector<T> vec;

		if(!k_ey.is_broken)
		{
		vec.reserve(k_ey.size);
		std::copy(this->begin_of(k_ey), this->end_of(k_ey), std::back_inserter(vec));
		}

		return std::move(vec);
	}

protected:
	std::vector<T>& get_lockers() { return mLockers; }
	std::unordered_map<int, DLib::range>& get_ranges() { return mRanges; }

	size_t to_index(key& k_ey, size_t local_index)
	const
	{
		DLib::range rng = mRanges.at(k_ey.id);
		size_t index = rng.from() + local_index;

		// range check
		assert(rng.contains(index));
		assert(index < mLockers.size());

		return index;
	}

public:
	int get_serial() {return mSerial;}

private:
	int mSerial;
	T mDummy;
	std::vector<T> mLockers;
	std::unordered_map<int, DLib::range> mRanges;
	// do not use this variable directly!
	static int mStaticSerial;

	int make_serial_code() { return mStaticSerial++; }

};

// use this serial number for identificaiton of each locker-array object
template <typename T>
int DLib::locker_array<T>::mStaticSerial = 0;

#endif
