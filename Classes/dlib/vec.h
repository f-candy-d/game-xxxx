#ifndef DLIB_VEC_H
#define DLIB_VEC_H

#include <vector>
#include <iostream>

namespace dlib
{
	template <unsigned int D, typename T> class vec;
}

template <unsigned int D, typename T> class dlib::vec
{
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;

public:
	vec() :mComponents(D, 0) {}
	vec(T dflt) : mComponents(D, dflt) {}
	inline unsigned int dim() { return D; }

	template < typename... Rest>
	inline void set(const T& first, const Rest&... rest)
	{
		this->set_recur(0, first, rest...);
	}

	/**
	 * iterators
	 */
	inline iterator begin() {  return mComponents.begin(); }
	inline iterator end() {  return mComponents.end(); }
	inline const_iterator cbegin() const {  return mComponents.cbegin(); }
	inline const_iterator cend() const {  return mComponents.cend(); }

	/**
	 * operators
	 */
	inline T& operator[](std::size_t index)
	{
		assert(index < mComponents.size());
		return mComponents[index];
	}

	inline const T& operator[](std::size_t index)
	const
	{
		assert(index < mComponents.size());
		return mComponents[index];
	}

	inline vec<D, T>& operator+()
	const
	{
		return (*this);
	}

	inline vec<D, T> operator+(const vec<D, T>& other)
	const
	{
		vec<D, T> result;
		auto end_r = result.cend();
		auto end_a = this->mComponents.cend();
		auto end_b = other.mComponents.cend();

		for(auto itr_r = result.begin(), itr_a = this->mComponents.cbegin(), itr_b = other.mComponents.cbegin();
		itr_r != end_r && itr_a != end_a && itr_b != end_b;
		++itr_r, ++itr_a, ++itr_b)
			*itr_r = *itr_a + *itr_b;

		return std::move(result);
	}

	inline vec<D, T> operator-()
	{
		for(auto itr = mComponents.begin(); itr != mComponents.end(); ++itr)
			*itr *= -1;
		return vec(*this);
	}

	inline vec<D, T> operator-(const vec<D, T>& other)
	const
	{
		vec<D, T> result;
		auto end_r = result.cend();
		auto end_a = this->mComponents.cend();
		auto end_b = other.mComponents.cend();

		for(auto itr_r = result.begin(), itr_a = this->mComponents.cbegin(), itr_b = other.mComponents.cbegin();
		itr_r != end_r && itr_a != end_a && itr_b != end_b;
		++itr_r, ++itr_a, ++itr_b)
			*itr_r = *itr_a - *itr_b;

		return std::move(result);
	}

protected:
	std::vector<T> mComponents;

private:
	// do not anything in this function
	inline void set_recur(std::size_t index) {}

	// this function will be only called in set() function
	template < typename... Rest>
	void set_recur(std::size_t index, const T& first, const Rest&... rest)
	{
		mComponents[index] = first;
		++index;
		this->set_recur(index, rest...);
	}
};

#endif
