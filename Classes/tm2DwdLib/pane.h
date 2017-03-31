#ifndef TM2D_W_D_LIB_PANE_H
#define TM2D_W_D_LIB_PANE_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "DLib/dlib_size.h"
#include "DLib/dlib_vec2.h"
#include <vector>

namespace tm2d_w_d
{
	namespace tm2d_w_d_unit
	{
		class Pane;

		//forward declaration
		struct AtlasInfo;
	}
}

class tm2d_w_d::tm2d_w_d_unit::Pane : public cocos2d::Ref
{
public:
	Pane();
	Pane(DLib::size<size_t> v_size, int v_tile_type_no_tile);
	static Pane* create(DLib::size<size_t> v_size, const AtlasInfo* v_atlas_info);
	int getTypeAt(int v_x, int v_y) const;
	int getTypeAt(int v_index) const;
	void insertTypeAt(int v_x, int v_y, int v_type);
	void insertTypeAt(int v_index, int v_type);
	// accessors and mutators
	inline DLib::size<size_t> get_grid_size() const { return grid_size_; }
	inline bool is_modified() const { return is_modified_; }
	inline std::vector<int>& get_tiles() { return tiles_; }
	inline const std::vector<int>& get_tiles() const { return tiles_; }
	inline DLib::vec2<unsigned int> get_grid_point() const { return grid_point_; }
	inline void set_grid_point(DLib::vec2<unsigned int> v_val) { grid_point_ = v_val; }

protected:
	bool init();

private:
	// constant variable
	const int kTileTypeNoTile;
	// read-only variables
	const DLib::size<size_t> grid_size_;
	bool is_modified_;
	std::vector<int> tiles_;
	// others
	DLib::vec2<unsigned int> grid_point_;
};

#endif
