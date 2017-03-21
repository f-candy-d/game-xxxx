#include "dlib_grid_point.h"

/**
 * publc
 */
void DLib::grid_point::set_representative_point(RepresentativePoint type)
{
	switch (type)
	{
		case DLib::grid_point::RepresentativePoint::LEFT_BOTTOM :
			this->set_representative_point(0, 0);
			break;

		case DLib::grid_point::RepresentativePoint::RIGHT_BOTTOM :
			this->set_representative_point(1.0, 0);
			break;

		case DLib::grid_point::RepresentativePoint::LEFT_TOP :
			this->set_representative_point(0, 1.0);
			break;

		case DLib::grid_point::RepresentativePoint::RIGHT_TOP :
			this->set_representative_point(1.0, 1.0);
			break;

		case DLib::grid_point::RepresentativePoint::CENTER :
			this->set_representative_point(0.5, 0.5);
	}
}

void DLib::grid_point::set_representative_point(float repScaX, float repScaY)
{
	//scale-x
	if(repScaX < 0)
		mRepPointScaleX = 0.0;
	else if(1 < repScaX)
		mRepPointScaleX = 1.0;
	else
		mRepPointScaleX = repScaX;

	//scale-y
	if(repScaY < 0)
		mRepPointScaleY = 0.0;
	else if(1 < repScaY)
		mRepPointScaleY = 1.0;
	else
		mRepPointScaleY = repScaY;
}

inline int DLib::grid_point::x()
{
	return mX;
}

inline int DLib::grid_point::y()
{
	return mY;
}

inline DLib::size<float> DLib::grid_point::unit()
{
	return mUnit;
}
