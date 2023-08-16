#pragma once
#include <cstddef>
#include <type_traits>
namespace stl
{
	//---------�ƶ����壬��ֵ\��ֵǿ��ת��Ϊ��ֵ------------
	template<class T>
	typename std::remove_reference<T>::type&& move(T&& arg) noexcept {//T&&:��������
		return static_cast<typename std::remove_reference<T>::type &&>(arg);
	}
	//---------����ת������ֵ������ֵ����ֵ������ֵ----------
	//-----------------ģ��������۵�---------------------
	template<class T>
	T&& forward(typename std::remove_reference<T>::type & arg) noexcept {
		return static_cast<T&&>(arg);
	}
	template<class T>
	T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
		//���ԣ�����ֻ��Ϊ��ֵ����
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(arg);
	}

	//------swap---------
	template<class Tp>
	void swap(Tp& lhs, Tp& rhs) {
		auto temp(stl::move(lhs));//lhs��Դ���Ϳգ�����temp
		lhs = stl::move(rhs);//rhs��Դ���Ϳգ�����lhs
		rhs = stl::move(temp);//temp��Դ���Ϳգ�����rhs
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