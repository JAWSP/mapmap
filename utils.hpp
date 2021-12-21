/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 10:13:14 by juhpark           #+#    #+#             */
/*   Updated: 2021/12/20 12:38:30 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <stdexcept>
# include <memory>

//• iterators_traits, reverse_iterator, enable_if, is_integral, equal/lexicographical compare, std::pair, std::make_pair, must be reimplemented.
//그러니깐
//iterators_traits
//reverse_iterator
//enable_if
//is_integral
//equal/lexicographical comapre
//std::pair
//std::make_pair
//를 재구현
//순서도 얼추 저걸로 맞춤
namespace ft
{
/*
 * * 반복자
 * 요놈은 종류가 5가지
 * 1. 입력
 * 2. 출력
 * 3. 순뱡향
 * 4. 양뱡향
 * 5. 임의접근
 * 요놈들은 아래로 갈수록 할 수 있는게 많아짐
 * 그래서 이놈들에 대해 구별할려고 태그로 비어있는 클래스를 정의함
 * 각각 종류에 대하여 특수화를 이용하여 구별을 한다 이말이야 
 *
 */
//요놈은 반복자들에 대한 기본이 되는 base iterator class
//근데 막상 std::iterator에 대한 소스코드를 보면 그냥 인클루드만 정의되어있음
//https://code.woboq.org/gcc/libstdc++-v3/include/std/iterator.html ㅇㄱㄹㅇ
//그래서 그냥 iterator_traits만 정의할까
//암튼 거기선 tag정의/base정의/trait정의가 된듯하다
//암튼 저걸 가지고 쉐킷 쉐킷한다던데

//구별하기 위하여 미리 선언해둠
//상속은 레퍼런스에서 저리 쓰라고 했고 실제로도 저런 관계더만 
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

