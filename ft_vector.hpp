/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 12:30:03 by juhpark           #+#    #+#             */
/*   Updated: 2021/12/20 14:58:30 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <iostream>
# include "random_access_iterator.hpp"
# include "utils.hpp" 

//정신나갈것같아 정신나갈것같아 정신나갈것같아 정신나갈것같아 정신나갈것같아
//전능하신아카라트시여 이하생략

namespace ft
{
	//일단 저 베이스 구조체는  보류 하려고 했다
	//뭔가 rebind를 쓸 것 같이 생겼는데
	//실제 통과한 사람들 보면 rebind를 안썼다
	//저거 관련해서 물어봐도 한사람 안한사람 있고...
	//라기보단 저 rebind를 따로 private변수로 따로 만들어서 관리했다
	//심지어 저게 스노우볼이 되어서 
	//서브젝트에서 제시한 cppreference나 cpluscplus에선 그냥 allocator쓰는데
	//실제 코드에선 rebind된 allocator를 쓰거나 
	//그냥 내가 본양반들은 다 저리 만들었더라
	//그리고 base클라스에 있던 친구들도 다 vector클라스 안에서 다 만들었더라
	//확실히 가독성은 저 base클래스를 안쓰는게 더 좋아보였다
	//왜냐고? 여기저기 와리가리해서 안봐도 되거든
	//그런데 그래도 rebind배웠는데 함 써봐야지 ㅎㅎ
	//암튼 그리해서 그냥 쓸란다
	
	
	//레퍼런스를 보니 벡터 클래스 템플릿을 만들기 전에
	//저걸 보좌하는 구조체 템플릿을 따로 만드는 듯 하다
	template<typename T, typename Alloc> 
		//T는 그거맞고 디폴트는 아마 std::allocator인듯
		//2번째 인자로 갖고노는 영상(https://www.youtube.com/watch?v=v-qePUHf8iU&ab_channel=CodeSports)
		//거기서 보니깐 거기도 자기만의 할당자 만들때 std::allocator만들더라
		//애초에 그래야지 rebind쓰지 ㅎㅎㅎ
	struct vector_base
	{
		//rebind쓰는 이유
		//너님이 int형 백터를 선언한다고 해서 int형 인자만 할당하는게 아니야
		//Int뿐만 아니라 요  타입에 대한 할당할게 추가로 존재하는데
		//그것에 대한 메모리를 맞추기위해 
		//할당자 클래스 안의 rebind중첩 구조체 템플릿을 가져와야 한다
		typedef typename Alloc::template rebind<T>::other rebind_alloc;
		//other는 rebind안에 정의되어있는 별칭
		struct B_imp : public rebind_alloc
		{
			typename rebind_alloc::pointer M_start;
			typename rebind_alloc::pointer M_finish;
			typename rebind_alloc::pointer M_eos;
			//나 구조체에 생성자 만들 수 있는거 처음봄
			B_imp() : rebind_alloc(), M_start(0), M_finish(0), M_eos(0)
			{
//				std::cout << "예압 렛츠기릿 베베" << std::endl;
			}
			//할당자의 생성자를 보면 복사생성자도 있다
			//그러니깐 std 할당자의 복사생성자를 이용한 rebind의 복사생성자
			B_imp(rebind_alloc const& another)
				: rebind_alloc(another), M_start(0), M_finish(0), M_eos(0) { }
		};

		//rebind로 만든 중첩 구조체를 객체화
		B_imp imp;

		//템플릿 에 있는놈을 쓰니깐 반환형이라도 typename를 쓴다
		//암튼 이바닥 버전 할당함수
		typename rebind_alloc::pointer M_allocate(size_t n)
		{
//			std::cout << " 얼록얼록 " <<std::endl;
			if (n != 0)
				return (imp.allocate(n));
			//구조체에 왜 갑자기 allocater냐고? 왜긴 저걸 상속받았으니 쓰는거지
			else
				return (0);
			//없으면 0이지 저 0은 nullptr인가
		}
		//이건 해제
		void M_deallocate(typename rebind_alloc::pointer p, size_t n)
		{
//			std::cout << " 얼록해제 " <<std::endl;
			if (p)
				imp.deallocate(p, n);
		}

		//추가로 할당할 친구들이 있는 구조체들을 rebind형 할당자로 변환
		//+ rebind로 구현된 구조체의 객체를 그것에 대한 할당자 클래스로 변환
		//근데 이리 하는 이유가 뭐지
		//일단은 imp는 rebind_alloc의 상속받은 구조체
		//기본적으론 rebind_alloc의 근본은 std::allocator이지만
		//커스텀 할당자로 할당을 했을땐 std::allocator와 완전히 똑같은게 아니기 때문에
		//해당 할당자를 이용하여 확인하기 위하여 저걸 쓰는거
		rebind_alloc& transform_allocator()
		{
			return (*static_cast<rebind_alloc*>(&this->imp));
		}
		const rebind_alloc& transform_allocator() const
		{
			return (*static_cast<const rebind_alloc*>(&this->imp));
		}
		//-><>안에 별붙이니 됨 뭐지?
		//그리고 그것에 대한 할당자를 리턴 
		//근데 요 아래는 진짜 쓰이지도않는데 왜넣은거지

		//이제 진짜 생성자
		vector_base() : imp() { }
		vector_base(const Alloc& A) : imp(A) { }
		vector_base(size_t n) : imp()
		{
			this->imp.M_start = this->M_allocate(n);//요만큼 할당,저함수는 아래에
			this->imp.M_finish = this->imp.M_start;//아무것도 없으니 처음이 곧 끝
			this->imp.M_eos = this->imp.M_start + n;//저장량은 n만큼 할당이니 처음+n
		}
		//이건 특정 할당자에 대하여 n만큼
		vector_base(size_t n, const Alloc& A) : imp(A)
		{
			this->imp.M_start = this->M_allocate(n);
			this->imp.M_finish = this->imp.M_start;
			this->imp.M_eos = this->imp.M_start + n;
		}
		//이건 파괴자
		~vector_base()
		{
			M_deallocate(this->imp.M_start, this->imp.M_eos - this->imp.M_start);
		}
	};

	//------------------------- 찐 벡터 -------------------------------------

	//이제 진짜 벡터 시작
	//인자 T는 자료형
	//Alloc는 할당자
	template<typename T, typename Alloc = std::allocator<T> >
	class vector : protected vector_base<T, Alloc>
	{
	private:
		typedef vector_base<T, Alloc> Base; //base그자체
		typedef typename Base::rebind_alloc R_allocator_type;//rebind 할당자
		R_allocator_type Ra;

	public:
		typedef T value_type;
		typedef Alloc allocator_type;
		//할당할떈 new보단 할당자가 더 어울린다고 한다
		//그럼 저걸 봐서라도 할당자 봐야할듯
		//문제는 다른건 몰라도 ref같은 놈들은 사이트에선 &value_type이리 되어있는데
		//마이크로소프트나 원본소스코드보면 allocator::reference로 정의됨
		//어느장단에 맞추라는거지?
		//걍 마소 원본 따라간다
		//그런데 여기서 또 갈리는게 마소는 그냥 allocator안의 pointer를 쓰는데
		//원본 소스코드는 rebind가 들어간 allocator의 pointer를 쓴다
		//결국은 같아보이긴한데 rebind함 써봐야지
		typedef typename R_allocator_type::pointer pointer;
		typedef typename R_allocator_type::const_pointer const_pointer;
		typedef typename R_allocator_type::reference reference;
		typedef typename R_allocator_type::const_reference const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		//반복자는 어캐 정의하지
		//레퍼런스를 보니깐 random access iterator같은걸쓴다던데
		//찾아보니 저 T를 포인터로 해서 연산을 하는 것 같아보임
		//그래도 랜덤 액세스 이터레이터도 걸리는데...

		//암튼 그담은 반복자 친구들
		//그냥 반복자는 ft::iterator를 베이스로 했기떄문에 자료형이 들어감
		//그리고 역방향은 그iteraotr를 인자로 받는놈이니 저렇게 쓴거
		typedef ft::random_access_iterator<value_type> iterator;
		typedef ft::random_access_iterator<const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

//------------------------------ 별칭화 끝남 --------------------------------

		//생성자는 의도치 않은 형변환이 일어 날 수 있으므로 explicit으로 막아둠
		explicit vector() : Base()//디폴트
		{
			Ra = Base::transform_allocator(); 
		//	std::cout << "asd\n";
		}
		explicit vector(const allocator_type& A) : Base(A)//커스텀 할당자
		{
			Ra = Base::transform_allocator(); 
		//std::cout << "custom vector spawn\n";
		}
		//count개의 요소를 가지고, value로 값을 집어넣는 생성자
		//할당자는 그냥 std그거
		//Base(count, A)는 start는 n개할당
		//finish는 start의 주소값을 받고
		//eos는 start+n과 같음
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
		
		//그담은 범위 지정 생성자
		//위의 생성자랑 (4, 8)같이 들어올 때가 있는데
		//그거랑 구별을 하기 위하여 나 숫자 아니요 란걸 넣어야함
		//반복자는 숫자가 아니자나
		//그래서 그걸 구별하기 위하여 쓴게 enable_if
		/*
		 *이전엔 연산을 썼지만 +-연산은 오직 임의접근만
		 * 양방향 반복자증감 같거나 틀리거나만 사용가능
		 */
		template<class InputIterator>
		vector(InputIterator first, InputIterator last,
				const allocator_type& A = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::result, InputIterator>::type* = 0) : Base(A)
		{
			size_type dif = ft::its_dif(first, last);
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
		//그담은 복사생성자
		//ref에선 할당자 있는경우 없는경우 나누었는데
		//원본은 합쳐놓음
		//그래 합친게 낫겠지
		//근데 복사생성자 만들때 기존에 있었을때도 만들었었나
		
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
			//보통 insert쓰던데...
		}
		
		//일단은 begin, end, size를 다 맹글어야지 될듯

		//그담은 소멸자
		//메모리 해제하고 소멸이 아니라
		//소멸하고 해제를 시키네
		//일단 해제는 vector_base에서 하기때문에 안넣었지만 저게 맞는건가
		~vector()
		{
			this->clear();
//			std::cout << "bye" << std::endl;
		}

