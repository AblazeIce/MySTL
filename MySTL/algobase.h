#pragma once
#include <cstring>
#include "iterator.h"
#include "reverse_iterator.h"
#include "util.h"

namespace stl
{
	template<class T>
	const T& max(const T& lhs, const T& rhs) {
		return lhs < rhs ? rhs : lhs;//相等时，保证返回第一个
	}

	template<class T, class Compare>
	const T& max(const T& lhs, const T& rhs, Compare compare) {
		return compare(lhs, rhs) ? rhs : lhs;
	}

	template<class T>
	const T& min(const T& lhs, const T& rhs) {
		return rhs < lhs ? rhs : lhs;//相等时，保证返回第一个
	}

	template<class T, class Compare>
	const T& min(const T& lhs, const T& rhs, Compare compare) {
		return compare(rhs, lhs) ? rhs : lhs;
	}



	template<class ForwardIter1, class ForwardIter2>
	void iter_swap(ForwardIter1 lhs, ForwardIter2 rhs) {
		stl::swap(*lhs, *rhs);
	}

	template<class InputIter, class OutputIter>
	OutputIter unchecked_copy_cat(InputIter first, InputIter last, 
		OutputIter result, input_iterator_tag) {
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	template<class RandomIter, class OutputIter>
	OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, 
		OutputIter result, random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	template<class InputIter, class OutputIter>
	OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result) {
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_copy_assignable<Up>::value,
		Up*>::type
		unchecked_copy(Tp* first, Tp* last, Up* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}
	template<class InputIter, class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result) {
		return unchecked_copy(first, last, result);
	}

	//bidirectional_iterator_tag 版本
	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first,
		BidirectionalIter1 last, BidirectionalIter2 result, bidirectional_iterator_tag) {
		while (first != last) {
			*--result = *--last;
		}
		return result;
	}

	//random_access_iterator_tag 版本
	template<class RandomIter1,class BidirectionalIter2>
	BidirectionalIter2 unchecked_copy_backward_cat(RandomIter1 first, 
		RandomIter1 last, BidirectionalIter2 result, random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n) {
			*--result = *--last;
		}
		return result;
	}

	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first, 
		BidirectionalIter1 last, BidirectionalIter2 result) {
		return unchecked_copy_backward_cat(first, last,result,iterator_category(first));
	}
	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_copy_assignable<Up>::value,
		Up*>::type
		unchecked_copy_backward(Tp* first, Tp* last, Up* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
		{
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}
	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter2 copy_backward(BidirectionalIter1 first,
		BidirectionalIter1 last, BidirectionalIter2 result) {
		return unchecked_copy_backward(first, last, result);
	}

	//---------------一元操作----------------------------
	template<class InputIter,class OutputIter,class UnaryPredicate>
	OutputIter copy_if(InputIter first, InputIter last, 
		OutputIter result, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))
				*result++ = *first;
		}
		return result;
	}

	/*template<class InputIter,class Size,class OutputItsr>
	pair<InputIter, OutputItsr> unchecked_copy_n_cat(InputIter first, Size n,
		OutputItsr result, input_iterator_tag) {
		for (; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return pair<InputIter, OutputItsr>(first, result);
	}

	template<class RandomIter, class Size, class OutputItsr>
	pair<RandomIter, OutputItsr> unchecked_copy_n_cat(RandomIter first, Size n,
		OutputItsr result, random_access_iterator_tag) {
		auto last = first + n;
		return pair<RandomIter, OutputItsr>(first, copy(first,last,result));
	}

	template<class InputIter,class Size,class OutputIter>
	pair<InputIter, OutputIter>unchecked_copy_n(InputIter first, Size n, OutputIter result) {
		return unchecked_copy_n_cat(first, n, result, iterator_category(first));
	}
	template<class InputIter, class Size, class OutputIter>
	pair<InputIter, OutputIter>copy_n(InputIter first, Size n, OutputIter result) {
		return unchecked_copy_n(first, n, result);
	}*/

	template<class InputIter,class OutputIter>
	OutputIter unchecked_move_cat(InputIter first, InputIter last,
		OutputIter result, input_iterator_tag) {
		for (; first != last; ++first, ++last) {
			*result = stl::move(*first);
		}
		return result;
	}
	template<class RandomIter,class OutputIter>
	OutputIter unchecked_move_cat(RandomIter first, RandomIter last,
		OutputIter result, random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n, ++first, ++result) {
			*result = ::move(*first);
		}
		return result;
	}

	template<class InputIter,class OutputIter>
	OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result) {
		return unchecked_move_cat(first, last, result, iterator_category(first));
	}
	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_move_assignable<Up>::value,
		Up*>::type
		unchecked_move(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}
	template<class InputIter,class OutputIter>
	OutputIter move(InputIter first, InputIter last, OutputIter result) {
		return unchecked_move(first, last, result);
	}

	//bidirectional_iterator_tag 版本
	template<class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first,
		BidirectionalIter1 last, BidirectionalIter2 result, bidirectional_iterator_tag) {
		while (first != last) {
			*--result = move(*--last);
		}
		return result;
	}

	//random_access_iterator_tag 版本
	template<class RandomIter1, class RandomIter2>
	RandomIter2 unchecked_move_backward_cat(RandomIter1 first,
		RandomIter1 last, RandomIter2 result, random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n) {
			*--result = move(*--last);
		}
		return result;
	}

	template<class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first,
		BidirectionalIter1 last, BidirectionalIter2 result) {
		return unchecked_move_backward_cat(first, last, result, iterator_category(first));
	}
	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_move_assignable<Up>::value,
		Up*>::type
		unchecked_move_backward(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
		{
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}
	template<class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2 move_backward(BidirectionalIter1 first,
		BidirectionalIter1 last, BidirectionalIter2 result) {
		return unchecked_move_backward(first, last, result);
	}

	template<class InputIter1,class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			if (*first1 != *first2) {
				return false;
			}
		}
		return true;
	}
	template<class InputIter1,class InputIter2,class Compare>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2,Compare compare) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!compare(*first1,*first2)) {
				return false;
			}
		}
		return true;
	}

	template<class OutputIter,class Size,class T>
	OutputIter unchecked_fill_n(OutputIter first, Size n,const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}
	// 为 one-byte 类型提供特化版本
	template <class Tp, class Size, class Up>
	typename std::enable_if<
		std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
		!std::is_same<Tp, bool>::value&&
		std::is_integral<Up>::value && sizeof(Up) == 1,
		Tp*>::type
		unchecked_fill_n(Tp* first, Size n, Up value)
	{
		if (n > 0)
		{
			std::memset(first, (unsigned char)value, (size_t)(n));
		}
		return first + n;
	}
	template<class OutputIter,class Size,class T>
	OutputIter fill_n(OutputIter first, Size n, const T& value) {
		return unchecked_fill_n(first, n, value);
	}

	template<class ForwardIter,class T>
	void fill_cat(ForwardIter first, ForwardIter last, const T& value,forward_iterator_tag) {
		for (; first != last; ++first) {
			*first = value;
		}
	}

	template<class RandomIter,class T>
	void fill_cat(RandomIter first, RandomIter last, const T& value, random_access_iterator_tag) {
		fill_n(first, last - first, value);
	}

	template<class ForwardIter,class T>
	void fill(ForwardIter first, ForwardIter last, const T& value) {
		fill_cat(first, last, value,iterator_category(first));
	}
}