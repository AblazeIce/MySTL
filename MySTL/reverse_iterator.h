#pragma once
#include "iterator.h"
namespace stl
{
	template<class Iterator>
	class reverse_iterator {
	private:
		Iterator current;//记录正向迭代器
	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;

		typedef Iterator                                              iterator_type;
		typedef reverse_iterator<Iterator>                            self;
	public:
		reverse_iterator(){}
		explicit reverse_iterator(iterator_type i):current(i){}
		reverse_iterator(const self& rhs):current(rhs.current){}
	public:
		// 取出对应的正向迭代器
		iterator_type base() const
		{
			return current;
		}

		reference operator*() const {
			//实际对应正向迭代器的前一个位置
			auto temp = current;
			return *--temp;
		}
		pointer operator->() const {
			return &(operator*());
		}
		//前增++
		self& operator++() {
			--current;
			return *this;
		}
		//后增++
		self operator++() {
			self temp = *this;
			--current;
			return temp;
		}
		// 前减--
		self& operator--()
		{
			++current;
			return *this;
		}
		//后减--
		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}
		self& operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self operator+(difference_type n)const {
			return self(current - n);
		}
		self& operator-=(difference_type n) {
			current += n;
			return *this;
		}
		self operator-(difference_type n)const {
			return self(current + n);
		}
		reference operator[](difference_type n)const {
			return *(*this + n);
		}
	};
	// 重载 operator-
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}
	// 重载比较操作符
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}
	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() < lhs.base();
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs == rhs);
	}
	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return rhs < lhs;
	}
	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs < rhs);
	}
}