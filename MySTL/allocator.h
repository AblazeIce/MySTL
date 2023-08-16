#pragma once
#include "construct.h"
#include "util.h"

namespace stl
{
	template<class T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	public:
		static T* allocate();
		static T* allocate(size_type n);

		static void deallocate(T* ptr);
		static void deallocate(T* ptr,size_type n);

		static void construct(T* ptr);
		static void construct(T* ptr,const T& value);
		static void construct(T* ptr,T&& value);

		template<class... Args>
		static void construct(T* ptr, Args&& ...args);

		static void destroy(T* ptr);
		static void destroy(T* first,T* last);
	};
	template<class T>
	inline T* allocator<T>::allocate()
	{
		return static_cast<T*>(::operator new(sizeof(T)));
	}
	template<class T>
	inline T* allocator<T>::allocate(size_type n)
	{
		if (n == 0)
			return nullptr;
		return static_cast<T*>(::operator new[](n * sizeof(T)));
	}
	template<class T>
	inline void allocator<T>::deallocate(T* ptr)
	{
		if (ptr == nullptr)
			return;
		::operator delete(ptr);
	}
	template<class T>
	inline void allocator<T>::deallocate(T* ptr, size_type n)
	{
		if (ptr == nullptr)
			return;
		::operator delete[](ptr);
	}
	template<class T>
	inline void allocator<T>::construct(T* ptr)
	{
		stl::construct(ptr);
	}
	template<class T>
	inline void allocator<T>::construct(T* ptr, const T& value)
	{
		stl::construct(ptr, value);
	}
	template<class T>
	inline void allocator<T>::construct(T* ptr, T&& value)
	{
		stl::construct(ptr, stl::move(value));
	}
	template<class T>
	inline void allocator<T>::destroy(T* ptr)
	{
		stl::destroy(ptr);
	}
	template<class T>
	inline void allocator<T>::destroy(T* first, T* last)
	{
		stl::destroy(first, last);
	}
	template<class T>
	template<class ...Args>
	inline void allocator<T>::construct(T* ptr, Args&& ...args)
	{
		stl::construct(ptr, stl::forward<Args>(args)...);
	}
}