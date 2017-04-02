#include "grid_size.h"
#include <cassert>

/**
 * public
 */
DLib::size<float> DLib::grid_size::comv_to_px()
{
	return DLib::size<float>(unit.width * width, unit.height * height);
}

void DLib::grid_size::copy(const DLib::grid_size &other)
{
	width = other.width;
	height = other.height;
	const_cast<DLib::size<float>&>(unit) = other.unit;
}

/**
 * global
 */
bool DLib::operator<(const DLib::grid_size& a, const DLib::grid_size& b)
{
	assert(a.unit == b.unit);
	return (a.width < b.width) && (a.height < b.height);
}

bool DLib::operator>(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return b < a;
}

bool DLib::operator==(const DLib::grid_size& a, const DLib::grid_size& b)
{
	assert(a.unit == b.unit);
	return (a.width == b.width) && (a.height == b.height);
}

bool DLib::operator!=(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return !(a == b);
}

bool DLib::operator<=(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return (a < b) || (a == b);
}

bool DLib::operator>=(const DLib::grid_size& a, const DLib::grid_size& b)
{
	return (a > b) || (a == b);
}

std::ostream& DLib::operator<<(std::ostream& os, const DLib::grid_size& gd_size)
{
	return (os << '[' << gd_size.width << " x " << gd_size.height << ']');
}
