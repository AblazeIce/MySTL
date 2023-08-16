#pragma once
#include <cstddef>
#include <type_traits>
namespace stl
{
	//---------移动语义，左值\右值强制转换为右值------------
	template<class T>
	typename std::remove_reference<T>::type&& move(T&& arg) noexcept {//T&&:万能引用
		return static_cast<typename std::remove_reference<T>::type &&>(arg);
	}
	//---------完美转发，左值就是左值，右值就是右值----------
	//-----------------模板的引用折叠---------------------
	template<class T>
	T&& forward(typename std::remove_reference<T>::type & arg) noexcept {
		return static_cast<T&&>(arg);
	}
	template<class T>
	T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
		//断言，参数只能为右值引用
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(arg);
	}

	//------swap---------
	template<class Tp>
	void swap(Tp& lhs, Tp& rhs) {
		auto temp(stl::move(lhs));//lhs资源被掏空，赋给temp
		lhs = stl::move(rhs);//rhs资源被掏空，赋给lhs
		rhs = stl::move(temp);//temp资源被掏空，赋给rhs
	}
	template<class ForwardIter1,class ForwardIter2>
	ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter2 last1, ForwardIter2 first2) {
		for (; first1 != last1; ++first1, (void)++first2)
			swap(*first1, *first2);
		return first2;
	}
	template<class Tp,size_t N>
	void swap(Tp(&a)[N], Tp(&b)[N]) {
		swap_range(a, a + N, b);
	}

}