//---------------------------------------생성자 소멸자 끝-----------------------------

		//맴버함수들은 크게 캐노니컬4머장/반복자/capacity/요소접근/추가제거/할당자

		//그담은 할당연산자, assign
		vector& operator=(const vector &other)
		{
			if (*this != other)
			{
				this->assign(other.begin(), other.end());
				/*
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
				*/
			}
			return (*this);
		}

		//그담은 assign
		/*
		 * 기존껄 지우고
		 * 인자가 반복자면
		 * 반복자 구간사이의 값을 집어넣음
		 * 만약에 다른 매개변수라면 원소 무언가를 n개나 가지는 형식으로 만든다고캄
		 */
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::result>::type* = 0)
		{
			this->clear();
			this->insert(this->begin(), first, last);
		}

		void assign(size_type n, const value_type& u)
		{
			this->clear();
			if (n == 0)
				return ;
			if (size_type(this->Base::imp.M_eos - this->Base::imp.M_start) >= n)
			{
				for (; n > 0; n--)
					Ra.construct(this->Base::imp.M_finish++, u);
			}
			else
				insert(this->begin(), n, u);//아무고토 없을떄 세그먼트 나는거 수정
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

		//요놈은 벡터에 대한 첫번째 포인터를 반환해주는 놈
		value_type* data()
		{
			return (this->Base::imp.M_start);
		}
		value_type* data() const
		{
			return (this->Base::imp.M_start);
		}

		//--------- 그담은 ----------- 반복자관련 맴버 ----
		//이터레이터에 대한 생성자가 없으니 아마 형변환으로 리턴이겠지

		iterator begin() { return (iterator(this->Base::imp.M_start)); }
		const_iterator begin() const { return const_iterator(this->Base::imp.M_start); }

		iterator end() { return (iterator(this->Base::imp.M_finish)); }
		const_iterator end() const { return (const_iterator(this->Base::imp.M_finish)); }

		reverse_iterator rbegin() { return (reverse_iterator(this->end())); }
		const_reverse_iterator rbegin() const { return (const_reverse_iterator(this->end())); }

		reverse_iterator rend() { return (reverse_iterator(this->begin())); }
		const_reverse_iterator rend() const { return (const_reverse_iterator(this->begin())); }

		// ----------------그담은  ---------- 용량관련 --------------------
		bool empty() const
		{
			if (this->begin() == this->end())
				return (true);
			else
				return (false);
		}


		size_type size() const { return (size_type(this->Base::imp.M_finish - this->Base::imp.M_start)); }

		//max_size는 최대로 뽕뽑을 수 있는 길이
		size_type max_size() const
		{
//			R_allocator_type Ra = Base::transform_allocator();
			return (Ra.max_size());
		}
		
		size_type capacity() const { return (this->Base::imp.M_eos - this->Base::imp.M_start); }

		//벡터 개체의 최소 스토리지 길이를 예약하여
		//필요한경우 공간을 할당

		void reserve(size_type new_cap)
		{
			if (new_cap > max_size())
				throw (std::length_error("vector length mani giller"));
			else if (new_cap > capacity())
			{
	//			R_allocator_type Ra = Base::transform_allocator();
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
				//빼주는 이유는 그만큼 포인터를 이동해서 할당했기 때문에 
				//다시 원위치로 돌아가기 위하여 저걸 빼주게 됨
			}
		}
		
		void clear()
		{
	//		R_allocator_type Ra = Base::transform_allocator();
			size_type tmp = this->size();
			for (size_type i = 0; i < tmp; i++, Base::imp.M_finish--)
				Ra.destroy(this->Base::imp.M_finish);
		}

		//pos에 value를 삽입
		//새로운 벡터의 size가 현재 백터의 capacity보다 크면 재할당
		//그리고 내여쓰기(finish는 end와 같이 끝의 다음이라 상관ㄴㄴ)
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
		
		//근데 반복자인경우 구할때 굳이 이렇게 해야하나
		template<class InputIt>
		void insert(iterator pos, InputIt first, InputIt last,
				typename ft::enable_if<!ft::is_integral<InputIt>::result>::type* = 0)
		{
			size_type where = (size_type)(pos - begin());
			size_type many = ft::its_dif(first, last);
			if ((size() + many) > capacity() * 2)
				reserve(size() + many);//얘도 여기 추가
			else if ((size() + many) > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			for (pointer out = Base::imp.M_finish; out != Base::imp.M_start + where; out--)
				*(out + many - 1) = *(out - 1);
			for (int i = 0; first != last; first++, i++)
				*(Base::imp.M_start + where + i) = *first;
			Base::imp.M_finish += many;
		}
		
		//한놈만 지우기
		//어캐지우냐
		//search and destroy
		//지우고 지운만큼 떙겨쓰기
		//그리고 원소 하나를 지우는거니 당연히 size도 줄여야지
		iterator erase(iterator pos)
		{
			Ra.destroy(&(*pos));
			//포인터라길래 캐스팅 연산자 썼는데 안되더라고
			//그래서 킹쩔수없이 역참조한 것에 대한 주소값을 씀
			//뒤가 끝일경우 굳이 떙길 필요 없다
			if (pos + 1 != end())
			{
				for (int i = 0; i < Base::imp.M_finish - &(*pos) - 1; i++)
				{
					Ra.construct(&(*pos) + i, *(&(*pos) + i + 1));
					Ra.destroy(&(*pos) + i + 1);
				}
			}
			//하나만 지우면 알아서 되는 줄 알았는데
			//begin을 지웠는데 맨앞은 멀쩡하고 맨뒤가 지워짐 
			//그래서 땡겨쓰는걸 추가하니 이제서야 지워지고 
			//죽어있는줄 알았던 맨뒤가 살아남
			Base::imp.M_finish--;
			return (pos);
		}

		//범위로 지우기
		iterator erase(iterator first, iterator last)
		{
			int many = (int)(last - first);
			for (int i = 0; i < many; i++)
				Ra.destroy(&(*(first + i)));
			if (last != end())
			{
				for (int i = 0; i < Base::imp.M_finish - &(*last); i++)
				{
					Ra.construct(&(*first) + i, *(&(*last) + i));
					Ra.destroy(&(*last) + i);
				}
			}
			Base::imp.M_finish -= many;
			return (first);
		}

		//뒤에 하나 추가
		void push_back(const value_type& value)
		{
			if (size() + 1 > capacity())
				reserve(capacity() == 0 ? 2 : this->capacity() * 2);
			Ra.construct(Base::imp.M_finish, value);
			Base::imp.M_finish++;
		}

		//이건 빼는거
		void pop_back()
		{
			Ra.destroy(Base::imp.M_finish - 1);
			Base::imp.M_finish--;
		}

		//이건 reserve마냥 공간할당하고
		//추가로 채울거 있음 채우는거
		//근데 만약에 요구량이 컨테이너 크기보다 적다?
		//그럼 잘라버려
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
		//이번엔 값채워넣기
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
		
		//이건 벡터의 요소들을 바꾸는거
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
	};//vector
	
	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		if (x.size() != x.capacity() || y.size() != y.capacity())
			return (false);
		typename vector<T, Alloc>::const_iterator it_x = x.begin();
		typename vector<T, Alloc>::const_iterator it_y = y.begin();
		for (; it_x < x.end(); it_x++, it_y++)
		{
			if (it_y == y.end())
				return (false);
			if (*it_x != *it_y)
				return (false);
		}
		if (it_x != x.end() || it_y != y.end())
			return (false);
		return (true);
	}

	template <typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		return (!(x == y));
	}

	template <typename T, typename Alloc>
	bool operator<(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		typename vector<T, Alloc>::const_iterator it_x = x.begin();
		typename vector<T, Alloc>::const_iterator it_y = y.begin();
		for (; it_x < x.end(); it_x++, it_y++)
		{
			if (it_y == y.end())
				return (false);
			if (*it_x < *it_y)
				return (true);
			if (*it_x > *it_y)
				return (false);
		}
		if (it_y != y.end() || y.size() < y.capacity()) 
		//x는 다끝났는데 y가 더 있거나 할당된게 더있을경우 
			return (true);
		return (false);
	}

	template <typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		return (!(y < x));
	}

	template <typename T, typename Alloc>
	bool operator>(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		return ((y < x));
	}

	template <typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
	{
		return (!(x < y));
	}

	template <class T, class Alloc>
	void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
	{
		x.swap(y);
	}
	//그리고 맴버함수는 아닌 함수 오버로드
	//여기엔 friends아닌 다른놈들(비교연산자)쓴다
	//그리고 swap까지
	
	//그리고 템플릿 특화인 친구(vector<bool>)을 씀
}//ft

#endif
