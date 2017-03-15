#ifndef D_LIB_H
#define D_LIB_H

/**
 * All members of the namespace 'DLib'.
 */
namespace DLib
{
	template <typename T> class LockerArray;
	class range;
}

/**
 * NOTE : If you are going to use many classes of D-Library, include this header file.
 *        This file will include all other header files in it.
 */

#include "LockerArray.h"
#include "Range.h"

#endif
