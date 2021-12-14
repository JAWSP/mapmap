/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_bool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 12:30:03 by juhpark           #+#    #+#             */
/*   Updated: 2021/10/22 20:14:22 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <iostream>
# include "random_access_iterator.hpp"
# include "utils.hpp" 


namespace ft
{
	//------------------------- 찐 벡터 -------------------------------------

	template<typename Alloc = std::allocator<bool> >
	class vector : protected vector_base<bool, Alloc>
	{
	private:
		typedef vector_base<bool, Alloc> Base; //base그자체
		typedef typename Base::rebind_alloc R_allocator_type;//rebind 할당자
		R_allocator_type Ra;

	public:
		typedef bool value_type;
		typedef Alloc allocator_type;
		typedef typename R_allocator_type::pointer pointer;
		typedef typename R_allocator_type::const_pointer const_pointer;
		typedef typename R_allocator_type::reference reference;
		typedef typename R_allocator_type::const_reference const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef ft::random_access_iterator<value_type> iterator;
		typedef ft::random_access_iterator<const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

//------------------------------ 별칭화 끝남 --------------------------------

		explicit vector() : Base()//디폴트
		{
			Ra = Base::transform_allocator(); 
			std::cout << "asd\n";
		}
		explicit vector(const allocator_type& A) : Base(A)//커스텀 할당자
		{
			Ra = Base::transform_allocator(); 
			std::cout << "custom vector spawn\n";
		}
		explicit vector(size_type size, const value_type& value = value_type(),
				const allocator_type& A = allocator_type()) : Base(size, A)
		{
			Ra = Base::transform_allocator(); 
			try
			{
				for (; size > 0; --size, this->Base::imp.M_finish++)
					Ra.construct(this->Base::imp.M_finish, value);
			}
			catch (...)
			{
				this->clear();
			}
		}
		
		template<class InputIterator>
		vector(InputIterator first, InputIterator last,
				const allocator_type& A = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::result, InputIterator>::type* = nullptr) : Base(A)
		{
			typename ft::iterator_traits<InputIterator>::difference_type dif;
			dif = last - first;
			Ra = Base::transform_allocator(); 
			this->Base::imp.M_start = Ra.allocate(dif);
			this->Base::imp.M_finish = this->Base::imp.M_start;
			this->Base::imp.M_eos = this->Base::imp.M_start + dif; //그냥 Base(A, la-fi)하면안됨?
			try
			{
				for (; dif > 0; --dif, Base::imp.M_finish++)
					Ra.construct(this->Base::imp.M_finish, *first++);
			}
			catch (...)
			{
				this->clear();
			}
		}

		vector(const vector& other) : Base(other.size(), (other.transform_allocator()))
		{
			Ra = this->transform_allocator();
			try
			{
				const_iterator it = other.begin();
				for (size_type other_size = other.size(); other_size > 0; other_size--)
				{
					Ra.construct(this->Base::imp.M_finish, *it);
					it++;
					this->Base::imp.M_finish++;
				}
			}
			catch (...)
			{
				this->clear();
			}
		}
		
		~vector()
		{
			this->clear();
			std::cout << "bye" << std::endl;
		}

//---------------------------------------생성자 소멸자 끝-----------------------------

		vector& operator=(const vector &other)
		{
			if (*this != other)
			{
				this->clear();
				Ra = other.transform_allocator();
				try
				{
					const_iterator it = other.begin();
					for (size_type other_size = other.size(); other_size > 0; other_size--)
					{
						Ra.construct(this->Base::imp.M_finish, *it);
						it++;
						this->Base::imp.M_finish++;
					}
				}
				catch (...)
				{
					this->clear();
				}
			}
			return (*this);
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::result, InputIterator>::type* = nullptr,
				typename ft::enable_if<ft::is_iterator<InputIterator>::result, InputIterator>::type* = nullptr)
		{
			this->clear();
			this->insert(this->begin(), first, last);
		}

		void assign(size_type n, const value_type& u)
		{
			this->clear();
			if (n == 0)
				return ;
			if (size_type(this->Base::M_eos - this->Base::M_start) >= n)
			{
				for (; n > 0; n--)
					Ra.construct(this->Base::end++, u);
			}
			else
			{
				M_deallocate(this->Base::imp.M_start, this->Base::imp.M_eos - this->Base::imp.M_start);
				this->Base(n, Ra);
				for (; n > 0; n--)
					Ra.construct(this->Base::end++, u);
			}
		}

		reference operator [](size_type pos)
		{
			return(*(this->Base::imp.M_start + pos));
		}
		
		const_reference operator [](size_type pos) const
		{
			return(*(this->Base::imp.M_start + pos));
		}

		reference at(size_type pos)
		{
			if (pos >= this->size())
				throw (std::out_of_range("vector"));
			return ((*this)[pos]);
		}
		
		const_reference at(size_type pos) const
		{
			if (pos >= this->size())
				throw (std::out_of_range("vector bomwi bursurnam"));
			return ((*this)[pos]);
		}

		reference front()
		{
			return (*(this->Base::imp.M_start));
		}
		const_reference front() const
		{
			return (*(this->Base::imp.M_start));
		}

		reference back()
		{
			return (*(this->Base::imp.M_finish - 1));
		}
		const_reference back() const
		{
			return (*(this->Base::imp.M_finish - 1));
		}

		value_type* data()
		{
			return (this->Base::imp.M_start);
		}
		value_type* data() const
		{
			return (this->Base::imp.M_start);
		}

		//--------- 그담은 ----------- 반복자관련 맴버 ----

		iterator begin() { return (iterator(this->Base::imp.M_start)); }
		const_iterator begin() const { return const_iterator(this->Base::imp.M_start); }

		iterator end() { return (iterator(this->Base::imp.M_finish)); }
		const_iterator end() const { return (const_iterator(this->Base::imp.M_finish)); }

		reverse_iterator rbegin() { return (reverse_iterator(this->Base::imp.M_start)); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->Base::imp.M_start); }

		reverse_iterator rend() { return (reverse_iterator(this->Base::imp.M_finish)); }
		const_reverse_iterator rend() const { return (const_reverse_iterator(this->Base::imp.M_finish)); }

		// ----------------그담은  ---------- 용량관련 --------------------
		bool empty() const
		{
			if (this->begin() == this->end())
				return (true);
			else
				return (false);
		}


		size_type size() const { return (size_type(this->Base::imp.M_finish - this->Base::imp.M_start)); }

		size_type max_size() const
		{
			return (Ra.max_size());
		}
		
		size_type capacity() const { return (this->Base::imp.M_eos - this->Base::imp.M_start); }

		void reserve(size_type new_cap)
		{
			if (new_cap > max_size())
				throw (std::length_error("vector length mani giller"));
			else
			{
				pointer past_start = this->Base::imp.M_start;
				size_type past_size = size();
				size_type past_capacity = capacity();//이전껄 지정하지 않으면 누수가 나겠지
				try
				{
					Base::imp.M_start = Ra.allocate(new_cap);
				}
				catch (const std::bad_alloc& e)
				{
					std::cout << "allocation failed " << e.what() << std::endl;
				}
				Base::imp.M_finish = Base::imp.M_start;
				Base::imp.M_eos = Base::imp.M_start + new_cap;
				for (size_type i = 0; i < past_size; i++)
				{
					Ra.construct(Base::imp.M_finish, *past_start);
					Base::imp.M_finish++;
					past_start++;
				}
				Ra.deallocate(past_start - past_size, past_capacity);
			}
		}
		
		void clear()
		{
			size_type tmp = this->size();
			for (size_type i = 0; i < tmp; i++, Base::imp.M_finish--)
				Ra.destroy(this->Base::imp.M_finish);
		}

		iterator insert(iterator pos, const value_type& value)
		{
			size_type res = (size_type)(pos - begin());
			if ((size() + 1)  > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			for (pointer out = Base::imp.M_finish; out != Base::imp.M_start + res; out--)
				*out = *(out - 1);
			*(Base::imp.M_start + res) = value;
			Base::imp.M_finish++;
			return iterator(Base::imp.M_start + res);
		}

		void insert(iterator pos, size_type count, const value_type& value)
		{
			size_type where = (size_type)(pos - begin());
			
			if ((size() + count) > capacity() * 2)
				reserve(size() + count);
			else if ((size() + count) > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			for (pointer out = Base::imp.M_finish; out != Base::imp.M_start + where; out--)
				*(out + count - 1) = *(out - 1);
			for (size_type i = 0; i < count; i++)
				*(Base::imp.M_start + where + i) = value;
			Base::imp.M_finish += count;
		}
		
		template<class InputIt>
		void insert(iterator pos, InputIt first, InputIt last,
				typename ft::enable_if<!ft::is_integral<InputIt>::result, InputIt>::type* = nullptr,
				typename ft::enable_if<ft::is_iterator<InputIt>::result, InputIt>::type* = nullptr)
		{
			size_type where = (size_type)(pos - begin());
			size_type many = (size_type)(last - first);
			if ((size() + many) > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			for (pointer out = Base::imp.M_finish; out != Base::imp._start + where; out--)
				*(out + many - 1) = *(out - 1);
			for (int i = 0; first != last; first++, i++)
				*(Base::imp.M_start + where + i) = *first;
			Base::imp.M_finish += many;
		}
		
		iterator erase(iterator pos)
		{
			Ra.destroy(&(*pos));
			if (pos + 1 != end())
			{
				for (int i = 0; i < Base::imp.M_finish - &(*pos) - 1; i++)
				{
					Ra.construct(&(*pos) + i, *(&(*pos) + i + 1));
					Ra.destroy(&(*pos) + i + 1);
				}
			}
			Base::imp.M_finish--;
			return (pos);
		}

		iterator erase(iterator first, iterator last)
		{
			int many = (int)(last - first) + 1;
			for (int i = 0; i < many; i++)
				Ra.destroy(&(*(first + i)));
			if (last != end())
			{
				for (int i = 0; i < Base::imp.M_finish - &(*last); i++)
				{
					Ra.construct(&(*first) + i, *(&(*last) + i + 1));
					Ra.destroy(&(*last) + i + 1);
				}
			}
			Base::imp.M_finish -= many;
			return (first);
		}

		void push_back(const value_type& value)
		{
			if (size() + 1 > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			Ra.construct(Base::imp.M_finish, value);
			Base::imp.M_finish++;
		}

		void pop_back()
		{
			Ra.destroy(Base::imp.M_finish - 1);
			Base::imp.M_finish--;
		}

		void resize(size_type new_size)
		{
			if (new_size > max_size())
				throw (std::length_error("vector length mani giller"));
			if (new_size < size())
			{
				while (size() > new_size)
				{
					Ra.destroy(&back());
					Base::imp.M_finish--;
				}
			}
			else
				reserve(new_size);
		}
		void resize(size_type new_size, const value_type &value)
		{
			if (new_size > max_size())
				throw (std::length_error("vector length mani giller"));
			if (new_size < size())
			{
				while (size() > new_size)
				{
					Ra.destroy(&back());
					Base::imp.M_finish--;
				}
			}
			else
				insert(end(), new_size - size(), value);
		}
		
		void swap(vector &other)
		{
			if (*this == other)
				return ;
			typename R_allocator_type::pointer tmp_start = other.imp.M_start;
			typename R_allocator_type::pointer tmp_finish = other.imp.M_finish;
			typename R_allocator_type::pointer tmp_eos = other.imp.M_eos;
			R_allocator_type tmp_Ra = other.Ra;

			other.imp.M_start = this->Base::imp.M_start;
			other.imp.M_finish = this->Base::imp.M_finish;
			other.imp.M_eos = this->Base::imp.M_eos;
			other.Ra = this->Ra;

			this->Base::imp.M_start = tmp_start;
			this->Base::imp.M_finish = tmp_finish;
			this->Base::imp.M_eos = tmp_eos;
			this->Ra = tmp_Ra;
		}

		//bool original
		void flip()
		{
			for (size_type i = 0; i < size(); i++)
			{
				if (*(Base::imp.M_start + i) == true)
					*(Base::imp.M_start + i) = false;
				else
					*(Base::imp.M_start + i) = true;
			}
		}

		void swap(reference x, reference y)
		{
			reference tmp;
			tmp = x;
			x = y;
			y = tmp;
		}
	};//vector

	class vector<bool>::reference
	{
	private
		friend class vector;
		reference();
	public:
		~reference();
		opreator bool () const;
		reference& operator= (const bool x);
		reference& operator= (const reference& x);
		void flip();
	};//bool
	
	}//ft

#endif
