#ifndef COCOS_LOCKER_ARRAY_H
#define COCOS_LOCKER_ARRAY_H

#include "../DLib/dlib_locker_array.h"
#include "../../cocos2d/cocos/cocos2d.h"

namespace TM2DwD
{
	namespace TM2DwDUnit
	{
		template <typename T> class CocosLockerArray;
	}
}

/**
 * NOTE : In CocosLockerArray<T> class, T must be a pointer of a class which inherits cocos2d::Ref class.
 *        Increase the reference counter of an element when add it in this array,
 *        and decrease that of an element when remove it from this array.
 */
template <typename T>
class TM2DwD::TM2DwDUnit::CocosLockerArray : public DLib::locker_array<T>
{
public:
	CocosLockerArray(const size_t capacity = 0)
	:DLib::locker_array<T>(capacity)
	{
		static_assert(std::is_convertible<T, cocos2d::Ref*>::value, "In CocosLockerArray<T>, T must be a pointer of a child class of cocos2d::Ref class!");

		// NOTE : DO NOT set again the dummy baggage on another place!
		T dummy = nullptr;
		this->set_dummy_baggage(dummy);
	}

	~CocosLockerArray()
	{
		auto dummy = this->get_dummy_baggage();
		for(auto itr = this->begin(); itr != this->end(); ++itr)
			if(*itr != dummy)
			{
				(*itr)->release();
				*itr = dummy;
			}
	}

	bool return_locker(typename DLib::locker_array<T>::key& k_ey)
	override
	{
		if(k_ey.is_broken)
			return false;

		auto dummy = this->get_dummy_baggage();
		for(auto itr = this->begin_of(k_ey); itr != this->end_of(k_ey); ++itr)
			if(*itr != dummy)
				(*itr)->release();

		return DLib::locker_array<T>::return_locker(k_ey);
	}

	T remove_baggage(typename DLib::locker_array<T>::key& k_ey, size_t local_index)
	override
	{
		T removed = DLib::locker_array<T>::remove_baggage(k_ey, local_index);
		if(removed != this->get_dummy_baggage())
			removed->release();

		return removed;
	}

	bool leave_baggage(typename DLib::locker_array<T>::key& k_ey, size_t local_index, T baggage)
	override
	{
		bool result = DLib::locker_array<T>::leave_baggage(k_ey, local_index, baggage);
		if(result)
			baggage->retain();

		return result;
	}

	T replace_baggage(typename DLib::locker_array<T>::key& k_ey, size_t local_index, T baggage)
	override
	{
		baggage->retain();
		T old = DLib::locker_array<T>::replace_baggage(k_ey, local_index, baggage);
		if(old != this->get_dummy_baggage())
			old->release();

		return old;
	}

private:
	// NOTE : hide this function from the user
	void set_dummy_baggage(T dummy)
	override
	{
		DLib::locker_array<T>::set_dummy_baggage(dummy);
	}
};

#endif
