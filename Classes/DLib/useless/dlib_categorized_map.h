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
	// if the category 'cat' is already exists in the map, return true
	inline bool find_category(Category& cat)
	const
	{ return mMap.find(cat) != mMap.end(); }

	inline bool is_empty()
	const
	{ return mMap.empty(); }

	inline bool is_empty(Category& cat)
	const
	{ return this->find_category(cat) ? mMap.at(cat).empty() : true; }

	inline size_t count_category()
	const
	{ return mMap.size(); }

	// if the category 'cat' is not found in a map, return (size_t)-1
	inline size_t size_of(Category& cat)
	const
	{ return this->find_category(cat) ? mMap.at(cat).size() : static_cast<size_t>(-1); }

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
	template <typename L>
	void go_round(L lambda)
	{
		for(auto itr_cat = mMap.begin(); itr_cat != mMap.end(); ++itr_cat)
			for(auto itr_val = itr_cat->second.begin(); itr_val != itr_cat->second.end(); ++itr_val)
				lambda(itr_cat->first, *itr_val);
	}

	template <typename L>
	void go_round(L lambda)
	const
	{
		// read-only
		for(auto itr_cat = mMap.cbegin(); itr_cat != mMap.cend(); ++itr_cat)
			for(auto itr_val = itr_cat->second.cbegin(); itr_val != itr_cat->second.cend(); ++itr_val)
				lambda(itr_cat->first, *itr_val);
	}

	// iterator
	inline typename category_map::iterator begin()
	{ return mMap.begin(); }

	inline typename category_map::iterator end()
	{ return mMap.end(); }

	inline typename category_map::const_iterator cbegin()
	{ return mMap.cbegin(); }

	inline typename category_map::const_iterator cend()
	{ return mMap.cend(); }

	// operator
	inline category_vector& operator[](Category& cat)
	{ return mMap[cat]; }

	inline const category_vector& operator[](Category& cat)
	const
	{ return mMap.at(cat); }

	inline category_map& operator()()
	{ return mMap; }

	inline const category_map& operator()()
	const
	{ return mMap; }

private:
	category_map mMap;
};

#endif
