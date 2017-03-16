#ifndef D_LIB_H
#define D_LIB_H

/**
 * All members of the namespace 'DLib'.
 */
namespace DLib
{
	template <typename T> class locker_array;
	class range;
}

/**
 * NOTE : If you are going to use many classes of D-Library, include this header file.
 *        This file will include all other header files in it.
 */

#include "dlib_locker_array.h"
#include "dlib_range.h"

#endif
