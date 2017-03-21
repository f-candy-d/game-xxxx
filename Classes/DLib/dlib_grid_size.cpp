#include "dlib_grid_size.h"

/**
 * public
 */
DLib::size<float> DLib::grid_size::comv_to_px()
{
	return DLib::size<float>(unit.width * width, unit.height * height);
}

/**
 * global
 */
bool DLib::operator<(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return (a.width < b.width) && (a.height < b.height);
}

bool DLib::operator>(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return b < a;
}

bool DLib::operator<=(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return !(a > b);
}

bool DLib::operator>=(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return !(a < b);
}

bool DLib::operator==(const DLib::grid_size& a, DLib::grid_size& b)
{
	return (a.width == b.width) && (a.height == b.height);
}

bool DLib::operator!=(const DLib::grid_size& a, DLib::grid_size& b)
{
	return !(a == b);
}
