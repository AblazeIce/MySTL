#pragma once
#include <initializer_list>
#include "iterator.h"
#include "reverse_iterator.h"
#include "allocator.h"
#include "uninitialized.h"
#include "exceptdef.h"
#include "memory.h"
#include "algo.h"
namespace stl
{

	template<class T>
	class vector
	{
	public:
		typedef stl::allocator<T> allocator_type;
		typedef stl::allocator<T> data_allocator;

		typedef typename allocator_type::value_type		 value_type;
		typedef typename allocator_type::pointer		 pointer;
		typedef typename allocator_type::const_pointer	 const_pointer;
		typedef typename allocator_type::reference		 reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type		 size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef stl::reverse_iterator<iterator> reverse_iterator;
		typedef stl::reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		allocator_type get_allocator() { return data_allocator; }

	private:
		iterator begin_;
		iterator end_;
		iterator cap_;
	public:
		vector()noexcept {
			try_init();
		}
		explicit vector(size_type n) {
			fill_init(n, value_type{});
		}
		vector(size_type n, const value_type& value) {
			fill_init(n, value);
		}
		template<class Iter>
		vector(Iter first, Iter last) {
			STL_DEBUG(first < last);
			range_init(first, last);
		}
		vector(const vector& lhs) {//拷贝构造
			range_init(lhs.begin(), lhs.end());
		}
		vector(vector&& rhs)noexcept//移动构造
			:begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_)
		{
			rhs.begin_ = rhs.end_ = rhs.cap_ = nullptr;
		}
		vector(std::initializer_list<value_type> ilist) {//接受花括号初始化器列表列表初始化对象
			range_init(ilist.begin(), ilist.end());
		}
		vector& operator=(const vector& lhs);
		vector& operator=(vector&& rhs) noexcept;
		vector& operator=(std::initializer_list<value_type> ilist);
		~vector() {
			destroy_and_recover(begin_, end_, cap_ - begin_);
			begin_ = end_ = cap_ = nullptr;
		}
	public:


		iterator begin()noexcept { return begin_; }
		const_iterator begin()const noexcept { return begin_; }
		iterator end()noexcept { return end_; }
		const_iterator end()const noexcept { return end_; }

		reverse_iterator rbegin()noexcept { return reverse_iterator(end_); }
		const_reverse_iterator rbegin()const noexcept { return const_reverse_iterator(end_); }
		reverse_iterator rend()noexcept { return reverse_iterator(begin_); }
		const_reverse_iterator rend()const noexcept { return const_reverse_iterator(begin_); }
		const_iterator cbegin()const noexcept { return begin_; }
		const_iterator cend()const noexcept { return end_; }
		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator crend()   const noexcept
		{
			return rend();
		}
	public:
		bool empty() const noexcept { return begin_ == end_; }
		size_type size()const noexcept { return static_cast<size_type>(end_ - begin_); }
		//size_t下能分配的最大容量
		size_type max_size()const noexcept { return static_cast<size_type>(-1) / sizeof(T); }
		size_type capacity()const noexcept { return static_cast<size_type>(cap_ - begin_); }
		//重新分配n个空间
		void reserve(size_type n);
		//放弃多余容量
		void shrink_to_fit();
	public:
		reference operator[](size_type n) {
			STL_DEBUG(n < size());
			return *(begin_ + n);
		}
		const_reference operator[](size_type n)const {
			STL_DEBUG(n < size());
			return *(begin_ + n);
		}
		reference at(size_type n) {
			THROW_OUT_OF_RANGE_IF(n < size(), "Vector<T>::at() subscript out of range!");
			return (*this)[n];
		}
		const_reference at(size_type n)const {
			THROW_OUT_OF_RANGE_IF(n < size(), "Vector<T>::at() subscript out of range!");
			return (*this)[n];
		}
		reference front() { 
			STL_DEBUG(!empty());
			return *begin_;
		}
		const_reference front() const{ 
			STL_DEBUG(!empty());
			return *begin_; 
		}
		reference back() { 
			STL_DEBUG(!empty());
			return *(end_-1);
		}
		const_reference back() const{ 
			STL_DEBUG(!empty());
			return *(end_-1); 
		}
		pointer data() { return begin_; }
		const_pointer data() const{ return begin_; }
	public:
		void assign(size_type n, const value_type& value) {
			fill_assign(n, value);
		}
		template<class Iter, typename std::
			enable_if<stl::is_input_iterator<Iter>::value, int>::type = 0>
		void assign(Iter first,Iter last){
			STL_DEBUG(first<last);
			copy_assign(first, last, stl::iterator_category(first));
		}
		void assign(std::initializer_list<value_type> ilist) {
			copy_assign(ilist.begin(), ilist.end(),stl::forward_iterator_tag());
		}
		template<class... Args>
		iterator emplace(const_iterator pos, Args&& ...args);

