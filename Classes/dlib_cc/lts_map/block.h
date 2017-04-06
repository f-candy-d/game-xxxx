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
	static Block* Create(const dlib::size<size_t>& size, const AtlasInfo* atlas_info);
	void Reset(const dlib::vec2<int>& position, bool do_cleanup);
	// inline methods
	inline int GetTypeAt(int index) const { return tiles_[index]; }
	inline void InsertTypeAt(int index, int type) { tiles_[index] = type; }
	inline void PushBackType(int type) { tiles_.push_back(type); }
	inline cocos2d::Sprite* GetSpriteAt(int index) const { return sprites_.at(index); }
	inline void InsertSpriteAt(int index, cocos2d::Sprite* sprite) { sprites_.insert(index, sprite); }
	inline void PushBackSprite(cocos2d::Sprite* sprite) { sprites_.pushBack(sprite); }
	// accessors and mutators
	inline dlib::size<size_t> size() const { return size_; }
	inline bool is_modified() const { return is_modified_; }
	inline const std::vector<int>& tiles() const { return tiles_; }
	inline const cocos2d::Vector<cocos2d::Sprite*>& sprites() const { return sprites_; }
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
	cocos2d::Vector<cocos2d::Sprite*> sprites_;
	// others
	dlib::vec2<int> position_;
};

#endif
