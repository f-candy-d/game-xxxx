#ifndef D_LIB_H
#define D_LIB_H

/**
 * All members of the namespace 'DLib'.
 */
namespace DLib
{
	template <typename T> class locker_array;
	template <class Category, class Value, class Hash> class categorized_map;
	class range;
}

/**
 * NOTE : If you are going to use many classes of D-Library, include this header file.
 *        This file will include all other header files in it.
 */

#include "dlib_locker_array.h"
#include "dlib_categorized_map.h"
#include "dlib_range.h"

#endif
