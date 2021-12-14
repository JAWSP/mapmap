/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:22:30 by juhpark           #+#    #+#             */
/*   Updated: 2021/11/01 16:54:57 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STACK_HPP
# define FT_STACK_HPP

# include "ft_vector.hpp"

/*
 * 이번엔 스택이다
 * 원본과 큰 차이점이라면 deque대신에 vector를 인자로 받는거
 * 얼추 벡터 상속이라고 해도 되겠네 ㅎㅎㅎㅎ
 */

namespace ft
{
	template<class T, class Container = ft::vector<T> >
	class stack
	{
		template<typename T1, typename T2>
		friend bool operator ==(const stack<T1, T2>&, const stack<T1, T2>&);
		template<typename T1, typename T2>
		friend bool operator >(const stack<T1, T2>&, const stack<T1, T2>&);
	public:
		//스택엔 반복자가 없어요
		//데이터에 접근하는 순서가 있기 때문
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef	Container container_type;
	protected:
		Container c; //->일종의 객체
	public:
		//디폴트 생성자는 C++11부터 있다고
		explicit stack(const container_type& cont = container_type()) : c(cont) {}
		stack(const stack& S)
		{
			c = S.c;
		}
		//놀랍게도 이게다
		//그담은 소멸자랑 연산자 오버로딩
		//근데 원본코드엔 소멸자가 정의되어 있지않음
		~stack()
		{
		//	~c();
		}
		stack& operator =(const stack &S)
		{
			if (*this != S)
				this->c = S.c;
			return (*this);
		}

		//스택에 맨위에 있는 놈을 리턴
		//그러니까 맨 마지막에 추가한 놈을 리턴해주는 함수
		//맨마지막 = 벡터의 back()
		const_reference top() const
		{
			return (c.back());
		}

		bool empty() const
		{
			return (c.empty());
		}

		size_type size() const
		{
			return(c.size());
		}

		//값을 넣는게 push
		//빼는게 pop
		void push(const value_type& value)
		{
			return (c.push_back(value));
		}

		void pop()
		{
			return (c.pop_back());
		}
	};//stack

	///그담은 비교연산자
	template<class T, class Container>
	bool operator ==(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (left.c == right.c);
	}

	template<class T, class Container>
	bool operator !=(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (!(left == right));
	}
	template<class T, class Container>
	bool operator >(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (left.c > right.c);
	}
	template<class T, class Container>
	bool operator >=(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (!(left < right));
	}
	template<class T, class Container>
	bool operator <(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (right > left);
	}
	template<class T, class Container>
	bool operator <=(const ft::stack<T, Container>& left, const ft::stack<T, Container>& right)
	{
		return (!(left > right));
	}
}//ft


#endif
