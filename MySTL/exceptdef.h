#pragma once
#include <stdexcept>
#include <cassert>
namespace stl
{
#define	STL_DEBUG(expr) assert(expr)

#define THROW_OUT_OF_RANGE_IF(expr,what)\
	if(!(expr))throw std::out_of_range(what)

#define THROW_LENGTH_ERROR_IF(expr,what)\
	if(!(expr))throw std::range_error(what)

#define THROW_RUNTIME_ERROE_IF(expr,what)\
	if(!(ecpr))throw std::runtime_error(what)
}