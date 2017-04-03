#ifndef DLIB_CC_LTS_MAP_BLOCK_H
#define DLIB_CC_LTS_MAP_BLOCK_H

#include "../tools/include_lib_index.h"
#include <vector>

namespace lts_map
{
	namespace unit
	{
		class Block;

		//forward declaration
		struct AtlasInfo;
	}
}

class lts_map::unit::Block : public cocos2d::Ref
{
public:
	struct SpriteOwnership
	{
		using Id = std::string;

	public:
		bool has_ownership;
		Id id;

		SpriteOwnership() :has_ownership(false) {}
		SpriteOwnership(Id& id) :has_ownership(false),id(id) {}

		inline void transfer(SpriteOwnership& to)
		{
			to.id = this->id;
			this->id.clear();
			this->id.shrink_to_fit();
			to.has_ownership = true;
			this->has_ownership = false;
		}
	};

public:
	static Block* Create(const dlib::size<size_t>& size, const AtlasInfo* atlas_info);
	int GetTypeAt(int x, int y) const;
	int GetTypeAt(int index) const;
	void InsertTypeAt(int x, int y, int type);
	void InsertTypeAt(int index, int type);
	void Reset(const dlib::vec2<int>& position, bool do_cleanup);
	// accessors and mutators
	inline dlib::size<size_t> size() const { return size_; }
	inline bool is_modified() const { return is_modified_; }
	inline const std::vector<int>& tiles() const { return tiles_; }
	inline dlib::vec2<int> position() const { return position_; }
	inline void set_position(const dlib::vec2<int>& val) { position_ = val; }

protected:
	Block();
	Block(const dlib::size<size_t>& size, int tile_type_no_tile);
	bool Init();

private:
	// constant variable
	const int tile_type_no_tile_;
	const dlib::size<size_t> size_;
	// read-only variables
	bool is_modified_;
	std::vector<int> tiles_;
	// others
	dlib::vec2<int> position_;
};

#endif
