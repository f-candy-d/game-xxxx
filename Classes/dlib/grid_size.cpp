#include "grid_size.h"
#include <cassert>

/**
 * public
 */
dlib::size<float> dlib::grid_size::comv_to_px()
{
	return dlib::size<float>(unit.width * width, unit.height * height);
}

void dlib::grid_size::copy(const dlib::grid_size &other)
{
	width = other.width;
	height = other.height;
	const_cast<dlib::size<float>&>(unit) = other.unit;
}

/**
 * global
 */
bool dlib::operator<(const dlib::grid_size& a, const dlib::grid_size& b)
{
	assert(a.unit == b.unit);
	return (a.width < b.width) && (a.height < b.height);
}

bool dlib::operator>(const dlib::grid_size& a, const dlib::grid_size& b)
{
	return b < a;
}

bool dlib::operator==(const dlib::grid_size& a, const dlib::grid_size& b)
{
	assert(a.unit == b.unit);
	return (a.width == b.width) && (a.height == b.height);
}

bool dlib::operator!=(const dlib::grid_size& a, const dlib::grid_size& b)
{
	return !(a == b);
}

bool dlib::operator<=(const dlib::grid_size& a, const dlib::grid_size& b)
{
	return (a < b) || (a == b);
}

bool dlib::operator>=(const dlib::grid_size& a, const dlib::grid_size& b)
{
	return (a > b) || (a == b);
}

std::ostream& dlib::operator<<(std::ostream& os, const dlib::grid_size& gd_size)
{
	return (os << '[' << gd_size.width << " x " << gd_size.height << ']');
}