//이건 반복자들의 베이스가 되는 공통형 반복자 템플릿 클래스
//중첩된 형식만 정의함
//하는 일이 아무것도 없지만 요놈을 상속하여 일부 작업을 저장하는데 쓰인다고
//typdef들은 특수화 및 오버로딩에 사용됨
	template <class Category, class T, class Distance = ptrdiff_t,
			 class Pointer = T*, class Reference = T&>
	class iterator
	{
	public:
		typedef Category iterator_category; //5가지 타입 중 하나
		typedef T value_type; //자료형이 뭐냐
		typedef Distance difference_type; //2가지 반복자 사이의 차이라는데..
		typedef Pointer pointer;
		typedef Reference reference;
	};


	template <class Iterator> 
	//iterator에 대한 정의가 있으므로 이에 대한 특수화가 아니네
	//암튼 저 템플릿 인자는 속섬을검색할 반복자의 유형이라고 한다
	//근데 저것만 가지고 어떻게 점마가 Iterator란걸 인식을 하는거지?
	//그냥 저것이 되는놈만 거르는건가
	class iterator_traits
	{
	public:
		typedef typename Iterator::iterator_category iterator_category; //해당반복자가 뭐냐
		typedef typename Iterator::value_type value_type; //그 값의 타입이 뭐냐
		typedef typename Iterator::difference_type difference_type; //두개의 차이가 뭐냐
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};

	template <class T>
	class iterator_traits<T*> //자료형 T의 포인터에 대한 특수화
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template <class T>
	class iterator_traits<const T*>//얜 상수 포인터 T
	{
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

//그담은 역방향 반복자
//소스코드를 뒤져보니 임의지정 접근자에서 약간 수정한 버전인듯함
//근데 역방향의 타입은 이도저도 아닌데 타입을 뭘로 정해야 하는걸까

	template<class Iterator>
	class reverse_iterator
	{
	private:
		Iterator value;
		typedef ft::iterator_traits<Iterator> traits;
	public:
		typedef Iterator							iterator_type; //원작재현
		typedef typename traits::iterator_category	iterator_category;// =임의지정
		typedef typename traits::value_type			value_type; //=T
		typedef typename traits::difference_type	difference_type; //diffptr_t
		typedef typename traits::pointer			pointer; //=T*
		typedef typename traits::reference			reference;//=T&
		//원래 기존꺼는 카테고리랑 value타입이 없긴한데 나름 양식 맞추어야 할 것 같아서 넣음
		reverse_iterator() : value(0) { }
		virtual ~reverse_iterator() { }
		explicit reverse_iterator(Iterator v) : value(v) { }
		
		template<class It>
		reverse_iterator(const reverse_iterator<It>& R) : value(R.base()) { }
//근데 얜 먹힘
//심지어 이걸 다른 반복자에 넣어도 됨
//이야
		reverse_iterator &operator =(const reverse_iterator& R)
		{
			if (*this != R)
				this->value = R.value;
			return (*this);
		}
		//늘 그랬듯이 캐노니컬 4대장
		//과 추가
		//임의는 T*인데 얜 그냥 쓰이는 이유는 요놈의 인자가 T*형으로 받아왔기때문
		/*
		operator reverse_iterator<const Iterator> () const
		{
			return (reverse_iterator<const Iterator>(this->value));
		}
		*/
		//저 위엣놈은 그냥 반복자 -> const 반복자로 바꾸는걸 해주는 친구인데
		//저놈에게 인식이 안됨
		//근데 얜 안먹힘
		//그런데 기본으로 정의되어있는 템플릿 포함한 복사생성자가 그 역활을 대신함
		//왜 이건 안되고 저건 될까

		//기존 base(random access iterator)는 pointer지만 위와 같이 T*형으로 들어옴
		//그리고 base는 그대로 저리하고 실제위치는 저거보다 왼쪽에 위치함
		iterator_type base() const
		{
			return (value);
		}

		//실제 base를 유지해야 하기 떄문에 저리쓰는거
		//암튼 base보다 왼쪽으로 한칸 더 위치에 있기 떄문에 저리써야함
		reference operator *() const
		{
			iterator_type tmp = value;
			return (*--tmp);
		}
		//기존건 그대로 리턴하는데
		//얜 좀 신기하게 리턴하네
		//왜냐면 참조값의 주소값을 불러오기 위해서지
		//근데 궁금한건 주소에*붙이면 참조같은데 굳이 *&를 붙여야 되는 이유가 뭘까
		//반복자의 근본을 보면 반복자 자체는 포인터와 유사하지
		//포인터랑 같다는건 아니니깐 그런가봄
		pointer operator ->() const
		{
			return (&(operator*()));
		}
		//그담은 증감
		reverse_iterator& operator++() //전위(제갈량이 사랑한 그거 아님)->당연하지 쬬가 사랑한건데
		{
			--value;
			return (*this);
		}
		reverse_iterator operator++(int)//후위
		{
			reverse_iterator tmp = *this;
			--value;
			return (tmp);
		}
		reverse_iterator& operator--() //전위
		{
			++value;
			return (*this);
		}
		reverse_iterator operator--(int)//후위
		{
			reverse_iterator tmp = *this;
			++value;
			return (tmp);
		}
		//그담은 쁠마
		//역순이니 더하면 빼고
		//빼면 더해주고
		reverse_iterator operator +(difference_type n) const
		{
			return (reverse_iterator(value - n));
		}
		reverse_iterator operator +=(difference_type n)
		{
			value -= n;
			return (*this);
		}
		reverse_iterator operator -(difference_type n) const
		{
			return (reverse_iterator(value + n));
		}
		reverse_iterator operator -=(difference_type n)
		{
			value += n;
			return (*this);
		}
		//그담은 참조[] 
		//역방향 반복자의 []는 반대로 간다 
		//그러니깐 그냥 반복자의 []는 맨왼쪽부터 오른쪽으로 가는데
		//역방향은 맨 오른쪽부터 왼쪽으로간다
		//따라서 원본소스코드는 기존 역방향 반복자에다가 앞서 정의한
		//+연산자를 이용하여 표현한다
		//march *(riter + n)이거마냥
		//저 리턴값이 뭔의미인지 한참을 해맸네 ㅎㅎ
		reference operator[](difference_type n) const
		{
			return (*(*this + n));// 마이크로 소프트, 원본코드
			//return (this->base()[-n-1]);//cplus,refence에선
		}
	};
	//그담은 비교, 관계 추가적인 +-연산자
	//이친구들은 base를 이용하여 비교글 한다고 함
	//== !=이게 equal comapre이라고 생각
	template<typename Iterator>
	bool operator ==(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() == y.base());
	}
	template<typename Iterator>
	bool operator !=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() != y.base());
	}
	template<typename Iterator>
	bool operator <(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() > y.base());
	}
	template<typename Iterator>
	bool operator >(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() < y.base());
	}
	template<typename Iterator>
	bool operator <=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() >= y.base());
	}
	template<typename Iterator>
	bool operator >=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (x.base() <= y.base());
	}
