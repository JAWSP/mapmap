/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL_tree.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhpark <juhpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 12:22:36 by juhpark           #+#    #+#             */
/*   Updated: 2021/12/08 17:06:05 by juhpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVL_TREE_HPP
# define AVL_TREE_HPP

# include "utils.hpp"
//원본은 레드블랙이지만
//이게 더 쉽고 같은 균형트리를 유지하기 위한 이진트리라고 해서 이거씀
//그리고 map의 iterator가 요 트리를 조작하고
//map에선 이 트리의 루트노드만 있다고 한다
//map의 맴버함수가 iterator를 조작하는듯하낟
//일단 트리를 만들어보

namespace ft
{
	//Node에 pair를 넣을까 AVL에 값을 넣을까 고민하다가 AVL에 넣기로
	template<class T>
	struct Node
	{
		typedef T			value_type;
		struct Node*		left;
		struct Node*		right;
		struct Node*		parent;
//		->얜 넣으면 빨라지는 대신 복잡해진다는데, 아직 얘를 넣을 필요 모르겠어
//		이젠 필요해보일듯->생각해보니 --같은 연산자쓸때 바로 접근을 해야하자나
		value_type			value;


		Node() : left(NULL), right(NULL), parent(NULL), value() { }

		Node(value_type& other) : left(NULL), right(NULL), parent(NULL), value(other) { }

		Node(const Node& N) : left(N.left), right(N.right), parent(N.parent), value(N.value) { }

		~Node() { } 

		Node& operator =(const Node& N)
		{
			if (*this != N)
			{
				this->left = N.left;
				this->right = N.right;
				this->parent = N.parent;
				this->value = N.value;
			}
			return (*this);
		}

		bool operator ==(const Node &N)
		{
			if (this->value == N->value)
				return (true);
			return (false);
		}

		bool operator !=(const Node &N)
		{
			return (!(*this == N));
		}
	};//node

	template<class T, class Compare, class Alloc = std::allocator<T>, class N_Alloc = std::allocator<ft::Node<T> > >
	class AVL_tree
	{
	public:
	//	typedef Key					key_type;
		//pair의 첫번째 인자이긴 한데 
		//oop를 고려하자면 저것이 필요했었나 싶기도함
		//어차피 비교하는 함수도 만들었는데 말이야
		typedef T					value_type;//pair그자체
		typedef N_Alloc				allocator_type;//node를 할당
		typedef ft::Node<T>			Node;
		typedef Alloc				value_allocator_type;//pair를 할당
		typedef size_t				size_type;

//	private:
		Node* 			root;
//		Node* 			curr;
		//다른 후보로는 맨 마지막 노드도 있다
		allocator_type			node_alloc;
		value_allocator_type	value_alloc;
		Compare					node_compare;
		size_type				count;

//	public:
		//일단 이리하고 나중에 필요할때 챙기는걸루
		//디폴트는 원래도 저렇게 디폴트로 되어 있었다
//		AVL_tree() {}

		AVL_tree(const Compare& node_comp = Compare(), const value_allocator_type& value_allocator = value_allocator_type(), const allocator_type& node_allocator = allocator_type())
			: node_compare(node_comp), value_alloc(value_allocator), node_alloc(node_allocator)
		{
			//std::cout << "QQQQ" << std::endl;
			root = node_alloc.allocate(1);//일단 할당하기만 하고 나중에 값넣을때 값만 넣는걸루
			root->left = NULL;
			root->right = NULL;
			root->parent = NULL;
		//	curr = NULL;
			count = 0;
		}

		AVL_tree(const AVL_tree& other)
		{
			root = other.root;
		//	curr = other.curr;
			node_alloc = other.node_alloc;
			value_alloc = other.value_alloc;
			node_compare = other.node_comapre;
			count = other.count;
		}

//보통 이렇게 되겠지
		~AVL_tree()
		{
			deleteDamn(root);
		//	std::cout << "bye" << std::endl;
			//파괴자는 어쩌지
			//쟤 하나만 지워도 되는건가
			//while문돌아서 처음부터 끝까지 돌아서 지우는게 아닌가
			//일단은 임시로 위처럼 만들어봅시다
		}


		//일단 만들어야 할게
		//1. 새로운 노드 ㅇ
		//2. 하나 추가 ㅇ
		//3. 하나 제거 ㅇ
		//4. 균형 체크 ㅇ
		//5. 노드 회전 ㅇ
		//6. 다음 노드 부르기->걔보다 큰값
		//7. 이전 노드 부르기->걔보다 작은값
		//8. 맨 앞 노드(최소값)부르기->맨왼쪽 ㅇ
		//9. 맨 뒤 노드(최대값)부르기->맨오른쪽 ㅇ
		//예제에서 만든대로 하면 될듯
		//어차피 pair이니깐
		

		Node* new_node(Node* parent, value_type& val)
		{
			if (count == 0)
				node_alloc.construct(root, val);
			else
			{
				Node* tmp = node_alloc.allocate(1);
				node_alloc.construct(tmp, val);
				tmp->left = NULL;
				tmp->right = NULL;
				tmp->parent = parent;
				count++;
				return (tmp);
			}
			count++;
			return (root);
		}


		//노드 회전부터
		Node* Rotate_LL(Node *z)
		{
		//	std::cout << "LL" << std::endl;
			Node *y = z->left;
			Node *T2 = NULL;

			if (y->right)
				T2 = y->right;
			y->right = z;
			y->parent = z->parent;
			z->parent = y;
			if (T2)
			{
				z->left = T2;
				T2->parent = z;
			}
			else
				z->left = NULL;
			return (y);
		}

		Node* Rotate_RR(Node *z)
		{
		//	std::cout << "RR" << std::endl;
			Node *y = z->right;
			Node *T2 = NULL;

			if (y->left)
				T2 = y->left;
			y->left = z;
			y->parent = z->parent;
			z->parent = y;
			if (T2)
			{
				z->right = T2;
				T2->parent = z;
			}
			else
				z->right = NULL;
			return (y);
		}

		Node* Rotate_RL(Node *z)
		{
		//	std::cout << "RL" << std::endl;
			Node *y = z->right;
			z->right = Rotate_LL(y);
			z = Rotate_RR(z);
			return (z);
		}

		Node* Rotate_LR(Node *z)
		{
		//	std::cout << "LR" << std::endl;
			Node *y = z->left;
			z->left = Rotate_RR(y);//여기서 알아서 부모가 바뀌게 됨
			z = Rotate_LL(z);

			return (z);
		}

		//그담은 균형맞추기
		//높이를 구하고
		//높이차(balance factor)를 구하고
		//그걸루 비교해서 돌릴지 말지 결정
		int get_height(Node *node)
		{
			int height = 0;
			if (node != NULL)
			{
				if (node->left == NULL && node->right == NULL)
					height = 1;
				else
				{
					int left_child_height = get_height(node->left);
					int right_child_height = get_height(node->right);
					if (left_child_height > right_child_height)//더 큰놈이 실제 높이
						height = left_child_height + 1;
					else//만약 같을때 해당되는데 그럼 어차피 상관이 없는거자나
						height = right_child_height + 1;
				}
			}

		//	std::cout << "height is " << height << std::endl;
			return (height);
		}

		int get_BF(Node *node)
		{
			if (node == NULL || (node->left == NULL && node->right == NULL)) //자식이 없거나 아예 없을경우
				return (0);
			return (get_height(node->left) - get_height(node->right));
		}

		Node* rebalance(Node *node)
		{
			int BF = get_BF(node);
			int left_BF = get_BF(node->left);
			int right_BF = get_BF(node->right);

//			std::cout << "key : " << node->value << " BF : " << BF << std::endl;
			if (BF > 1 && left_BF == 1)
				return Rotate_LL(node);
			else if (BF < -1 && right_BF == -1)
				return Rotate_RR(node);
			else if (BF > 1 && left_BF == -1)
				return Rotate_LR(node);
			else if (BF < -1 && right_BF == 1)
				return Rotate_RL(node);
			return (node);
		}

		//그담은 노드추가
		//원랜 루트를 기준으로 할까 싶었는데
		//그건 생각이 짧았어
		//그래서 물어본 결과 노드떙추가는 삭제하는걸루
		Node* insert(Node *node, Node *parent, value_type key)
		{
			//없거나 끝이면 생성
			if (node == NULL || count == 0)
			{
		//		std::cout << "new node is "<< key.first << std::endl;
				return new_node(parent, key);
				//처음엔 node->parent로 했었는데
				//없는노드에 parent 찾아서 세그먼트남
			}
			//아니면 계속 내려감
			if (key > node->value)
				node->right = insert(node->right, node, key);
			else if (key < node->value)
				node->left = insert(node->left, node, key);
			else //같은경우
				return (node);
			node = rebalance(node);
			return (node);
		}
/*
		Node* searchMinest(Node *node)
		{
			if (!node)
				return (NULL);
			if (!node->left)
				return (node);
			else
				return (searchMinest(node->left));
		}
*/
		Node* remove(Node* node, value_type target)
		{
			if (node == NULL)
				return (NULL);
			if (node->value > target)
				node->left = remove(node->left, target);
			else if (node->value < target)
				node->right = remove(node->right, target);
			else if (node->value == target)
			{
				//삭제하는경우
				//1. 자식이 0
				//2. 자식이 1종
				//3. 자식이 2종
				//1.
				if (node->left == NULL && node->right == NULL)
				{
					if (node->parent->left == node)
					{
						node->parent->left = NULL;
						node_alloc.destroy(node);
						node_alloc.deallocate(node, 1);
						count--;
						//위에선 끊고 아래에선 삭제하고
					}
					else if (node->parent->right == node)
					{
						node->parent->right = NULL;
						node_alloc.destroy(node);
						node_alloc.deallocate(node, 1);
						count--;
					}
					//여기서 rebalance를 안하는 이유
					//지가 사라졌는데 그자리에서 확인할 필요가 없다
					//부모쪽에서 어차피 rebalance로 가기때문
					return (NULL);
				}
				//2
				else if (node->left == NULL || node->right == NULL)
				{
					Node* tmp;
					if (node->left)
						tmp = node->left;
					else
						tmp = node->right;
					tmp->parent = node->parent;
					if (node->parent->left == node)
					{
						node->parent->left = tmp;
						node_alloc.destroy(node);
						node_alloc.deallocate(node, 1);
						count--;
					}
					else if (node->parent->right == node)
					{
						node->parent->right = tmp;
						node_alloc.destroy(node);
						node_alloc.deallocate(node, 1);
						count--;
					}
					tmp = rebalance(tmp);
					return (tmp);
				}
				//3
				//이어 붙이기엔 애매하니 자식 후보군에서 고른다
				else if (node->left && node->right)
				{
					Node *SaChungWang_Jung_CheiYakChoi = minest(node->right);
					value_type tmp = SaChungWang_Jung_CheiYakChoi->value;
					//대신할 걸 찾아서
					node = remove(node, SaChungWang_Jung_CheiYakChoi->value);
					//기존에 있던 위치를 지우고
					node->value = tmp;//그냥 값만 바꿔치기이니 값만 바꿔줍시다
					count--;
					node = rebalance(node);
					return (node);
					//그걸 삭제할 값 대신으로 넣는다
				}
			}
			else
				return (node);
			node = rebalance(node);
			return (node);
		}
		//그 다음은 최대,최소값 찾는 노드
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
			{
				res = res->right;
			}
			return (res);
		}

		//그다음은 다음/이전노드
		//출력순서대로 다음 이전노드를 하면 될 듯 하다
		//
		Node* nextNode(Node *node)
		{
			if (node->right)
				return (minest(node->right));//우측 자식이 있으면 최약체를 구한다
			//부모로 올라갈때는 계속 우측으로 갔으니 루트이거나 혹은 부모가 누군가의 왼쪽자식일때까지 올라가면 됨
			Node *tmp = node;
			while (tmp->parent) //tmp->parent == NULL라면 멈춤
			{	
				if (tmp == tmp->parent->left)
					break;
				tmp = tmp->parent;
			}
			if (tmp == root)
				return (root);//root의 부모는 없고, 부모없는놈은 root뿐이니 이렇게 
			return (tmp->parent);
		}

		Node* prevNode(Node *node)
		{
			if (node->left)
				return (largest(node->left));//좌측 자식이 있다면 좌측 최대로 감
			Node *tmp = node;
			while (tmp->parent)
			{
				if (tmp == tmp->parent->right)
					break;
				tmp = tmp->parent;
			}
			if (tmp == root)
				return (root);
			return (node->parent);
		}

		Node* searchNode(Node *tree, const value_type& target)
		{
			Node *res = tree;
			//std::cout << res->value.first << " " << target.first << std::endl;
			if (res)
			{
				if (res->value.first > target.first && res->left)
				{
					//std::cout << "ASd" << std::endl;
					return (searchNode(res->left, target));
				}
				else if (res->value.first < target.first && res->right)
				{
					//std::cout << "BSd" << std::endl;
					return (searchNode(res->right, target));
				}
				else if (res->value.first == target.first)
				{
					//std::cout << "CSd" << std::endl;
					return (res);
				}
			}
			//std::cout << "is NULL" << std::endl;
			return (NULL);
		}

		void printNode(Node *node)
		{
			if (node->left)
				printNode(node->left);
//			std::cout << node->value.first << std::endl;
			if (node->right)
				printNode(node->right);
		}

		//전부다 삭제
		void deleteDamn(Node *node)
		{
			Node *rt = node;
			node = minest(node);
			Node *del;
			int cycle = 0;
			while (cycle < 2)
			{
				if (node->right)//오른쪽으로 넘어가야지
				{
					node = largest(node->right);
					continue ;
				}
				if (node->left)//오른쪽으로 넘어가야지
				{
					node = minest(node->left);
					continue ;
				}
				del = node;//부모와의 관계를 찾기위하여 잠시 냅둠
	//			std::cout << del->value.first << std::endl;
				if (node->parent) //노드가 둘 이상일때
					node = node->parent;
				if (node->left == del)
				{
					node->left = NULL;
					node_alloc.destroy(node->left);
					node_alloc.deallocate(node->left, 1);
					count--;
					//위에선 끊고 아래에선 삭제하고
				}
				else if (node->right == del)
				{
					node->right = NULL;
					node_alloc.destroy(node->right);
					node_alloc.deallocate(node->right, 1);
					count--;
				}
				if (node == rt)
					cycle++;//왼쪽으로 다 볼일 봤으면 오른쪽으로 가서 사이클을 돌린다
			}
			if (node->parent) //마지막으로 기준이 되는 해당 루트를 삭제
				node = node->parent;
			if (node->left == del)
			{
				node->left = NULL;
				node_alloc.destroy(node->left);
				node_alloc.deallocate(node->left, 1);
				count--;
			}
			else if (node->right == del)
			{
				node->right = NULL;
				node_alloc.destroy(node->right);
				node_alloc.deallocate(node->right, 1);
				count--;
			}
		}
		//이걸 이용하면 부분을 지우는 것두 가능하지 않을까

		//비어있나 보는거
		//추가 될때마다 추가된 부분을 curr가 저장하니
		//curr가 없다 ->점마는 비어있다
		//->수정
		//counte가 0이면 없는거니 0이면 true
		bool empty() const
		{
			if (count == 0)
				return (true);
			else
				return (false);
		}

		size_type max_size() const
		{
			return (node_alloc.max_size());
		}

		//얜 처음엔 반복자를 통해서 계산을 시도해볼까 싶었는데
		//생각해보니 -연산은 못넣고 ++만 넣어지는데
		//어차피 ++만 넣을꺼면 상관없지 않을까 생각함
		//그리고 찾아보니 노드를 만들고 뺄때마다 count를 넣었던것
		size_type size() const { return (this->count); }

