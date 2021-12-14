/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_access_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 14:31:56 by juhpark           #+#    #+#             */
/*   Updated: 2021/11/24 10:21:16 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

#include <iostream>
#include "utils.hpp"

//유틸리티에서 정의된 반복자의 양식에 따라 요놈을 만든다
//나름 내린 결론은 std::iterator로 정의를 하고 std::iterator_traits로 특성을 본다
namespace ft
{
	template<typename T>
	class random_access_iterator : public ft::iterator<ft::random_access_iterator_tag, T>
	//나 임의 접근 반복자란걸 저기서 정의를 한다
	//만약에 traits라고 할땐 점마 속성이 뭔지 모른채로 쓰자너
	{
	private:
	//원작고증?
		T* value;
		typedef ft::iterator<ft::random_access_iterator_tag, T> iterator_type;
	public:
		typedef typename iterator_type::iterator_category	iterator_category;
		typedef typename iterator_type::value_type			value_type;
		typedef typename iterator_type::difference_type		difference_type;
		typedef typename iterator_type::reference			reference;
		typedef typename iterator_type::pointer				pointer;

		random_access_iterator() : value(0) {}
		virtual ~random_access_iterator() {} ;
		explicit random_access_iterator(T* v) : value(v) {}
//		template<typename V>
//		random_access_iterator(const random_access_iterator<V> &R) : value(R.base()) { }
		random_access_iterator(const random_access_iterator &R) : value(R.value) {}
		random_access_iterator &operator =(const random_access_iterator &R)
		{
			if (*this != R)
				this->value = R.value;
			return (*this);
		}

		//기존 반복자를 const반복자로 변환을 가능하게 해줌
		//원리는 enable_if랑 복사생성자를 섞은거
		//라고 원본코드에선 저렇게 써져있었으나 
		//뭔가 안됨 이전에도 저런 방식으로 잘 되었던거 같았는데...
		/*
		template<typename Val_type>
		random_access_iterator(const random_access_iterator<Val_type, typename ft::enable_if<ft::are_same<Val_type, value_type>::result>::type* = 0>& I) : value(I.base())
		__normal_iterator(const __normal_iterator<_Iter, typename __enable_if<(std::__are_same<_Iter, typename _Container::pointer>::__value), _Container>::__type>& __i)
		{
		}
		*/
		operator random_access_iterator<const T> () const
		{
			return (random_access_iterator<const T>(this->value));
		}
		

		//일단 캐노니컬 4대장
		//그담은 각종연산자들
		//https://www.cplusplus.com/reference/iterator/ 여기 나온 순서대로 만드러야지
		//모든 반복자(위에선 할당/복사) 그담은 전위/후위 증가
		random_access_iterator &operator ++() //전위(삼국지 장수아님)
		{
			++value;
			return (*this);
		}
		const random_access_iterator operator ++(int)//후위
		{
			random_access_iterator tmp(*this);
			this->value++;
			return (tmp);
		}

		//input에 있는 같거나다르거나/참조
		//T &operator해도 되는데 그걸 줄여서 reference로 불러도 되니
		//이건 포인터도 마찬가지
		reference operator *() const
		{
			return (*value);
		}
		pointer operator ->() const //T* operator->
		{
			return (value);
		}
		random_access_iterator &operator --()
		{
			--value;
			return (*this);
		}
		const random_access_iterator operator --(int)
		{
			random_access_iterator tmp(*this);
			--this->value;
			return (tmp);
		}
		//그담은 증감연산 나머지 비교 []
		//사칙연산은 해당 인덱스가 가리키는 값끼리 연산이 아니라
		//연산한 인덱스를 나타냄
		random_access_iterator operator +(difference_type n) const
		{
			return (random_access_iterator(value + n));
		}
		random_access_iterator operator -(difference_type n) const
		{
			return (random_access_iterator(value - n));
		}
		//근데 원본이 이리나옴
		/*
		 * _normal_iterator
		 * operator+(difference_type __n) const _GLIBCXX_NOEXCEPT
		 * { return __normal_iterator(_M_current + __n); }
		 * 근데 굳이 새로 선언할 필요가 있나?
		 * 근데 이미 저거에 대한 오버로딩만들땐 객체를 새로만들어서 했네
		 * 근데 만든곳에 저리해도 그대로 되던데
		 * 아 맴버 변수가 여러개면 힘드니까 저리 쓰는거네
		 * 하지만 지금은 변수가 하나니깐 걍 저리써도 될듯
		 * 근데 리턴값이 클래스인데 다르게 나오네
		 * 그럼 저리쓰자
		 */
		random_access_iterator& operator +=(difference_type n)
		{
			value += n;
			return (*this);
		}
		random_access_iterator& operator -=(difference_type n)
		{
			value -= n;
			return (*this);
		}
		//얜 기존꺼에다가 더해서 쓰는거라서 실제 인덱스에다가 저리 더함
		reference operator [](difference_type n) const
		{
			return (value[n]);
		}
		//이놈은 요 클래스의 getter와 같은 역할
		pointer base() const
		{
			return (this->value);
		}
	};

		//그리고 크기비교는 죄다 밖에다 빼놓음, == != ><같은건 왜 밖에다 쓰는거지
		//일단 나와있는 바로는 너님이 임의지정 반복자 뿐만 아니라
		//다른 반복자끼리 비교를 할 수 있다고 한다
		//다른 유형의 반복자끼리 비교를 할 경우가 있나?
		//그뿐만 아니라 비교하는 유형도 여러개로 나누었다
		//한 반복자 안에서 비교할때
		//서로 다른 반복자(심지어 같은 임의비교인데!) 두놈 사이서 비교
		//도당채 왜....
		//대충 정수랑 부동소수점 비교할려고 그런가
	//이건 서로 다른 반복자사이일떄
	//원본판은
	/*
	template<typename _IteratorL, typename _IteratorR, typename _Container>
		inline bool
		operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
				   const __normal_iterator<_IteratorR, _Container>& __rhs)
		_GLIBCXX_NOEXCEPT
		{ return __lhs.base() == __rhs.base(); }
		이렇게 2개의 이터레이터 혹은
	template<typename _Iterator, typename _Container>
		inline bool
		operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
				   const __normal_iterator<_Iterator, _Container>& __rhs)
		_GLIBCXX_NOEXCEPT
		{ return __lhs.base() == __rhs.base(); }
		가 있게 된다
		하지만 우리가 만든 반복자는 컨테이너의 형태가 분명(벡터)이므로, 굳이 저걸 템플릿 인자로 안받아도 된다.
		근데 인라인 왜쓰지?
		모르면 가만히 있어야지
		없어도 잘돌아가는듯
	*/
	//같냐
	template<typename T_L, typename T_R>
	bool operator ==(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() == conB.base());
	}
	template<typename T>
	bool operator ==(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() == conB.base());
	}
	//다르냐
	template<typename T_L, typename T_R>
	bool operator !=(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() != conB.base());
	}
	template<typename T>
	bool operator !=(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() != conB.base());
	}

	//오른쪽이크냐
	template<typename T_L, typename T_R>
	bool operator <(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() < conB.base());
	}
	template<typename T>
	bool operator <(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() < conB.base());
	}

	template<typename T_L, typename T_R>
	bool operator <=(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() <= conB.base());
	}
	template<typename T>
	bool operator <=(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() <= conB.base());
	}

	//왼쪽이 같냐
	template<typename T_L, typename T_R>
	bool operator >(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() > conB.base());
	}
	template<typename T>
	bool operator >(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() > conB.base());
	}

	template<typename T_L, typename T_R>
	bool operator >=(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() >= conB.base());
	}
	template<typename T>
	bool operator >=(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() >= conB.base());
	}

	//그담은 반복자끼리 연산
	//빼기는 포인터끼리의 차니깐 ptrdiff_t으로 리턴
	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type operator -(const ft::random_access_iterator<T_L> conA, const ft::random_access_iterator<T_R> conB)
	{
		return (conA.base() - conB.base());
	}
	template<typename T>
	typename ft::random_access_iterator<T>::difference_type operator -(const ft::random_access_iterator<T> conA, const ft::random_access_iterator<T> conB)
	{
		return (conA.base() - conB.base());
	}

	// 그담은 더하기
	// 이놈은 겁나 신기하게 기존것과는 달리 반복자끼리 연산이 아닌 위에서 했던것과 같이
	// difference_type형의 변수만 가지고 연산을 한다
	// 또한 conA.base() + difference_type형 변수가 아닌 무건가로 리턴이 된다
	// 근데 저걸 저리 정의한 이유가 뭐지?
	// num + 반복자랑
	// 반복자 + num이랑 따로인가보다
	// 또한 반복자 + num 은 위에 미리 정의되어 있는데
	// num+ 반복자는 정의되어 있지 않았기 떄문
	template<typename T>
	ft::random_access_iterator<T> operator +(typename ft::random_access_iterator<T>::difference_type n, random_access_iterator<T> con)
	{
		return (ft::random_access_iterator<T>(con.base() + n));
	}

}

#endif
