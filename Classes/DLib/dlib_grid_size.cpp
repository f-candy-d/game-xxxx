#include "dlib_grid_size.h"

inline int DLib::grid_size::x()
{
	return mX;
}

inline int DLib::grid_size::y()
{
	return mY;
}

inline DLib::size<float> DLib::grid_size::unit()
{
	return mUnit;
}

inline DLib::size<float> DLib::grid_size::comv_to_px()
{
	return DLib::size<float>((mX + mComvScaleX) * mUnit.width, (mY + mComvScaleY) * mUnit.height);
}

void DLib::grid_size::set_comv_scele(DLib::grid_size::ComvScale type)
{
	switch (type)
	{
		case DLib::grid_size::ComvScale::LEFT_BOTTOM :
			this->set_comv_scele(0, 0);
			break;

		case DLib::grid_size::ComvScale::RIGHT_BOTTOM :
			this->set_comv_scele(1.0, 0);
			break;

		case DLib::grid_size::ComvScale::LEFT_TOP :
			this->set_comv_scele(0, 1.0);
			break;

		case DLib::grid_size::ComvScale::RIGHT_TOP :
			this->set_comv_scele(1.0, 1.0);
			break;

		case DLib::grid_size::ComvScale::CENTER :
			this->set_comv_scele(0.5, 0.5);
	}
}

void DLib::grid_size::set_comv_scele(float sc_x, float sc_y)
{
	//scale-x
	if(sc_x < 0)
		mComvScaleX = 0.0;
	else if(1 < sc_x)
		mComvScaleX = 1.0;
	else
		mComvScaleX = sc_x;

	//scale-y
	if(sc_y < 0)
		mComvScaleY = 0.0;
	else if(1 < sc_y)
		mComvScaleY = 1.0;
	else
		mComvScaleY = sc_y;
}
