#ifndef CATEGORIZED_MAP_H
#define CATEGORIZED_MAP_H

#include <unordered_map>
#include <vector>
#include <iostream>

namespace DLib
{
	template <class Category, class Value, class Hash> class categorized_map;
}

template <class Category, class Value, class Hash = std::hash<Category>>
class DLib::categorized_map
{
	using category_vector = std::vector<Value>;
	using category_map = std::unordered_map<Category, category_vector, Hash>;

public:
	inline bool add_category(Category& cat)
	{ return this->add_category(cat, 0); }

	bool add_category(Category& cat, size_t size)
	{
		if(this->find_category(cat))
			return false;

		mMap[cat] = std::move(category_vector());
		mMap[cat].reserve(size);

		return true;
	}

	bool add_category(Category& cat, category_vector& source)
	{
		if(this->find_category(cat))
			return false;

		mMap[cat] = std::move(category_vector(source));

		return true;
	}

	void erase_category(Category& cat)
	{ mMap.erase(cat); }

	inline void erase_all_category()
	{ mMap.clear(); }

	inline category_vector& get_category(Category& cat)
	{ return this->find_category(cat) ? mMap[cat] : mDummyCategory; }

	// if the category 'cat' is already exists in the map, return true
	inline bool find_category(Category& cat)
	const
	{ return mMap.find(cat) != mMap.end(); }

	inline size_t count_category()
	const
	{ return mMap.size(); }

	Value get(Category& cat, size_t index)
	const
	{
		// if the category 'cat' is not found, return default value of the 'Value' type
		if(!this->find_category(cat))
			return mDummy;

		assert(index < mMap.at(cat).size());

		return mMap.at(cat)[index];
	}

	bool push_back(Category& cat, Value val)
	{
		if(!this->find_category(cat))
			return false;

		mMap[cat].push_back(val);
		return true;
	}

	bool emplace_back(Category& cat, Value val)
	{
		if(!this->find_category(cat))
			return false;

		mMap[cat].emplace_back(val);
		return true;
	}

	bool insert(Category& cat, typename category_vector::iterator& iterator, Value val)
	{
		if(!this->find_category(cat))
			return false;

		mMap[cat].insert(iterator, val);
		return true;
	}

	void pop_back(Category& cat)
	{ if(this->find_category(cat)) mMap[cat].pop_back(); }

	void erase(Category& cat, typename category_vector::iterator iterator)
	{ if(this->find_category(cat)) mMap[cat].erase(iterator); }

	void erase(Category& cat, typename category_vector::iterator first, typename category_vector::iterator last)
	{ if(this->find_category(cat)) mMap[cat].erase(first, last); }

	inline bool empty()
	const
	{ return mMap.empty(); }

	inline bool empty(Category& cat)
	const
	{ return this->find_category(cat) ? mMap[cat].empty() : true; }

	inline size_t size(Category& cat)
	const
	{ return this->find_category(cat) ? mMap.at(cat).size() : 0; }

	void reserve(Category& cat)
	{ if(this->find_category(cat)) mMap[cat].reserve(); }

	void resize(Category& cat, size_t size)
	{ if(this->find_category(cat)) mMap[cat].resize(size); }

	inline void dummy(Value val)
	{ mDummy = val; }

	inline Value dummy()
	const
	{ return mDummy; }

	/**
	* NOTE : use as...
	* categorized_map<int,std::string> map;
	* //add categories and elements...
	* map.go_round( [](int category, std::string& value) {
	*     std::cout << "category => " << category << " : value => " << value << '\n';
	* });
	*
	* you can take all categories and values in a map!
	*/
	template <typename F>
	void go_round(F lambda)
	{
		for(auto itr_cat = mMap.begin(); itr_cat != mMap.end(); ++itr_cat)
			for(auto itr_val = itr_cat->second.begin(); itr_val != itr_cat->second.end(); ++itr_val)
				lambda(itr_cat->first, *itr_val);
	}

	template <typename F>
	void go_round(F lambda)
	const
	{
		// read-only
		for(auto itr_cat = mMap.cbegin(); itr_cat != mMap.cend(); ++itr_cat)
			for(auto itr_val = itr_cat->second.cbegin(); itr_val != itr_cat->second.cend(); ++itr_val)
				lambda(itr_cat->first, *itr_val);
	}

	// iterator
	inline typename category_vector::iterator begin_of(Category& cat)
	{ return this->find_category(cat) ? mMap[cat].begin() : mDummyCategory.end(); }

	inline typename category_vector::iterator end_of(Category& cat)
	{ return this->find_category(cat) ? mMap[cat].end() : mDummyCategory.end(); }

	inline typename category_vector::const_iterator cbegin_of(Category& cat)
	const
	{ return this->find_category(cat) ? mMap.at[cat].cbegin() : mDummyCategory.cend(); }

	inline typename category_vector::const_iterator cend_of(Category& cat)
	const
	{ return this->find_category(cat) ? mMap.at[cat].cend() : mDummyCategory.cend(); }

	inline typename category_map::iterator begin()
	{ return mMap.begin(); }

	inline typename category_map::iterator end()
	{ return mMap.end(); }

	inline typename category_map::iterator cbegin()
	{ return mMap.cbegin(); }

	inline typename category_map::iterator cend()
	{ return mMap.cend(); }

	// operator
	inline category_vector& operator[](Category& cat)
	{ return mMap.at(cat); }

	inline const category_vector& operator[](Category& cat)
	const
	{ return mMap.at(cat); }

private:
	category_map mMap;
	category_vector mDummyCategory;
	Value mDummy;
};

#endif
