
#ifndef WEBSERVER_DYNARRAY_SVO_HPP

# define WEBSERVER_DYNARRAY_SVO_HPP

#include <memory>
template <typename T, size_t stackBuf = 10, typename Allocator = std::allocator<T> >
class DynArraySVO;

# include "DynArraySVO.tpp"

#endif