		template<class... Args>
		void emplace_back(Args&& ...args);

		void push_back(const value_type& value);
		void push_back(value_type&& value) {
			emplace_back(stl::forward<value_type>(value));
		}

		void pop_back();

		iterator insert(const_iterator pos, const value_type& value);
		iterator insert(const_iterator pos, value_type&& value) {
			return emplace(pos, stl::forward<value_type>(value));
		}
		iterator insert(const_iterator pos, size_type n, const value_type& value) {
			STL_DEBUG(pos >= begin_ && pos <= end_);
			return fill_insert(const_cast<iterator>(pos), n, value);
		}
		
		//编译时决策，若为输入迭代器，则第二模板参数为int=0；若不为输入迭代器，则报错
		template<class Iter,typename 
			std::enable_if<stl::is_input_iterator<Iter>::value,int>::type=0>
		void insert(const_iterator pos, Iter first, Iter last) {
			STL_DEBUG(pos >= begin_ && pos <= end_ && first < last);
			copy_insert(const_cast<iterator>(pos), first, last);
		}

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first,const_iterator second);
		inline void clear() { erase(begin_, end_); }

		void resize(size_type new_size) { resize(new_size, value_type()); }
		void resize(size_type new_size,const value_type& value);
		void reverse() {stl::reverse(begin_, end_);}
		void swap(vector& lhs)noexcept;//直接交换指针

	private:
		void try_init()noexcept;//分配16个容量，不抛出异常
		void fill_init(size_type n, const value_type& value);//分配容量，抛出异常
		void init_space(size_type size, size_type cap);
		template<class Iter>
		void range_init(Iter first, Iter last);
		void destroy_and_recover(iterator first, iterator last, size_type n);//析构再释放内存

		size_type get_new_gap(size_type add_size);//返回开辟空间大小：开始开辟16个，后面2倍增长
		void fill_assign(size_type n, const value_type& value);

		template<class InputIter>
		void copy_assign(InputIter first, InputIter last, input_iterator_tag);
		template<class ForwardIter>
		void copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag);

		template<class... Args>
		void reallocate_emplace(iterator pos, Args&& ...args);
		void reallocate_insert(iterator pos, const value_type& value);

		iterator fill_insert(iterator pos, size_type n, const value_type& value);
		template<class InputIter>
		void copy_insert(iterator pos, InputIter first, InputIter last);
		void reinsert(size_type size);
	};


	template<class T>
	inline void vector<T>::fill_init(size_type n, const value_type& value)
	{
		const size_type init_size = max(static_cast<size_type>(16), n);
		init_space(n, init_size);//new 空间
		stl::uninitialized_fill_n(begin_, n, value);//调用构造函数进行填充
	}
	template<class T>
	inline vector<T>& vector<T>::operator=(const vector& lhs)
	{

		if (this != &lhs) {
			const auto length = lhs.size();
			if (length > capacity()) {
				vector temp(lhs.begin_, lhs.end_);//拷贝一份局部变量，结束后自动析构
				swap(temp);
			}
			else if (size() >= length) {
				auto i = stl::copy(lhs.begin_, lhs.end_, begin_);//先替换原先已有的对象
				data_allocator::destroy(i, end_);//再析构掉原有多余的对象
				end_ = begin_ + length;
			}
			else {
				stl::copy(lhs.begin_, lhs.begin_ + size(), begin_);//先替换原先已有的对象
				end_ = stl::uninitialized_copy(lhs.begin_ + size(), lhs.end_, end_);//再构建多的塞不下的对象
			}
		}
		return *this;
	}
	template<class T>
	inline vector<T>& vector<T>::operator=(vector&& rhs) noexcept
	{
		destroy_and_recover(begin_, end_, capacity());
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = rhs.end_ = rhs.cap_ = nullptr;
		return *this;
	}
	template<class T>
	inline vector<T>& vector<T>::operator=(std::initializer_list<value_type> ilist)
	{
		vector temp(ilist.begin(),ilist.end());
		swap(temp);
		return *this;
	}
	template<class T>
	inline void vector<T>::reserve(size_type n)
	{
		if (capacity() < n) {
			THROW_LENGTH_ERROR_IF(n < max_size(), "n can not larger than max_size() in vector<T>::reserve(n)");
			const auto old_size = size();
			auto temp = data_allocator::allocate(n);
			stl::uninitialized_move(begin_, end_, temp);
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = temp;
			end_ = temp + old_size;
			cap_ = begin_ + n;
		}
	}
	template<class T>
	inline void vector<T>::shrink_to_fit()
	{
		if (end_ < cap_) {
			reinsert(size());
		}
	}
	template<class T>
	inline void vector<T>::push_back(const value_type& value)
	{
		if (end_ != cap_) {
			data_allocator::construct(end_, value);
			++end_;
		}else{
			reallocate_insert(end_, value);
		}
	}
	
	template<class T>
	inline void vector<T>::pop_back()
	{
		STL_DEBUG(!empty());
		data_allocator::destroy(end_ - 1);
		--end_;
	}

	template<class T>
	inline typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value)
	{
		STL_DEBUG(pos >= begin_ && pos <= end_);

		iterator xpos = const_cast<iterator>(pos);
		const size_type n = pos - begin_;
		if (end_ != cap_ && xpos == end_)
		{
			data_allocator::construct(stl::address_of(*end_), value);
			++end_;
		}
		else if (end_ != cap_)
		{
			auto new_end = end_;
			data_allocator::construct(stl::address_of(*end_), *(end_ - 1));
			++new_end;
			auto value_copy = value;  // 避免元素因以下复制操作而被改变
			stl::copy_backward(xpos, end_ - 1, end_);
			*xpos = stl::move(value_copy);
			end_ = new_end;
		}
		else
		{
			reallocate_insert(xpos, value);
		}
		return begin_ + n;
	}

	template<class T>
	inline typename vector<T>::iterator vector<T>::erase(const_iterator pos)
	{
		STL_DEBUG(pos >= begin_ && pos < end_);
		iterator xpos = begin_ + (pos - begin());
		stl::move(xpos + 1, end_, xpos);
		data_allocator::destroy(end_ - 1);
		--end_;
		return xpos;
	}
	template<class T>
	inline typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
	{
		STL_DEBUG(first >= begin_ && last <= end_ && first < last);
		iterator xfirst = const_cast<iterator>(first);
		iterator xlast = const_cast<iterator>(last);
		stl::move(xlast, end_, xfirst);
		data_allocator::destroy(xlast, end_);
		end_ = end_ - (xlast - xfirst);
		return xfirst;
	}
	template<class T>
	inline void vector<T>::resize(size_type new_size, const value_type& value)
	{
		if (new_size < size()) {
			erase(begin_ + new_size, end_);
		}
		else
		{
			insert(end_, new_size - size(), value);
		}
	}

	template<class T>
	inline void vector<T>::swap(vector& lhs) noexcept
	{
		if (this != &lhs) {
			stl::swap(begin_, lhs.begin_);
			stl::swap(end_, lhs.end_);
			stl::swap(cap_, lhs.cap_);
		}
	}
	
	template<class T>
	inline void vector<T>::try_init() noexcept
	{
		try
		{
			begin_ = data_allocator::allocate(16);
			end_ = begin_;
			cap_ = begin_ + 16;
		}
		catch (...)
		{
			begin_ = end_ = cap_ = nullptr;
		}
	}
	template<class T>
	inline void vector<T>::init_space(size_type size, size_type cap)
	{
		try
		{
			begin_ = data_allocator::allocate(cap);
			end_ = begin_ + size;
			cap_ = begin_ + cap;
		}
		catch (...)
		{
			begin_ = end_ = cap_ = nullptr;
			throw;
		}
	}
	template<class T>
	inline void vector<T>::destroy_and_recover(iterator first, iterator last, size_type n)
	{
		data_allocator::destroy(first, last);
		data_allocator::deallocate(first, n);
	}

	template<class T>
	inline typename vector<T>::size_type vector<T>::get_new_gap(size_type add_size)
	{
		const auto old_size = capacity();
		THROW_LENGTH_ERROR_IF(old_size + add_size < max_size(), "vector<T>'s size too big!");

		if (2*old_size > max_size()) {//空间不够两倍增长，则改为16个增长
			return old_size + add_size > max_size() - 16 ?
				old_size + add_size : old_size + add_size + 16;
		}
		return old_size == 0 ? stl::max(add_size, static_cast<size_type>(16))
			: (stl::max(2*old_size, old_size + add_size));
	}

	template<class T>
	inline void vector<T>::fill_assign(size_type n, const value_type& value)
	{
		if (n > capacity()) {
			vector temp(n, value);
			swap(temp);
		}
		else if (n > size()) {
			stl::fill(begin_, end_, value);
			end_=stl::uninitialized_fill_n(end_, n - size(), value);
		}
		else {
			erase(stl::fill_n(begin_, n, value), end_);
		}
	}

	template<class T>
	inline void vector<T>::reallocate_insert(iterator pos, const value_type& value)
	{
		const auto new_size = get_new_gap(1);
		auto new_begin = data_allocator::allocate(new_size);//开辟1.5倍新空间
		auto new_end = new_begin;
		try
		{
			new_end=stl::uninitialized_move(begin_, pos, new_begin);//复制pos前原先数据
			data_allocator::construct(new_end, value);//拷贝构造新数据
			++new_end;
			new_end=stl::uninitialized_move(pos, end_, new_end);//复制pos后原先数据
		}	
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);//确保释放原先内存
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template<class T>
	inline typename vector<T>::iterator vector<T>::fill_insert(iterator pos, size_type n, const value_type& value)
	{
		if (n == 0)
			return pos;
		const size_type xpos = pos - begin_;
		const value_type value_copy = value;  // 避免被覆盖
		if (cap_ - end_ >=n) {
			const size_type after_elems = end_ - pos;
			auto old_end = end_;
			if (after_elems > n) {
				stl::uninitialized_copy(end_ - n, end_, end_);
				end_ += n;
				stl::move_backward(pos, old_end - n, old_end);
				stl::uninitialized_fill_n(pos, n, value_copy);
			}
			else
			{
				end_ = stl::uninitialized_fill_n(end_, n - after_elems, value_copy);
				end_ = stl::uninitialized_move(pos, old_end, end_);
				stl::uninitialized_fill_n(pos, after_elems, value_copy);
			}
		}
		else
		{
			const auto new_size = get_new_gap(n);
			auto new_begin = data_allocator::allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = stl::uninitialized_move(begin_, pos, new_begin);
				new_end = stl::uninitialized_fill_n(new_end, n, value);
				new_end = stl::uninitialized_move(pos, end_, new_end);
			}
			catch (...)
			{
				destroy_and_recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = new_begin + new_size;
		}
		return begin_ + xpos;
	}

	template<class T>
	inline void vector<T>::reinsert(size_type size)
	{
		auto new_begin = data_allocator::allocate(size);
		try
		{
			stl::uninitialized_move(begin_, end_, new_begin);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, size);
			throw;
		}
		data_allocator::deallocate(begin_, cap_ - begin_);
		begin_ = new_begin;
		end_ = begin_ + size;
		cap_ = begin_ + size;
	}

	template<class T>
	template<class ...Args>
	inline typename vector<T>::iterator vector<T>::emplace(const_iterator pos, Args && ...args)//模板内&&:万能引用
	{
		STL_DEBUG(begin_ <= pos && pos <= end_);
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = xpos - begin_;
		if (end_ != cap_ && xpos == end_) {
			data_allocator::construct(end_, stl::forward<Args>(args)...);//移动构造新数据
			++end_;
		}
		else if (end_ != cap_) {
			auto new_end = end_;
			data_allocator::construct(end_, *(end_ - 1));
			++new_end;
			stl::copy_backward(xpos, end_ - 1, end_);
			*xpos = value_type(stl::forward<Args>(args)...);
			end_ = new_end;
		}
		else
		{
			reallocate_emplace(xpos, stl::forward<Args>(args)...);
		}
		return begin_ + n;
	}

	template<class T>
	template<class ...Args>
	void vector<T>::emplace_back(Args&& ...args)
	{
		if (end_ < cap_) {
			data_allocator::construct(end_, stl::forward<Args>(args)...);
			++end_;
 		}
		else {
			reallocate_emplace(end_, stl::forward<Args>(args)...);
		}
	}

	template<class T>
	template<class Iter>
	inline void vector<T>::range_init(Iter first, Iter last)
	{
		const size_type length = stl::distance(first, last);
		const size_type init_size = stl::max(length, static_cast<size_type>(16));
		init_space(length, init_size);
		stl::uninitialized_copy(first, last, begin_);
	}

	template<class T>
	template<class InputIter>
	inline void vector<T>::copy_assign(InputIter first, InputIter last, input_iterator_tag)
	{
		auto cur = begin_;
		for (; first != last && cur != end_; ++first, ++cur) {
			*cur = *first;
		}
		if (first == last) {
			erase(cur, end_);
		}
		else {
			insert(end_, first, last);
		}
	}
	template<class T>
	template<class ForwardIter>
	inline void vector<T>::copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag)
	{
		const size_type len = stl::distance(first, last);
		if (len > capacity())
		{
			vector tmp(first, last);
			swap(tmp);
		}
		else if (size() >= len)
		{
			auto new_end = stl::copy(first, last, begin_);
			data_allocator::destroy(new_end, end_);
			end_ = new_end;
		}
		else
		{
			auto mid = first;
			stl::advance(mid, size());
			stl::copy(first, mid, begin_);
			auto new_end = stl::uninitialized_copy(mid, last, end_);
			end_ = new_end;
		}
	}
	//重新分配空间，并且在pos处移动构造构造元素
	template<class T>
	template<class ...Args>
	inline void vector<T>::reallocate_emplace(iterator pos, Args && ...args)
	{
		const auto new_size = get_new_gap(1);
		auto new_begin = data_allocator::allocate(new_size);
		auto new_end = new_begin;
		try
		{
			new_end = stl::uninitialized_move(begin_, pos, new_begin);//移动pos前的数据
			data_allocator::construct(new_end, stl::forward<Args>(args)...);//在pos处移动构造新构建
			++new_end;
			new_end = stl::uninitialized_move(pos, end_, new_end);//移动pos后的数据
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);//释放原先内存
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}
	template<class T>
	template<class InputIter>
	inline void vector<T>::copy_insert(iterator pos, InputIter first, InputIter last)
	{
		if (first == last)
			return;
		const auto n = ::distance(first, last);
		if ((cap_ - end_) >= n)
		{ // 如果备用空间大小足够
			const auto after_elems = end_ - pos;
			auto old_end = end_;
			if (after_elems > n)
			{
				end_ = stl::uninitialized_copy(end_ - n, end_, end_);
				stl::move_backward(pos, old_end - n, old_end);
				stl::uninitialized_copy(first, last, pos);
			}
			else
			{
				auto mid = first;
				stl::advance(mid, after_elems);
				end_ = stl::uninitialized_copy(mid, last, end_);
				end_ = stl::uninitialized_move(pos, old_end, end_);
				stl::uninitialized_copy(first, mid, pos);
			}
		}
		else
		{ // 备用空间不足
			const auto new_size = get_new_gap(n);
			auto new_begin = data_allocator::allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = stl::uninitialized_move(begin_, pos, new_begin);
				new_end = stl::uninitialized_copy(first, last, new_end);
				new_end = stl::uninitialized_move(pos, end_, new_end);
			}
			catch (...)
			{
				destroy_and_recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_size;
		}
	}

}