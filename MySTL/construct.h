#pragma once
#include "util.h"
namespace stl {

	//------------construct----------------
	template<class T>
	void construct(T* ptr) {
		::new ((void*)ptr) T();
	}
	template<class T>
	void construct(T* ptr, const T& value) {
		::new ((void*)ptr) T(value);
	}
	template<class T,class... Args>
	void construct(T* ptr, Args&&... args) {
		::new ((void*)ptr) T(stl::forward<Args>(args)...);
	}

	//-------------destroy-------------
	template<class T>
	void destroy_one(T*,std::true_type){}
	
	template<class T>
	void destroy_one(T* ptr,std::false_type){
		if (ptr != nullptr)
			ptr->~T();
	}
	template<class T>
	void destroy(T* ptr) {
		destroy_one(ptr, std::is_trivially_destructible<T>{});
	}
	template<class T>
	void destroy_cat(T first,T last,std::true_type){}
	
	template<class T>
	void destroy_cat(T first,T last,std::false_type){
		for (; first != last; ++first)
			destroy(&*first);
	}

	template<class T>
	void destroy(T first, T last) {
		destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<T>::value_type>{});
	}
}