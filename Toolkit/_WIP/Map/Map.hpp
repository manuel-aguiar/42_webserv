

#ifndef MAP_HPP

# define MAP_HPP

#include <functional>
#include <memory>

template <
	typename Key,
	typename Value,
	typename Compare = std::less<Key>,
	typename Allocator = std::allocator<std::pair<const Key, Value> >
>
class Map;

# include "Map.tpp"

#endif
