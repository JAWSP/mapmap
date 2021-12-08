#include <iostream>
#include <string>
#include <map>
#include <utility>
#include "AVL_tree.hpp"
#include "AVL_iterator.hpp"
#include "utils.hpp"
#include "ft_map.hpp"

int main()
{
	//
	
	ft::AVL_tree<ft::pair<int, int>, std::less<ft::pair<int, int> > > tree;
	ft::pair<int, int> test(1, 2);
	ft::pair<int, int> test1(10, 2);
	ft::pair<int, int> test2(2, 2);
	ft::pair<int, int> test3(5, 2);
	ft::pair<int, int> test4(4, 2);
	ft::pair<int, int> test5(0, 2);
	ft::pair<int, int> test6(3, 2);
	ft::pair<int, int> test7(11, 2);
	ft::pair<int, int> test8(12, 2);
	ft::pair<int, int> test9(6, 2);
//	ft::pair<int, int> test10(4, 2);
	tree.root = tree.insert(tree.root, NULL, test);
	tree.root = tree.insert(tree.root, NULL, test1);
	tree.root = tree.insert(tree.root, NULL, test2);
	tree.root = tree.insert(tree.root, NULL, test3);
	tree.root = tree.insert(tree.root, NULL, test4);
	tree.root = tree.insert(tree.root, NULL, test5);
	tree.root = tree.insert(tree.root, NULL, test6);
	tree.root = tree.insert(tree.root, NULL, test7);
	tree.root = tree.insert(tree.root, NULL, test8);
	tree.root = tree.insert(tree.root, NULL, test9);
	tree.root = tree.insert(tree.root, NULL, ft::make_pair(4, 2));
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	/*
	ft::AVL_iterator<ft::pair<int, int> > it(tree.minest(tree.root));
	ft::AVL_iterator<ft::pair<int, int> > fin(tree.largest(tree.root));
	while (it != fin)
	{
		std::cout << it->first << std::endl;
		it++;
	}
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	//AVL안에선 저리 쓰지만 map에선 make pair로 페어 만든다음에 넣기
//	tree.root = tree.remove(tree.root, test2);
	tree.printNode(tree.root);
	ft::Node<ft::pair<int, int> >* target;
	ft::pair<int, int> asd(200, 100);
	*/
	//////
	/*
	std::map<int, char> container;

	for (int i = 0; i < 10; ++i)
		container.insert(make_pair(i, char(i + 65)));

	std::map<int, char>::iterator itr;
	std::cout << *itr << std::endl;
	*/

	ft::map<int, int> ww;
	ww[2] = 3;
	ft::map<int, int>::iterator ii = ww.begin();
	ii++;
	if (ii == ww.end())
		std::cout << "QWwqwq" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	ft::map<int, int> qqq;
	qqq[1] = 2;
	qqq[10] = 2;
	qqq[2] = 2;
	qqq[5] = 3;
	qqq[4] = 3;
	qqq[0] = 3;
	qqq[3] = 2;
	qqq[11] = 6;
	qqq[12] = 3;
	qqq[6] = 4;
	qqq[4] = 8;
	std::cout << "2 is 3" << std::endl;
	std::cout << qqq.at(2) << std::endl;
	ft::map<int, int>::const_iterator qw = qqq.begin();
	
//	qw = qqq.begin();
	
//	std::cout << qqq.end()->first << std::endl;
	/*
	while (qw != qqq.end())
	{
		std::cout << "key is : " << qw->first << " val is : " << qw->second << std::endl;
		qw++;
	}
	*/
}

/*
	Node *asd = newNode(1);
	std::cout << "add 1" << std::endl;
	asd = insert(asd, 10);
	asd = insert(asd, 2);
	asd = insert(asd, 5);
	asd = insert(asd, 4);
	asd = insert(asd, 0);
	asd = insert(asd, 3);
	asd = remove(asd, NULL, 2);
	std::cout << asd->value << std::endl;
	printNode(asd);
	asd = insert(asd, 11);
	if (asd->right->right == NULL)
		std::cout << "qwqqwwq" << std::endl;

*/
