#include "grid_point.h"

/**
 * publc
 */
dlib::grid_point::grid_point(dlib::size<float> unit)
:x(0)
,y(0)
,unit(unit)
,rep_point_scale_x(0.0)
,rep_point_scale_y(0.0)
{}

dlib::grid_point::grid_point(unsigned int p, unsigned int q, dlib::size<float> unit)
:x(p)
,y(q)
,unit(unit)
,rep_point_scale_x(0.0)
,rep_point_scale_y(0.0)
{}

void dlib::grid_point::set_representative_point(RepresentativePoint type)
{
	switch (type)
	{
		case dlib::grid_point::RepresentativePoint::LEFT_BOTTOM :
			this->set_representative_point(0, 0);
			break;

		case dlib::grid_point::RepresentativePoint::RIGHT_BOTTOM :
			this->set_representative_point(1.0, 0);
			break;

		case dlib::grid_point::RepresentativePoint::LEFT_TOP :
			this->set_representative_point(0, 1.0);
			break;

		case dlib::grid_point::RepresentativePoint::RIGHT_TOP :
			this->set_representative_point(1.0, 1.0);
			break;

		case dlib::grid_point::RepresentativePoint::CENTER :
			this->set_representative_point(0.5, 0.5);
	}
}

void dlib::grid_point::set_representative_point(float rep_scal_x, float rep_scal_y)
{
	//scale-width
	if(rep_scal_x < 0)
		rep_point_scale_x = 0.0;
	else if(1 < rep_scal_x)
		rep_point_scale_x = 1.0;
	else
		rep_point_scale_x = rep_scal_x;

	//scale-height
	if(rep_scal_y < 0)
		rep_point_scale_y = 0.0;
	else if(1 < rep_scal_y)
		rep_point_scale_y = 1.0;
	else
		rep_point_scale_y = rep_scal_y;
}

dlib::vec2<float> dlib::grid_point::comv_to_px()
const
{
	return dlib::vec2<float>((x + rep_point_scale_x) * unit.width, (y + rep_point_scale_y) * unit.height);
}
