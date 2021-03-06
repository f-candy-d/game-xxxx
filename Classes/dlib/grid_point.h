#ifndef D_LIB_GRID_POINT_H
#define D_LIB_GRID_POINT_H

#include "size.h"
#include "vec2.h"

namespace dlib
{
	struct grid_point;
}

/**
 * NOTE : this class does not support the negative number
 */
struct dlib::grid_point
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
	const dlib::size<float> unit;

	grid_point(dlib::size<float> unit);
	grid_point(unsigned int p, unsigned int q, dlib::size<float> unit);
	void set_representative_point(RepresentativePoint type);
	void set_representative_point(float rep_scal_x, float rep_scal_y);
	dlib::vec2<float> comv_to_px() const;

private:
	float rep_point_scale_x;
	float rep_point_scale_y;
};

#endif
