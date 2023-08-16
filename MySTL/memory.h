#pragma once
namespace stl
{
	template<class T>
	constexpr T* address_of(T& value)noexcept {
		return &value;
	}
}