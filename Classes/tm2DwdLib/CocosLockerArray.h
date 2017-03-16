#ifndef COCOS_LOCKER_ARRAY_H
#define COCOS_LOCKER_ARRAY_H

#include "../DLib/dlib_locker_array.h"
#include "../../cocos2d/cocos/cocos2d.h"

namespace TM2DwD
{
	namespace TM2DwDComponet
	{
		template <typename T> class CocosLockerArray;
	}
}

template <typename T>
class TM2DwD::TM2DwDComponet::CocosLockerArray : public DLib::locker_array<T>
{
public:
	CocosLockerArray(const size_t capacity = 0)
	:DLib::locker_array<T>(capacity)
	{
		std::cout << "CocosLockerArray("<< capacity << ") called!!" << '\n';

		// NOTE : T must be a child class of cocos2d::Ref class
		static_assert(std::is_convertible<T, cocos2d::Ref*>::value, "In CocosLockerArray<T>, T must be a pointer of a child class of cocos2d::Ref class!");
	}

	~CocosLockerArray()
	{
		std::cout << "~CocosLockerArray() called!!" << '\n';
	}
};

#endif