//다른 유형
	template<typename IteratorL, typename IteratorR>
	bool operator ==(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() == y.base());
	}
	template<typename IteratorL, typename IteratorR>
	bool operator !=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() != y.base());
	}
	template<typename IteratorL, typename IteratorR>
	bool operator <(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() > y.base());
	}
	template<typename IteratorL, typename IteratorR>
	bool operator >(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() < y.base());
	}
	template<typename IteratorL, typename IteratorR>
	bool operator <=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() >= y.base());
	}
	template<typename IteratorL, typename IteratorR>
	bool operator >=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (x.base() <= y.base());
	}
	//뺄
	//아무리 역순이라쳐도 빼는 순서가 반대로 되어있네

	template<typename IteratorL, typename IteratorR>
	typename reverse_iterator<IteratorL>::difference_type
	operator -(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{
		return (y.base() - x.base());
	}
	template<typename Iterator>
	typename reverse_iterator<Iterator>::difference_type
	operator -(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{
		return (y.base() - x.base());
	}
	template<typename Iterator>
	reverse_iterator<Iterator> operator +(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x)
	{
		return (reverse_iterator<Iterator>(x.base() - n));
	}
	//그담은 allocator...?
	//응 아냐 enable_if야
	
	/*
	 * 함수 템플릿으로 오버로딩을 만들다보면
	 * 우리의 템플릿은 겁나게 스마트해서 의도치 않은 함수 템플릿으로 인자가 인식을 하게 됨
	 * typename T::value_type negate(const T& t) 요런놈(T는 템플릿 인자)가 있는데
	 * 만약 int형을 인자로 받았다고 하면 int::value_type이런건 없자나
	 * 그래서 의도치 않게 인자가 들어가서 에러를 뱉는걸 방지하기 위한 무시처리인 SFINAE가 있는데
	 * enable_if는 그걸 이용하여 우리가 의도한 인자만 받게 걸러주는 역할을 한다고 함
	 * 작동방식은 bool형의 인자 B가 true가 되면 아래 enable_if가 작동되어 type가 정의되고
	 * 만약 아니다 그럼 첫번째 enable_if로 가서 아무고토 없는게 나오게됨
	 */
	template<bool B, typename T = void>
	struct enable_if { };

	template<class T>
	struct enable_if<true, T> {typedef T type;};

	//그리고 그걸 써먹을 방법 중 하나인 is_integral
	/*
	 * 요놈은 말 그대로 정수이냐 따지는거
	 * 요놈도 enable_if랑 비스므리하게 돌아가는데
	 * 만약 특정 특수화랑 부합되는게 있음 true가 되고 type가 정의
	 * 아님 false되고 나가리
	 * 근데 아닌걸 거르는게 더 힘드니 아닌게 기본형
	 * 맞는게 특수화되어서 등장
	 */
	//일단 이놈은 is_integral를 상속해준 모체다
	//마이크로소프트에서 말하길 형식 및 값에서 정수 계열 상수를 만든다던데...
	//그외 여러가지 추가요소에 맨붕해서 물어보니
	//그냥 정수맞냐 틀리냐 그것만 쓰면 된다고함
	//그럼 enable_if를 밴치마킹해야지
	//라고 무심코 들이댔다가 얻어맞고 얌전히 rc_remove랑 const_머시기에서 필요한것만 넣기로 함
	//그리고 맨처음 helper원본은 나머지이니 false라 치고
	//나머지는 정수형이니 true인 res에게 상속받도록 함
	template <bool B, typename T>
	struct is_integral_res
	{
		static const bool result = B; //저렇게 안하면 저리 안했다고 에러를 밷음
		typedef T type;
	};

	typedef is_integral_res<true, bool>		true_type;
	typedef is_integral_res<false, bool>	false_type;

	template<typename>
    struct is_integral_helper : public false_type { };
	template<>
    struct is_integral_helper<bool> : public true_type { };
	template<>
    struct is_integral_helper<char> : public true_type { };
	template<>
    struct is_integral_helper<signed char> : public true_type { };
  	template<>
    struct is_integral_helper<unsigned char> : public true_type { };
  	template<>
    struct is_integral_helper<char16_t> : public true_type { };
  	template<>
    struct is_integral_helper<char32_t> : public true_type { };
  	template<>
    struct is_integral_helper<short> : public true_type { };
  	template<>
    struct is_integral_helper<unsigned short> : public true_type { };
  	template<>
    struct is_integral_helper<int> : public true_type { };
  	template<>
    struct is_integral_helper<unsigned int> : public true_type { };
  	template<>
    struct is_integral_helper<long> : public true_type { };
  	template<>
    struct is_integral_helper<unsigned long> : public true_type { };
  	template<>
    struct is_integral_helper<long long> : public true_type { };
  	template<>
    struct is_integral_helper<unsigned long long> : public true_type { };
	//그러니깐 integral의 작동방식은 젤 앞에 있는 
	template<typename T>
	struct is_integral : public is_integral_helper<T> { };

	//그럼 그담은 저게 이터레이터가 맞는지
	//요런 방법도 있다고
	//helper를 없애고 is-res로 이어지게 만들었다캄

	template<bool B, typename T>
	struct is_iterator_res
	{
		typedef T type;
		const static bool result = B;
	};

	template<typename T>
	struct is_iterator : public is_iterator_res<false, T> { };
	template<>
	struct is_iterator<random_access_iterator_tag>
	: public is_iterator_res<true, random_access_iterator_tag> { };
	template<>
	struct is_iterator<bidirectional_iterator_tag>
	: public is_iterator_res<true, bidirectional_iterator_tag> { };
	template<>
	struct is_iterator<forward_iterator_tag>
	: public is_iterator_res<true, forward_iterator_tag> { };
	template<>
	struct is_iterator<input_iterator_tag>
	: public is_iterator_res<true, input_iterator_tag> { };
	template<>
	struct is_iterator<output_iterator_tag>
	: public is_iterator_res<true, output_iterator_tag> { };

//이것이 lexicographical compare
	template<class InputIterator>
	size_t its_dif(InputIterator start, InputIterator finish)
	{
		ptrdiff_t res = 0;
		for (InputIterator tmp = start; tmp != finish; tmp++)
		{
			res++;
		}
		return ((size_t)res);
	}

	//그담은 pair
	//set, map과 같은 연관 컨테이너들은 둘 이상의 값을 묶어서 관리하거나 리턴할 일이 많다
	//따라서 키와 값을 표현하기 위해 pair라는 구조체를 이용한다
	template<class T1, class T2>
	struct pair
	{
	public:
		typedef T1 first_type;
		typedef T2 second_type;
		T1 first;
		T2 second;

		pair() : first(T1()), second(T2()) { }

		~pair() { }

		pair(const T1& x, const T2& y) : first(x), second(y) { }

		template<class U1, class U2>
		pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
		
		pair(const pair<T1, T2>& p) : first(p.first), second(p.second) { }

		pair& operator =(const pair& P)
		{
			if (*this != P)
			{
				this->first = P.first;
				this->second = P.second;
			}
			return (*this);
		}

		operator const pair<T1, T2> () const
		{
			return (pair<T1, T2>(this->first, this->second));
		}
	};//pair
	
	//이후는 pair의 맴버아닌 함수들
	//요놈은 pair의 객체를 생성
	template<class T1, class T2>
	ft::pair<T1, T2> make_pair(T1 t, T2 u)
	{
		ft::pair<T1, T2> res(t, u);
		return (res);
	}

	//그담은 pair의 비교연산자들
	template< class T1, class T2 >
	bool operator==(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		if ((left.first == right.first) && (left.second == right.second))
			return (true);
		else
			return (false);
	}

	template< class T1, class T2 >
	bool operator!=(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		return (!(left == right));
	}

	template< class T1, class T2 >
	bool operator <(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		if (left.first < right.first)
			return (true);
		else if (left.first == right.first && left.second < right.second)
			return (true);
		else
			return (false);
	}

	template< class T1, class T2 >
	bool operator <=(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		return (!(right < left));
	}

	template< class T1, class T2 >
	bool operator >(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		return (right < left);
	}

	template< class T1, class T2 >
	bool operator >=(const ft::pair<T1,T2>& left, const ft::pair<T1,T2>& right)
	{
		return (!(left < right));
	}
}//ft


#endif
