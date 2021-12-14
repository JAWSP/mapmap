/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 14:28:40 by juhpark           #+#    #+#             */
/*   Updated: 2021/12/14 20:23:13 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MAP_HPP
# define FT_MAP_HPP

# include "utils.hpp"
# include "AVL_tree.hpp"
# include "AVL_iterator.hpp"
# include <functional>//less

//map은 연관 컨테이너
//연관 컨테이너는 키와 값처럼 관련이 있는 데이터를
//하나의 쌍으로 저장하는 컨테이너
//셋은 키가 곧 값 맵은 키랑 값이랑 따로
//이진트리로 정렬하여 보관하기때문에 메모리의 낭비가 심하지 않고
//데이터의 종류에 상관없이 일정한 성능을 보장

namespace ft
{
	//진짜 실제코드에서 저리되어있어요
	template <class Val1, class Val2, class Res>
	struct binary_function
	{
		typedef Val1	first_argument_type;
		typedef Val2	second_argument_type;
		typedef Res		result_type;
	};
	//key가 키
	//T가 값
	//pair는 키와 값을 표현하기 위한 수단
	template<class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	public:
		typedef Key												key_type;
		typedef T												mapped_type;
		typedef ft::pair<const Key, T>							value_type;
		typedef size_t											size_type;
		typedef ptrdiff_t										difference_type;
		typedef Compare											compare_type;
		typedef Alloc											allocator_type;
		typedef value_type&										reference;
		typedef	const value_type& 								const_reference;

		typedef	typename Alloc::pointer							pointer;
		typedef	typename Alloc::const_pointer					const_pointer;
		typedef	ft::AVL_iterator<value_type>					iterator;
		typedef	ft::AVL_const_iterator<value_type>				const_iterator;
		typedef ft::reverse_iterator<iterator>					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;
	private:
		typedef	ft::Node<value_type>									Node;
		typedef	ft::AVL_tree<value_type, compare_type, allocator_type>	tree_type;
		tree_type														tree;
		allocator_type													alloc;
		compare_type													comp;


	public:

		class value_compare : public ft::binary_function<value_type, value_type, bool>
		{
			friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) { }
		public:
			bool operator ()(const value_type& x, const value_type& y) const
			{
				return (comp(x.first, y.first));
			}
		};

		//여기서부턴 생성자
		//1. 디폴트 2. 특정 비교(?)랑 할당자가 지정 3. 특정 범위를 복사 4. 복사생성자
		 
	//	map() : tree() { }
		
		//comp에서도  = Compare()할 줄 알았는데 안쓰네
		//신기방기
		explicit map(const Compare& compare = Compare(), const Alloc& allocator = Alloc())
			: tree(comp, alloc), alloc(allocator), comp(compare)  { }


		//근데 원본코드를 보면 enable_if를 쓰진않음
		//그래도 써야지
		template<class InputIt>
		map(InputIt first, InputIt last,const Compare& compare = Compare(), const Alloc& allocator = Alloc(),
				typename ft::enable_if<!ft::is_integral<InputIt>::result, InputIt>::type* = 0)
				: tree(compare, allocator), alloc(allocator), comp(compare)
		{
			InputIt it = first;
			while (it != last)
			{
				tree.root = tree.insert(tree.root, NULL, *it);
				it++;
			}
		}

		//원래는 tree(other.tree)로 하면 원타치로 되긴한데
		//그리 하게 되면 어째서인지 원본이 바뀌면 얘도 바뀜
		map(const map& other) : tree(other.comp, other.alloc), alloc(other.alloc), comp(other.comp) 
		{
			this->insert(other.begin(), other.end());
		}

		//그담은 소멸자
		~map() { }

		//싹다 갈고 다시 채우고
		map& operator =(const map& other)
		{
			if (*this != other)
			{
				
			//	std::cout << (--other.end())->second << std::endl;

				clear();
		//		for (const_iterator it = other.begin(); it != other.end(); it++)
		//			std::cout << it->second << std::endl;
				std::cout << "qwqwqw" << std::endl;
				this->tree.node_alloc = other.tree.node_alloc;
				this->tree.value_alloc = other.tree.value_alloc;
				this->tree.node_compare = other.tree.node_compare;
				this->tree.count = 0;
				this->alloc = other.alloc;
				this->comp = other.comp;

				this->insert(other.begin(), other.end());
	//			std::cout << "after" << std::endl;
	//			for (const_iterator it = this->begin(); it != this->end(); it++)
	//				std::cout << it->second << std::endl;
			}
			return (*this);
		}

		//이건 할당자를 가져오는 친구
		allocator_type get_allocator() const
		{
			return (this->tree.value_alloc);
		}

//다음은 키에 해당되는 값에 접근하는 친구들
		mapped_type& at(const key_type& key)
		{
	//		std::cout << tree.root->value.first << std::endl;
			ft::Node<value_type>* target;
			value_type vvv(key, mapped_type());
			target = tree.searchNode(tree.root, vvv);
//			std::cout << "!!!" << std::endl;
	//		std::cout << target->value.first << std::endl;
			if (!target)
				throw (std::out_of_range("map bomwi bursurnam\n"));
			return (target->value.second);
		}

		const mapped_type& at(const key_type& key) const
		{
			ft::Node<const value_type>* target;
			value_type vvv(key, mapped_type());
			target = tree.searchNode(tree.root, vvv);
			if (!target)
				throw (std::out_of_range("map bomwi bursurnam\n"));
			return (target->value.second);
		}
		//at와 역할을 비슷한데 값을 넣는 기능까지 있는 놈
		//그래서 범위안에 없으면 추가하는것만 바뀜
		
		mapped_type& operator [](const key_type& key)
		{
			iterator it = lower_bound(key);
	//		std::cout << "key is " << key << std::endl;
	//		std::cout << "end is " << end()->first << std::endl;
			if (it == end() || key_comp()(key, it->first))
			{
				value_type vvv(key, mapped_type());
				it = insert(it, vvv);//여기에 value_type(key, 0)이 안되는 이유는 뭘까
			}
			else if (size() == 0)
			{
				value_type vvv(key, mapped_type());
				it = insert(it, vvv);
			}
			return (it->second);
		}


		//begin,end친구들
		//그리고 end쪽에서 아무고토 없다면 begin()을 내놓도록 만듦
		iterator begin() { return (iterator(tree.minest(tree.root))); }
		const_iterator begin() const { return (const_iterator(tree.const_minest(tree.root))); }

		iterator end()
		{
			if (size() == 0)
				return (begin());
			return (++iterator(tree.largest(tree.root)));
		}
		const_iterator end() const
		{
			if (size() == 0)
				return (begin());
			return (++const_iterator(tree.const_largest(tree.root)));
		}

		reverse_iterator rbegin() { return (reverse_iterator(tree.largest(tree.root))); }
		const_reverse_iterator rbegin() const { return (const_reverse_iterator(tree.largest(tree.root))); }

		reverse_iterator rend()
		{
			if (size() == 0)
				return (rbegin());
			return (reverse_iterator(tree.minest(tree.root)->left));
		}
		const_reverse_iterator rend() const
		{
			if (size() == 0)
				return (rbegin());
			return (const_reverse_iterator(tree.minest(tree.rooot)->left));
		}


		//그담은 사이즈, 비어있나, 최대 할당량
		bool empty() const { return (tree.empty()); }
		size_type size() const { return (tree.size()); }
		size_type max_size() const { return (tree.max_size()); }

		void clear()
		{
			tree.deleteDamn(tree.root);
		}

		//그담은 값을 추가해주는 insert
		
		//근데 요놈은 반환유형이 겁나 유니크함
		//중복아니면 false, 중복이면 true
		//iterator는 해당 노드가 들어있는 iterator를 반환한다고
		ft::pair<iterator, bool> insert(const value_type& value)
		{
			Node *target = tree.searchNode(tree.root, value);
			if (target)
				return (ft::make_pair(iterator(target), false));
			tree.root = tree.insert(tree.root, NULL, value);
			target = tree.searchNode(tree.root, value);
			return (ft::make_pair(iterator(target), true));
		}

		//얜 해당 이터레이터에 값을 넣는 놈
		//하지만 원본에서도 마찬가지로 저건 힌트만 주는거지
		//사실상 페이크
		//따라서 트리 형식에 맞게 넣어주면 되니까 그냥 평소대로 넣듯이 넣자
		iterator insert(iterator hint, const value_type& value)
		{
			(void)hint;
			Node *target = tree.searchNode(tree.root, value);
			if (target)
				return (iterator(target));
			tree.root = tree.insert(tree.root, NULL, value);
			target = tree.searchNode(tree.root, value);
			return (iterator(target));
		}

		//얜 해당 구간을 채워넣는 그런 구조다 이말이야
		template<class InputIt>
		void insert(InputIt first, InputIt last,
				typename ft::enable_if<!ft::is_integral<InputIt>::result, InputIt>::type* = 0)
		{
			InputIt it = first;
			while (it != last)
			{
				tree.root = tree.insert(tree.root, NULL, *it);
				it++;
			}
		}


		//그담은 해당 원소를 지우는 놈
		
		void erase(iterator pos)
		{
			tree.root = tree.remove(tree.root, *pos);
		}

		void erase(iterator first, iterator last)
		{
			if (first == begin() && last == end())
				clear();
			else
			{
				while (first != last)
				{
				//	iterator it = first;
				//	++first;
				//	std::cout << it->first << " is removed\n";
					erase(first++);//후위연산자 잡기술, 위는안됨
				}
			}
		}

		size_type erase(const key_type key)
		{
			value_type val = ft::make_pair(key, mapped_type());
			if (tree.searchNode(tree.root, val))
			{
				tree.root = tree.remove(tree.root, val);
				return (1);
			}
			return (0);
		}

		void swap(map& other)
		{
			Node *tmp = this->root;
			this->root = other->root;
			other->root = tmp;
		}

		//요기서부턴 특정 요소를 찾는 친구들
		
		size_type count(const key_type key)
		{
			value_type val = ft::make_pair(key, mapped_type());
			if (searchNode(tree.root, val))
				return (1);
			return (0);
		}

		iterator find(const key_type& key)
		{
			value_type val = ft::make_pair(key, mapped_type());
			ft::Node<value_type>* target = tree.searchNode(tree.root, val);
			if (target)
				return (iterator(target));
			return (end());
		}
		
		const_iterator find(const key_type& key) const
		{
			value_type val = ft::make_pair(key, mapped_type());
			ft::Node<const value_type>* target = tree.searchNode(tree.root, val);
			if (target)
				return (const_iterator(target));
			return (end());
		}

		//less a < b ->true
		//a > b -> false
		iterator lower_bound(const key_type& key)
		{
			if (key < tree.minest(tree.root)->value.first)
				return (this->begin());
			Node* cur = tree.root;
			while (1) 
			{
				//if (cur->right == NULL)
				//	std::cout << "yeah" << std::endl;
//				if (cur == NULL)//제일 크면 없을 수 있지
//					return (this->end());
				if (cur->left == NULL && cur->right == NULL)
					break ;
				if (comp(cur->value.first, key)) //f < k
				{
					if (cur->right)
						cur = cur->right;
					else
					{
						//if (++iterator(cur, tree.root) == this->end())
						//	std::cout << "endend" << std::endl;
						return (++iterator(cur));//걔보다 크긴한데 없으면 걔 다음꺼지
					}
				}
				else if (cur->value.first == key)
					return (iterator(cur));
				else
				{
					if (cur->left)
						cur = cur->left;
					else
						return (iterator(cur));
				}
			}
			if (size() == 0 || cur->value.first == key)//그냥 비어있을때나 맨밑 값이 같은경우
				return (iterator(cur));
			//std::cout << "next" << std::endl;
			return (++iterator(cur));
		}
		
		const_iterator lower_bound(const key_type& key) const
		{	
			if (key < tree.const_minest(tree.root)->value.first)
				return (this->begin());
			Node* cur = tree.root;
			//최소값보다 작으면 -> 최소값을 리턴
			while (1) 
			{
				if (cur->left == NULL && cur->right == NULL)
					break ;
				if (comp(cur->value.first, key))
				{
					if (cur->right)
						cur = cur->right;
					else
						return (++const_iterator(cur));
				}
				else if (cur->value.first == key)
					return (const_iterator(cur));
				else
				{
					if (cur->left)
						cur = cur->left;
					else
						return (const_iterator(cur));
				}
			}
			if (size() == 0 || cur->value.first == key)
				return (const_iterator(cur));
			//std::cout << "next" << std::endl;
			return (++const_iterator(cur));
		}
		
		//얘보다 작거나 같은 놈인줄 알았냐?
		//쟌넨 그냥 큰놈입니다
		iterator upper_bound(const key_type& key)
		{
			if (key < tree.minest(tree.root)->value.first)
				return (this->begin());
			Node* cur = tree.root;
			while (1) 
			{
				if (cur->left == NULL && cur->right == NULL)
					break ;
				if (comp(cur->value.first, key))
				{
					if (cur->right)
						cur = cur->right;
					else
						return (++iterator(cur));
				}
				else if (cur->value.first == key)
					return (++iterator(cur));
				else
				{
					if (cur->left)
						cur = cur->left;
					else
						return (iterator(cur));
				}
			}
			if (size() == 0)
				return (iterator(cur));
			return (++iterator(cur));
		}
		
		const_iterator upper_bound(const key_type& key) const
		{
			if (key < tree.const_minest(tree.root)->value.first)
				return (this->begin());
			Node* cur = tree.root;
			while (1) 
			{
				if (cur->left == NULL && cur->right == NULL)
					break ;
				if (comp(cur->value.first, key))
				{
					if (cur->right)
						cur = cur->right;
					else
						return (++const_iterator(cur));
				}
				else if (cur->value.first == key)
					return (++const_iterator(cur));
				else
				{
					if (cur->left)
						cur = cur->left;
					else
						return (const_iterator(cur));
				}
			}
			if (size() == 0)
				return (iterator(cur));
			return (++const_iterator(cur));
		}

		//lower, upper둘다 얻고 싶다면 이걸 쓰시오란 느낌
		ft::pair<iterator, iterator>equal_range(const key_type& key)
		{
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}
		
		ft::pair<const_iterator, const_iterator>equal_range(const key_type& key) const
		{
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}


		compare_type key_comp() const
		{
			return (comp);
		}

		//위가 key에 대하여 비교를 한다면
		//아래는 value_type(pair)에 대하여 비교를 한다
		value_compare value_comp() const
		{
			return (value_compare(comp));
		}
		template <class K, class V, class C, class A>
		friend bool operator ==(const map<K, V, C, A>&, const map<K, V, C, A>&);
		template <class K, class V, class C, class A>
		friend bool operator <(const map<K, T, C, A>&, const map<K, V, C, A>&);

	};//map

	template <class Key, class T, class Compare, class Alloc>
	bool operator ==(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y)
	{
			return (x.tree == y.tree);
	}
	template <class Key, class T, class Compare, class Alloc>
	bool operator !=(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y)
	{ return (!(x == y)); }
	template <class Key, class T, class Compare, class Alloc>
	bool operator <(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return (x.tree < y.tree); }
	template <class Key, class T, class Compare, class Alloc>
	bool operator <=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return (!(y < x)); }
	template <class Key, class T, class Compare, class Alloc>
	bool operator >(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return ((y < x)); }
	template <class Key, class T, class Compare, class Alloc>
	bool operator >=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return (!(x < y)); }

	//말그대로 바꾸는거
	template<class Key, class T, class Compare, class Alloc>
	void swap(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y)
	{ x.swap(y); }

}//ft


#endif
