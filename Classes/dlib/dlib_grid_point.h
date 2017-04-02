#ifndef D_LIB_GRID_POINT_H
#define D_LIB_GRID_POINT_H

#include "dlib_size.h"
#include "dlib_vec2.h"

namespace DLib
{
	struct grid_point;
}

/**
 * NOTE : this class does not support the negative number
 */
struct DLib::grid_point
{
public:
	enum class RepresentativePoint
	{
		LEFT_BOTTOM,
		RIGHT_BOTTOM,
		LEFT_TOP,
		RIGHT_TOP,
		CENTER,
	};

public:
	unsigned int x;
	unsigned int y;
	const DLib::size<float> unit;

	grid_point(DLib::size<float> unit);
	grid_point(unsigned int p, unsigned int q, DLib::size<float> unit);
	void set_representative_point(RepresentativePoint type);
	void set_representative_point(float rep_scal_x, float rep_scal_y);
	DLib::vec2<float> comv_to_px() const;

private:
	float rep_point_scale_x;
	float rep_point_scale_y;
};

#endif
