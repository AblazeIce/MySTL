#pragma once
#include "algobase.h"
#include "construct.h"
namespace stl
{
	template<class ForwardIter, class Size, class T>
	ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type)
	{
		return ::fill_n(first, n, value);
	}

	template<class ForwardIter,class Size,class T>
	ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type) {
		auto cur = first;
		try
		{
			for (; n > 0; --n, ++cur) {
				::construct(cur, value);
			}
		}
		catch (...)
		{
			for (; first!=cur; ++first) {
				::destroy(first);
			}
		}
		return cur;
	}

	template<class ForwardIter,class Size,class T>
	ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
		return unchecked_uninit_fill_n(first, n, value, std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
	}

	template<class InputIter,class ForwardIter>
	ForwardIter unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return ::copy(first, last, result);
	}
	template<class InputIter,class ForwardIter>
	ForwardIter unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		auto cur = result;
		try
		{
			for (; first != last; ++first, ++cur) {
				::construct(cur, ::move(*first));
			}
		}
		catch (...)
		{
			for (; result != cur; --cur) {
				::destroy(cur);
			}
		}
		return cur;
	}
	template<class InputIter,class ForwardIter>
	ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
		return unchecked_uninit_copy(first, last,result,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
	}

	template<class InputIter,class ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return ::move(first, last, result);
	}
	template<class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		ForwardIter cur = result;
		try
		{
			for (; first != last; ++first, ++cur) {
				::construct(cur, ::move(*first));
			}
		}
		catch (...)
		{
			::destroy(result, cur);
		}
		return cur;
	}
	template<class InputIter,class ForwardIter>
	ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
		return  unchecked_uninit_move(first, last, result, std::is_trivially_move_assignable<
			typename iterator_traits<InputIter>::value_type>());
	}
}