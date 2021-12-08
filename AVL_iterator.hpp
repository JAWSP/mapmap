/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:33:58 by juhpark           #+#    #+#             */
/*   Updated: 2021/12/08 16:50:28 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVL_ITERATOR_HPP
# define AVL_ITERATOR_HPP

//map의 반복자는 양뱡향
# include "utils.hpp"
# include "AVL_tree.hpp"

namespace ft
{
	//요놈은 트리가 어찌 돌아가는지 보여주는 반복자
	//트리는 곧 루트에서 이어지기 때문에 AVL트리 객체 자체를 복사 안해도 될듯
	//그데 템플릿 인자를 뭘로받냐 ->T니까 pair겠지 <int, int>막 이런걸루다가
	//그런데 저거랑 AVL트리랑 어떻게 연결하지?
	//그건 반복자는 반복자니깐 AVL트리를 벡터의 imp로 취급을 해서
	//거기로 노드를 옮기는게 낫지 않나 싶다
	//그럼 해당 노드 말고 트리 자체는 어떻게 옮길껀데?
	//매개변수로 계속 루트를 받아와야하나
	//다른 사람들이 한걸 보면 기존 트리에 저 반복자가 변수(?)로 들가는듯한느낌
	//원본코드는 겁나 간단하게 트리의 반복자요 이리 했는데
	//난 기존 컨테이너 반복자까지 뒤집어쓴 무언가가 되어버림
	template<typename T>
	class AVL_iterator : public ft::iterator<ft::bidirectional_iterator_tag, T>
	{
	private:
		typedef	ft::iterator<ft::bidirectional_iterator_tag, T> iterator_type;
		typedef	ft::Node<T> 									Node;


		Node* minest(Node *node)
		{
			Node* res = node;
			if (!res)
				return (NULL);
			while (res->left)
				res = res->left;
			return (res);
		}

		Node* largest(Node *node)
		{
			Node* res = node;
			if (!res)
				return (NULL);
			while (res->right)
				res = res->right;
			return (res);
		}

		//current는 맨처음(최소값) 위치로 잡고 앞뒤 변수 붙일바에
		//그냥 함수로 갱신시키는게 나을듯
	public:
		typedef typename iterator_type::iterator_category		iterator_category;
		typedef typename iterator_type::value_type				value_type;
		typedef typename iterator_type::difference_type			difference_type;
		typedef typename iterator_type::reference				reference;
		typedef typename iterator_type::pointer					pointer;
		Node*	current;
		Node*	root;

		AVL_iterator() : current(), root() { }
		virtual ~AVL_iterator() { }
		explicit AVL_iterator(Node* n, Node* r) : current(n), root(r) { }
		AVL_iterator(const AVL_iterator &A) : current(A.current), root(A.root) { }
		AVL_iterator& operator =(const AVL_iterator &A)
		{
			if (*this != A)
			{
				this->current = A.current;
				this->root = A.root;
			}
			return (*this);
		}
		
	/*	
		//const형으로 바꿔주는친구
		operator AVL_iterator<const T> () const
		{
			return (AVL_iterator<const T>(this->current, this->root));
		}
		//근데 안먹힘
		iterator cast_const() const
		{
			return (iterator(const_cast<typename iterator::Node *>(current, root))
		}
		*/

		//전진
		//기존 tree에 있던 next, prev를 가져옴
		//근데 접근하기엔 트리를 객체선언을 안해서.. ㅎㅎ
		//나의 머리론 직접 짜는게 한계인듯 
		AVL_iterator &operator ++()//전위
		{
			if (current == largest(root))
				current = current->right;
			else if (root)
			{
				if (current->right)
				{
					current = minest(current->right);
					return (*this);
				}
				Node *tmp = current;
				while (tmp->parent)
				{
					if (tmp == tmp->parent->left)
						break ;
					tmp = tmp->parent;
				}
				if (!tmp->parent)
					current = tmp;
				else
					current = tmp->parent;
			}
			return (*this);
		}

		const AVL_iterator operator ++(int)//후위
		{
			AVL_iterator tmp(*this);
			operator++();
			return (tmp);
		}

		AVL_iterator &operator --()//전위
		{
			if (current == minest(root))
				current = current->left;
			else if (root)
			{
				if (current->left)
				{
					current = largest(current->left);
					return (*this);
				}
				Node *tmp = current;
				while (tmp->parent)
				{
					if (tmp == tmp->parent->right)
						break ;
					tmp = tmp->parent;
				}
				if (!tmp->parent)
					current = tmp;
				else
					current = tmp->parent;
			}
			return (*this);
		}

		const AVL_iterator operator --(int)//후위
		{
			AVL_iterator tmp(*this);
			operator--();
			return (tmp);
		}

		//그담은 읽기/쓰기/접근에 해당되는 친구들
		//std를 기준으로 쌩으로 *으로 접근하려니 무수한 에러를 밷고
		//이터레이터->뭐시기로 출력을 하는듯하다
		reference operator *() const
		{
			return (current->value);
		}
		
		pointer	operator ->() const
		{
			return (&current->value);
		}
		//그담은 같냐 틀리냐에 대한 연산자
		bool operator ==(const AVL_iterator& other)
		{
			return (this->current == other.current);
		}

		bool operator !=(const AVL_iterator& other)
		{
			return (this->current != other.current);
		}
	};//bid

	template<typename T>
	bool operator==(const AVL_iterator<T>& x, const AVL_iterator<const T>& y)
	{
		return (x.current == y.current);
	}

	template<typename T>
	bool operator!=(const AVL_iterator<T>& x, const AVL_iterator<const T>& y)
	{
		return (x.current != y.current);
	}


	//결국은 const변환자로 바꾸기에 실패하게 되는데...
	template<typename T>
	class AVL_const_iterator : public ft::iterator<ft::bidirectional_iterator_tag, T>
	{
	private:
		typedef	ft::iterator<ft::bidirectional_iterator_tag, T> iterator_type;
		typedef	ft::Node<const T> 								Node;


		Node* minest(Node *node)
		{
			Node* res = node;
			if (!res)
				return (NULL);
			while (res->left)
				res = res->left;
			return (res);
		}

		Node* largest(Node *node)
		{
			Node* res = node;
			if (!res)
				return (NULL);
			while (res->right)
				res = res->right;
			return (res);
		}

		//current는 맨처음(최소값) 위치로 잡고 앞뒤 변수 붙일바에
		//그냥 함수로 갱신시키는게 나을듯
	public:
		typedef typename iterator_type::iterator_category		iterator_category;
		typedef typename iterator_type::value_type				value_type;
		typedef typename iterator_type::difference_type			difference_type;
		typedef typename iterator_type::reference				reference;
		typedef typename iterator_type::pointer					pointer;
		Node*	current;
		Node*	root;

		AVL_const_iterator() : current(), root() { }
		virtual ~AVL_const_iterator() { }
		explicit AVL_const_iterator(Node* n, Node* r) : current(n), root(r) { }
		AVL_const_iterator(const ft::AVL_iterator<T> &A) : current(A.current), root(A.root) { std::cout << "ttt" <<std::endl;}
		AVL_const_iterator(const AVL_const_iterator &A) : current(A.current), root(A.root) { }
		AVL_const_iterator& operator =(const AVL_const_iterator &A)
		{
			if (*this != A)
			{
				this->current = A.current;
				this->root = A.root;
			}
			return (*this);
		}

		/*
		//const형으로 바꿔주는친구
		operator AVL_iterator<const T> () const
		{
			return (AVL_iterator<const T>(this->current, this->root));
		}
		//근데 안먹힘
		*/

		//전진
		//기존 tree에 있던 next, prev를 가져옴
		//근데 접근하기엔 트리를 객체선언을 안해서.. ㅎㅎ
		//나의 머리론 직접 짜는게 한계인듯
		AVL_const_iterator &operator ++()//전위
		{
			if (current == largest(root))
				current = current->right;
			else if (root)
			{
				if (current->right)
				{
					current = minest(current->right);
					return (*this);
				}
				Node *tmp = current;
				while (tmp->parent)
				{
					if (tmp == tmp->parent->left)
						break ;
					tmp = tmp->parent;
				}
				if (!tmp->parent)
					current = tmp;
				else
					current = tmp->parent;
			}
			return (*this);
		}

		const AVL_const_iterator operator ++(int)//후위
		{
			AVL_const_iterator tmp(*this);
			operator++();
			return (tmp);
		}

		AVL_const_iterator &operator --()//전위
		{
			if (current == minest(root))
				current = current->left;
			else if (root)
			{
				if (current->left)
				{
					current = largest(current->left);
					return (*this);
				}
				Node *tmp = current;
				while (tmp->parent)
				{
					if (tmp == tmp->parent->right)
						break ;
					tmp = tmp->parent;
				}
				if (!tmp->parent)
					current = tmp;
				else
					current = tmp->parent;
			}
			return (*this);
		}

		const AVL_const_iterator operator --(int)//후위
		{
			AVL_const_iterator tmp(*this);
			operator--();
			return (tmp);
		}

		//그담은 읽기/쓰기/접근에 해당되는 친구들
		//std를 기준으로 쌩으로 *으로 접근하려니 무수한 에러를 밷고
		//이터레이터->뭐시기로 출력을 하는듯하다
		reference operator *() const
		{
			return (current->value);
		}

		pointer	operator ->() const
		{
			return (&current->value);
		}
		//그담은 같냐 틀리냐에 대한 연산자
		bool operator ==(const AVL_const_iterator& other)
		{
			return (this->current == other.current);
		}

		bool operator !=(const AVL_const_iterator& other)
		{
			return (this->current != other.current);
		}
	};//bid

	template<typename T>
	bool operator==(const AVL_const_iterator<T>& x, const AVL_const_iterator<const T>& y)
	{
		return (x.current == y.current);
	}

	template<typename T>
	bool operator!=(const AVL_const_iterator<T>& x, const AVL_const_iterator<const T>& y)
	{
		return (x.current != y.current);
	}



}//ft

#endif