//		template<class T, class Compare, class Alloc, class N_Alloc>
//		friend bool operator ==(const AVL_tree<T, Compare, Alloc, N_Alloc>&, const AVL_tree<T, Compare, Alloc, N_Alloc>&);
//		template<class T, class Compare, class Alloc, class N_Alloc>
//		friend bool operator <(const AVL_tree<T, Compare, Alloc, N_Alloc>&, const AVL_tree<T, Compare, Alloc, N_Alloc>&);
//얜 굳이 쓸 필요가 없었네

	};//avl
		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator ==(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{
			if (x.count != y.count)
				return (false);
			ft::Node<T>* cur = x.minest();
			ft::Node<T>* fin = x.largest();
			ft::Node<T>* cop = y.minest();
			ft::Node<T>* y_fin = y.largest();
			while (cur != fin)
			{
				if (cop == y_fin)
					return (false);
				if (cur->value != cop->value)
					return (false);
				cur = x.nextNode(cur);
				cop = y.nextNode(cop);
			}
			if (!(cur == fin && cop == y_fin))
				return (false);
			return (true);
		}

		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator !=(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{
			return (!(x == y));
		}

		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator <(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{
			ft::Node<T>* cur = x.minest();
			ft::Node<T>* fin = x.largest();
			ft::Node<T>* cop = y.minest();
			ft::Node<T>* y_fin = y.largest();
			while (cur != fin)
			{
				if (cop == y_fin)
					return (false);
				if (cur->value < cop->value)
					return (true);
				cur = x.nextNode(cur);
				cop = y.nextNode(cop);
			}
			if (cop != y_fin)
				return (true);
			return (false);
		}

		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator <=(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{ return (!(y < x)); }


		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator >(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{ return (y < x); }
		
		template<class T, class Compare, class Alloc, class N_Alloc>
		bool operator >=(const AVL_tree<T, Compare, Alloc, N_Alloc>& x, const AVL_tree<T, Compare, Alloc, N_Alloc>& y)
		{ return (!(y < x)); }
}//ft


#endif
